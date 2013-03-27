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
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <cctype>
#include "VirgoInstruction.hpp"

#define EVER ;;

const char* const VM::VM_ERR_STRINGS[] = { "Success", "Failed to open file", "Failed to read file", "File exceeded maximum size limits", "File was corrupt", "File contained an instruction longer than 512 bytes" };

void mem_rlog(size_t offset, size_t size) {
	std::ofstream fout("mem.log", std::ios::app);
	fout << size << " byte(s) of memory at 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << offset << " read" << std::endl;
	fout.close();
}

void mem_wlog(size_t offset, size_t size) {
	std::ofstream fout("mem.log", std::ios::app);
	fout << size << " byte(s) of memory at 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << offset << " written" << std::endl;
	fout.close();
}

VM::VM() : mLoaded(false), mRunning(false), mVirgo(false), mMem(MEM_SIZE), mProc(mMem) {

	Instruction::InitializeOpcodes();
	mMem.RegisterReadCallback(mem_rlog);
	mMem.RegisterWriteCallback(mem_wlog);
}

// This function will load a pure object file. No PE style header, just straight machine code
// The object file will be copied into memory at address 0x0000
// Execution will begin at address 0x0000 by default

int VM::LoadFlatFile(const char* filename) {

	//reset everything
	mLoaded = false;
	mProc.Initialize();
	mInstructions.clear();
	mMem.Clear();

	std::ifstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);
	if(fin.fail()) {
		return VM_ERR_FOPEN;
	}

	unsigned int i = 0;
	while(!fin.eof()) {
		if(((i + 1) * 1024) >= MEM_SIZE)
			return VM_ERR_BIG_FILE;

		fin.read((char*)(mMem.getPtr() + (i++) * 1024), 1024);
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
	mInstructions.clear();
	mMem.Clear();

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
	unsigned int hexSize;
	char* hex;
	char label[128];
	char disasm[256];
	unsigned int addr;
	unsigned int delay;
	char line[512];
	char text[TEXT_LEN];
	memset(line, 0, 512);
	fin.ignore(100, '\n');

	while(!fin.eof()) {
		fin.getline(line, 512);
		if(fin.eof())
			break;
		//swap all "quotation marks" with nulls
		for(char* c = line; *c != '\0'; c++)
			*c = *c == '\x001f' ? '\0' : *c;
		hex = 0;
		label[0] = '\0';
		disasm[0] = '\0';
		delay = 0;
		char* cl = line;
		cl++;

		//Parse Address
		if(strlen(cl) != 0)
			sscanf(cl, "%x", &addr);

		//Skip to next field
		cl += strlen(cl) + 2;

		//Number of bytes in hex representation
		if(strlen(cl) != 0)
			sscanf(cl, "%d", &hexSize);
		//Next Field
		cl += strlen(cl) + 2;

		//Create a buffer for the hex
		unsigned int hexArrSize = (hexSize > 1024 ? 1024 : (hexSize == 0 ? 1 : (hexSize * 2 + 1)) * sizeof(char));
		hex = new char[hexArrSize];

		//copy hex into buffer
		if(strlen(cl) != 0) {
			strncpy(hex, cl, hexArrSize);
		} else {
			hex[0] = '\0';
		}
		//Next Field
		cl += strlen(cl) + 2;

		//Copy label if it exists
		if(strlen(cl) != 0)
			strncpy(label, cl, 128);
		//next Field
		cl += strlen(cl) + 2;

		//copy disassembled version
		if(strlen(cl) != 0)
			strncpy(disasm, cl, 256);

		//next Field
		cl += strlen(cl) + 2;

		//copy delay (20)
		if(strlen(cl) != 0)
			sscanf(cl, "%d", &delay);

		//Check for invalid hex string
		if(strlen(hex) % 2 != 0 || strlen(hex) / 2 != hexSize) { //odd number of hex digits, not bytes
			delete hex;
			mLoaded = false;
			return VM_ERR_CORRUPT;
		}

		//convert ascii hex into real hex
		memset(text, 0, TEXT_LEN);
		for(size_t j = 0; j < strlen(hex); j++) {
			if(j == TEXT_LEN) {
				delete hex;
				mLoaded = false;
				return VM_ERR_OVERFLOW;
			}
			if(hex[j] >= '0' && hex[j] <= '9') {
				text[j/2] |= (hex[j] - '0') << (j % 2 == 0 ? 4 : 0);
			}else if(toupper(hex[j])  >= 'A' && toupper(hex[j]) <= 'F') {
				text[j/2] |= (hex[j] - 'A' + 10) << (j % 2 == 0 ? 4 : 0);
			}
		}


		//Try to build a prefix
		Prefix* pre = Prefix::GetPrefix((unsigned char*)text, TEXT_LEN);
		char* opLoc = text;
		if(pre) {
			opLoc += pre->GetLength();
		}

		std::string s;
		std::string dis(disasm);

		//trim whitespace from disassemblu
		dis.erase(dis.find_last_not_of(" \f\n\r\t\v") + 1);
		dis.erase(0, dis.find_first_not_of(" \f\n\r\t\v"));

		//Prefix with label
//		dis = label + ('\t' + dis);

		s.insert(0, hex);

		//clear up buffer from above
		delete hex;

		if((dis.size() == 0 || (hexSize == 0 && label[0] == '\0')) && (strlen(label) == 0))
			continue;

		Instruction* inst = new VirgoInstruction(pre, dis, s, (int)*opLoc);
		inst->SetAddress(addr);

		mInstructions.push_back(inst);
		mLabels.push_back(label);

		memcpy(mMem.getPtr() + (addr % MEM_SIZE), text, hexSize);

		if(++i >= numLines)
			break;
	}

	mLoaded = true;
	mVirgo = true;

	mProc.Initialize(addrStart);

	return VM_SUCCESS;
}

