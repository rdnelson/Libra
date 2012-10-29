/*-------------------------------------*\
|
|  File Name: VM.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Sep 27, 2012  2:06:12 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>

#include "Processor.hpp"
#include "IPeripheral.hpp"

class VM {

	public:
		VM(int argc, char* argv[]);
		
		int Run();

		int LoadFlatFile(const char* filename);
		int LoadVirgoFile(const char* filename);

		void Disassemble();

		const static int MEM_SIZE = 0x10000;

		const static int VM_SUCCESS	= 0x00;
		const static int VM_ERR_FOPEN	= 0x01;
		const static int VM_ERR_FREAD	= 0x02;
		const static int VM_ERR_BIG_FILE = 0x03;
		const static int VM_ERR_CORRUPT = 0x04;

	private:

		bool mLoaded;
		bool mRunning;
		bool mVirgo;

		Processor	mProc;
		unsigned char	mMem[MEM_SIZE];
		std::vector<Instruction*> mInstructions;
		
		
};
