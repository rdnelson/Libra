/*-------------------------------------*\
|
|  File Name: VM.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Wed, Sep 26, 2012 10:48:49 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "VM.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

#define EVER ;;

VM::VM(int argc, char* argv[]) : mProc(mMem) {

	for(int i = 1; i < argc; i++) {
		std::cout << argv[i];
	}
		
	//initialize main memory
	memset(mMem, 0xFF, MEM_SIZE);
}


// This function will load a pure object file. No PE style header, just straight machine code
// The object file will be copied into memory at address 0x0000
// Execution will begin at address 0x0000

int VM::LoadObjectFile(char* filename) {

	std::ifstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);
	if(fin.fail()) { 
		return VM_ERR_FOPEN;
	}

	int i = 0;
	while(!fin.eof()) {
		fin.read(mMem + (i++) * 1024, 1024);
		if(fin.bad()) { //an error occurred
			fin.close();
			return VM_ERR_FREAD;
		}
	};

	return VM_SUCCESS;

}



int VM::Run() {

	mProc.Initialize();
	
	for(EVER) {

		//This is where to change the base execution address.
		mProc.Step();
	}

	return 0;
}
