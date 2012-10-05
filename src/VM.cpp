/*-------------------------------------*\
|
|  File Name: VM.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct  4, 2012  9:14:03 PM
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

VM::VM(int argc, char* argv[]) : mProc(mMem) {

	//initialize main memory
	memset(mMem, 0xFF, MEM_SIZE);

	LoadObjectFile(argv[1]);
}


// This function will load a pure object file. No PE style header, just straight machine code
// The object file will be copied into memory at address 0x0000
// Execution will begin at address 0x0000 by default

int VM::LoadObjectFile(char* filename) {

	std::ifstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);
	if(fin.fail()) { 
		return VM_ERR_FOPEN;
	}

	int i = 0;
	while(!fin.eof()) {
		fin.read((char*)(mMem + (i++) * 1024), 1024);
		if(fin.bad()) { //an error occurred
			fin.close();
			return VM_ERR_FREAD;
		}
	};

	return VM_SUCCESS;

}



int VM::Run() {

	mProc.Initialize();
	Instruction::InitializeOpcodes();
	
	for(EVER) {

		//This is where to change the base execution address.
		mProc.ProcDump();
		mProc.MemDump();
		if(mProc.Step()) {
			//Hit an error, quit
			
			std::cout << "Encountered an error, quitting" << std::endl;
			break;
		}
		getchar();
	}

	return 0;
}
