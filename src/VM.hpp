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
#include "Breakpoint.hpp"

class Instruction;

class VM {

	public:
		VM();
		
		int Run();
		int Step();

		int LoadFlatFile(const char* filename);
		int LoadVirgoFile(const char* filename);

		inline bool isLoaded() { return mLoaded; }
		void Disassemble();
		std::string GetInstructionStr(unsigned int index) const;
		const unsigned int GetInstructionAddr(unsigned int index) const;
		const unsigned int GetNumInstructions() { return mInstructions.size(); }
		const std::vector<IPeripheral*> & GetDevices() { return mProc.GetDevices(); }
		const Processor & GetProc() { return mProc; }
		const unsigned char GetMemory(unsigned int addr) const;
		const unsigned char* GetMemPtr() const { return mMem; }

		inline void AddBreakpoint(Breakpoint* bp) { mBreakpoints.push_back(bp); }

		const static int MEM_SIZE = 0x10000;

		const static int VM_SUCCESS	= 0x00;
		const static int VM_ERR_FOPEN	= 0x01;
		const static int VM_ERR_FREAD	= 0x02;
		const static int VM_ERR_BIG_FILE = 0x03;
		const static int VM_ERR_CORRUPT = 0x04;

		const static int VM_BREAKPOINT = 0x03;

	private:

		bool mLoaded;
		bool mRunning;
		bool mVirgo;

		Processor	mProc;
		unsigned char	mMem[MEM_SIZE];
		std::vector<Instruction*> mInstructions;
		std::vector<Breakpoint*> mBreakpoints;
		
};
