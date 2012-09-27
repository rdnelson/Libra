/*-------------------------------------*\
|
|  File Name: VM.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Tue, Sep 25, 2012  5:14:46 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>

#include "Processor.hpp"
#include "Peripheral.hpp"

class VM {

	public:
		VM(int argc, char* argv[]);
		
		int Run();

		int LoadObjectFile(char* filename);
		int LoadVirgoFile(char* filename);

		const static int MEM_SIZE = 0x10000;

		const static int VM_SUCCESS	= 0x00;
		const static int VM_ERR_FOPEN	= 0x01;
		const static int VM_ERR_FREAD	= 0x02;

	private:

		

		Processor	mProc;
		char		mMem[MEM_SIZE];
		std::vector<Peripheral*>	mDevs;
		
		
};
