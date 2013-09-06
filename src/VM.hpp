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
#include <fstream>

#include "Memory.hpp"
#include "Processor.hpp"
#include "IPeripheral.hpp"
#include "Breakpoint.hpp"

class Instruction;
class QTimer;

class VM {

	public:
		VM();

		void EnableMemoryLogging();
		void DisableMemoryLogging();

		int Run();
		int Step();
		void Stop() { mProc.Stop(); }

		int LoadFlatFile(const char* filename);
		int LoadVirgoFile(const char* filename);

		inline bool isLoaded() { return mLoaded; }
		void Disassemble();
		std::string GetInstructionStr(unsigned int index) const;
		std::string GetInstructionLabel(unsigned int index) const;
		std::string GetInstructionText(unsigned int index) const;
		unsigned int GetInstructionAddr(unsigned int index) const;
		unsigned int GetInstructionLen(unsigned int index) const;
		unsigned int CalcInstructionLen();
		size_t GetNumInstructions() { return mInstructions.size(); }
		const std::vector<IPeripheral*> & GetDevices() { return mProc.GetDevices(); }
		const Processor & GetProc() { return mProc; }
		unsigned char GetMemory(unsigned int addr);
		const unsigned char* GetMemPtr() const { return mMem.getPtr(); }

		void AddBreakpoint(Breakpoint* bp);
		void RemoveBreakpoint(unsigned int addr);
		Breakpoint* FindBreakpoint(unsigned int addr);
		const std::vector<Breakpoint*> & GetBreakpoints() { return mBreakpoints; }

		inline void notifyReadCallbacks() { mMem.notifyReadCallbacks(&mMemLog); }
		inline void notifyWriteCallbacks() { mMem.notifyWriteCallbacks(&mMemLog); }

		inline void notifyReadCallbacks(void* arg) { mMem.notifyReadCallbacks(arg); }
		inline void notifyWriteCallbacks(void* arg) { mMem.notifyWriteCallbacks(arg); }

		inline unsigned int GetCallDepth() const { return mInFunc; }

		void SetTimer(QTimer* timer) { mProc.SetTimer(timer); }

		const static unsigned int MEM_SIZE = 0x10000;
		const static unsigned int TEXT_LEN = 0x400;

		const static int VM_SUCCESS	= 0x00;
		const static int VM_ERR_FOPEN	= 0x01;
		const static int VM_ERR_FREAD	= 0x02;
		const static int VM_ERR_BIG_FILE = 0x03;
		const static int VM_ERR_CORRUPT = 0x04;
		const static int VM_ERR_OVERFLOW = 0x05;

		const static char* const VM_ERR_STRINGS[];

		const char* const GetErrStr (unsigned int err) const;

		const static int VM_BREAKPOINT = 0x03;

	private:

		bool mLoaded;
		bool mRunning;
		bool mVirgo;
		unsigned int mInFunc;

		Memory	mMem;
		Processor	mProc;

		std::vector<Instruction*> mInstructions;
		std::vector<Breakpoint*> mBreakpoints;
		std::vector<std::string> mLabels;

		std::ofstream mMemLog;

};
