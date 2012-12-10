/*-------------------------------------*\
|
|  File Name: VM.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:22:41 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "VM.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cctype>
#include "VirgoInstruction.hpp"

#define EVER ;;

VM::VM() : mProc(mMem), mVirgo(false) {

	//initialize main memory
	memset(mMem, 0xFF, MEM_SIZE);
	Instruction::InitializeOpcodes();
}


// This function will load a pure object file. No PE style header, just straight machine code
// The object file will be copied into memory at address 0x0000
// Execution will begin at address 0x0000 by default

int VM::LoadFlatFile(const char* filename) {

	//reset everything
	mLoaded = false;
	mProc.Initialize();

	//reset everything
	mLoaded = false;
	mProc.Initialize();

	std::ifstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);
	if(fin.fail()) { 
		return VM_ERR_FOPEN;
	}

	int i = 0;
	while(!fin.eof()) {
		if(((i + 1) * 1024) >= MEM_SIZE)
			return VM_ERR_BIG_FILE;

		fin.read((char*)(mMem + (i++) * 1024), 1024);
		if(fin.bad()) { //an error occurred
			fin.close();
			return VM_ERR_FREAD;
		}
	};

	mLoaded = true;
	mVirgo = false;

	return VM_SUCCESS;

}

int VM::LoadVirgoFile(const char* filename) {

	mLoaded = false;
	mProc.Initialize();

	mInstructions.clear();
	memset(mMem, 0xFF, MEM_SIZE);

	std::ifstream fin;
	fin.open(filename, std::ios_base::in);
	if(fin.fail()) {
		return VM_ERR_FOPEN;
	}

	//parse header
	int numInitArgs, numLines, addrStart, bytesTotal;
	fin >> numInitArgs;
	fin >> numLines;
	fin >> addrStart;
	fin >> bytesTotal;

	int i = 0;
	int hexSize;
	char* hex;
	char label[128];
	char disasm[256];
	int addr;
	int delay;
	char line[512];
	char text[20];
	memset(line, 0, 512);
	fin.ignore(100, '\n');

	while(!fin.eof()) {
		fin.getline(line, 512);
		if(fin.eof())
			break;
		for(char* c = line; *c != '\0'; c++)
			*c = *c == '\x001f' ? '\0' : *c;
		hex = 0;
		label[0] = '\0';
		disasm[0] = '\0';
		delay = 0;
		char* cl = line;
		cl++;
		if(strlen(cl) != 0)
			sscanf(cl, "%x", &addr);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			sscanf(cl, "%d", &hexSize);
		cl += strlen(cl) + 2;

		unsigned int hexArrSize = (hexSize > 1024 ? 1024 : (hexSize == 0 ? 1 : (hexSize * 2 + 1)) * sizeof(char));
		hex = new char[hexArrSize];

		if(strlen(cl) != 0) {
			strncpy(hex, cl, hexArrSize);
		} else {
			hex[0] = '\0';
		}
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			strncpy(label, cl, 128);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			strncpy(disasm, cl, 256);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			sscanf(cl, "%d", &delay);

		if(strlen(hex) % 2 != 0 || strlen(hex) / 2 != hexSize) { //odd number of hex digits, not bytes
			delete hex;
			mLoaded = false;
			return VM_ERR_CORRUPT;
		}

		memset(text, 0, 20);
		for(int j = 0; j < strlen(hex); j++) {
			if(hex[j] >= '0' && hex[j] <= '9') {
				text[j/2] |= (hex[j] - '0') << (j % 2 == 0 ? 4 : 0);
			}else if(toupper(hex[j])  >= 'A' && toupper(hex[j]) <= 'F') {
				text[j/2] |= (hex[j] - 'A' + 10) << (j % 2 == 0 ? 4 : 0);
			}
		}

		delete hex;

		Prefix* pre = Prefix::GetPrefix((unsigned char*)text);
		char* opLoc = text;
		if(pre) {
			opLoc += pre->GetLength();
		}

		std::string s;
		std::string dis(disasm);

		dis.erase(dis.find_last_not_of(" \f\n\r\t\v") + 1);
		dis.erase(0, dis.find_first_not_of(" \f\n\r\t\v"));

		s.insert(0, text, hexSize);
		if(dis.size() == 0)
			continue;

		mInstructions.push_back(new VirgoInstruction(pre, dis, s, (int)*opLoc));

		memcpy(mMem + (addr % MEM_SIZE), text, hexSize);

		if(++i >= numLines)
			break;
	}

	mLoaded = true;
	mVirgo = true;

	return VM_SUCCESS;
}

void VM::Disassemble() {
	if(mLoaded) {
		if(!mVirgo) {
			unsigned int tmpIP = 0;
			Instruction* tmpInst = 0;
			while((tmpInst = Instruction::ReadInstruction(mMem + tmpIP, &mProc )) != 0) {
				mInstructions.push_back(tmpInst);
				tmpIP += tmpInst->GetLength() % MEM_SIZE;
			}
		}
	}
}

std::string VM::GetInstructionStr(unsigned int index) {
	if(mLoaded && index < mInstructions.size()) {
		return mInstructions[index]->GetDisasm();
	}
	return "";
}



int VM::Run() {

	int err = 0;
	for(EVER) {
		if(!mRunning)
			break;

		//This is where to change the base execution address.
		if(err = mProc.Step() < 0) {
			//Hit an error, quit
			break;
		}

	}
	return err;
}

int VM::Step() {
	int err = 0;
	if((err = mProc.Step()) < 0) {
		std::cout << "Encountered an error (#" << err << "), quitting" << std::endl;
	}
	return err;
}

const unsigned char VM::GetMemory(unsigned int addr) const {
	if(addr < MEM_SIZE) {
		return mMem[addr];
	}
	return 0xFF;
}