void VM::Disassemble() {
	if(mLoaded) {
		if(!mVirgo) {
			unsigned int tmpIP = 0;
			Instruction* tmpInst = 0;
			Memory::MemoryOffset curMem = mMem.getOffset(tmpIP);
			while((tmpInst = Instruction::ReadInstruction(curMem, &mProc )) != 0) {
				mInstructions.push_back(tmpInst);
				tmpIP += tmpInst->GetLength() % MEM_SIZE;
				curMem = curMem.getNewOffset(tmpIP);
			}
		}
	}
}

std::string VM::GetInstructionStr(unsigned int index) const {
	if(mLoaded && index < mInstructions.size()) {
		return mInstructions[index]->GetDisasm();
	}
	return "";
}

std::string VM::GetInstructionLabel(unsigned int index) const {
	if(mLoaded && index < mLabels.size()) {
		return mLabels[index];
	}
	return "";
}

unsigned int VM::GetInstructionAddr(unsigned int index) const {
	if(mLoaded && index < mInstructions.size()) {
		return mInstructions[index]->GetAddress();
	}
	return -1;
}

unsigned int VM::GetInstructionLen(unsigned int index) const {
	if(mLoaded && index < mInstructions.size()) {
		return mInstructions[index]->GetLength();
	}
	return 0;
}

std::string VM::GetInstructionText(unsigned int index) const {
	if(mLoaded && index < mInstructions.size()) {
		return mInstructions[index]->GetInstruction();
	}
	return "";
}

unsigned int VM::CalcInstructionLen() {
	if(mLoaded) {
		Memory::MemoryOffset curMem = mMem.getOffset(mProc.GetRegister(REG_IP));
		Instruction* inst = Instruction::ReadInstruction(curMem, &mProc);
		if(inst != NULL) {
			unsigned int len = inst->GetLength();
			delete inst;
			return len;
		}
		return 0;
	}
	return 0;
}


int VM::Run() {

	int err = 0;
	for(EVER) {
		if(!mRunning)
			break;

		//This is where to change the base execution address.
		if((err = mProc.Step()) < 0) {
			//Hit an error, quit
			break;
		}

	}
	mMem.notifyReadCallbacks();
	mMem.notifyWriteCallbacks();
	return err;
}

int VM::Step() {
	int err = 0;
	if((err = mProc.Step()) < 0) {
		std::cout << "Encountered an error (#" << err << "), quitting" << std::endl;
	} else {
		for(size_t i = 0; i < mBreakpoints.size(); i++) {
			if(mBreakpoints[i]->Evaluate(&mProc))
				return VM_BREAKPOINT;
		}
	}
	return err;
}

unsigned char VM::GetMemory(unsigned int addr) {
	if(addr < MEM_SIZE) {
		return mMem[addr];
	}
	return 0xFF;
}

void VM::AddBreakpoint(Breakpoint* bp) {
	if(bp == 0)
		return;

	for(size_t i = 0; i < mBreakpoints.size(); i++) {
		if(mBreakpoints[i]->GetIP() == bp->GetIP()) {
			delete mBreakpoints[i];
			mBreakpoints[i] = bp;
			return;
		}
	}
	mBreakpoints.push_back(bp);
}

void VM::RemoveBreakpoint(unsigned int addr) {
	for(size_t i = 0; i < mBreakpoints.size(); i++) {
		if(mBreakpoints[i]->GetIP() == addr) {
			mBreakpoints.erase(mBreakpoints.begin() + i);
			break;
		}
	}
}

Breakpoint* VM::FindBreakpoint(unsigned int addr) {
	for(size_t i = 0; i < mBreakpoints.size(); i++) {
		if(mBreakpoints[i]->GetIP() == addr) {
			return mBreakpoints[i];
		}
	}
	return 0;
}

const char* const VM::GetErrStr(unsigned int err) const {
	if(err < sizeof(VM::VM_ERR_STRINGS) / sizeof(const char* const)) {
		return VM_ERR_STRINGS[err];
	}
	return 0;
}
