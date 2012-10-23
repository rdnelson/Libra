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

#define EVER ;;

VM::VM() : mProc(mMem), mVirgo(false) {

	//initialize main memory
	memset(mMem, 0xFF, MEM_SIZE);
	Instruction::InitializeOpcodes();

	LoadVirgoFile(argv[1]);

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

	mLoaded - false;
	mProc.Initialize();

	std::ifstream fin;
	fin.open(filename, std::ios_base::in);
	if(fin.fail()) {
		return VM_ERR_FOPEN;
	}

	//parse header
	int numInitArgs, numLines, addrStart, bytesTotal;
	char delim;
	fin >> numInitArgs;
	fin >> numLines;
	fin >> addrStart;
	fin >> bytesTotal;

	int i = 0;
	int hexSize;
	char hex[32];
	char label[128];
	char disasm[256];
	int addr;
	int delay;
	char line[512];
	memset(line, 0, 512);
	fin.ignore(100, '\n');

	while(!fin.eof()) {
		fin.getline(line, 512);
		for(char* c = line; *c != '\0'; c++)
			*c = *c == '\x001f' ? '\0' : *c;
		hex[0] = '\0';
		label[0] = '\0';
		disasm[0] = '\0';
		delay = 0;
		char* cl = line;
		cl++;
		if(strlen(cl) != 0)
			sscanf(cl, "%d", &addr);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			sscanf(cl, "%d", &hexSize);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			strncpy(hex, cl, 32);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			strncpy(label, cl, 128);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			strncpy(disasm, cl, 256);
		cl += strlen(cl) + 2;

		if(strlen(cl) != 0)
			sscanf(cl, "%d", &delay);
		

		//sscanf(line, "%d\x001f\x001f%d\x001f\x001f%[^\x001f]\x001f\x001f%[^\x001f]\x001f\x001f%[^\x001f]\x001f\x001f%d\x001f", &addr, &hexSize, hex, label, disasm, &delay);
		//sscanf(line, "%d%*[\n]%d%*[\n]%s%*[\n]%s%*[\n]%[^\n]%*[\n]%d", &addr, &hexSize, hex, label, disasm, &delay);


		if(++i >= numLines)
			break;

	}
}

void VM::Disassemble() {
    if(mLoaded) {
        if(!mVirgo) {
            unsigned int tmpIP = 0;
            Instruction* tmpInst = 0;
            while(tmpInst = Instruction::ReadInstruction(mMem + tmpIP, &mProc )) {
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
