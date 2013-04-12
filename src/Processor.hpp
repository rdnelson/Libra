/*-------------------------------------*\
|
|  File Name: Processor.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Fri, Apr 12, 2012  13:41:00 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>

//#include "Instruction.hpp"
#include "Memory.hpp"
#include "Register.hpp"
#include "IPeripheral.hpp"

class QTimer;
class Instruction;



class Processor {
	
	public:
		enum eModel {
			MODEL_INVALID,
			MODEL_8086,
			NUM_MODELS
		};
		Processor(Memory& mem) : mMem(mem), mNextInst(0), mStartAddr(0), mModel(MODEL_INVALID), mInterrupt(0), mHalt(false) {}
		eModel GetModel() const { return mModel; }
		virtual int Initialize(unsigned int startAddr) = 0;
		virtual int Step() = 0;
		virtual void Stop() = 0;

		static const int PROC_SUCCESS		=  0;
		static const int PROC_HALT		=  1000;
		static const int PROC_ERR_INV_ADDR 	= -1;
		static const int PROC_ERR_INV_INST 	= -2;
		static const int PROC_ERR_INST		= -3;

		virtual bool GetFlag(unsigned int flag) const = 0;
		virtual void SetFlag(unsigned int flag, bool val) = 0;

		virtual unsigned int GetRegister(unsigned int reg) const = 0;
		virtual void SetRegister(unsigned int reg, unsigned int val) = 0;

		virtual unsigned int GetRegisterLow(unsigned int reg) const = 0;
		virtual void SetRegisterLow(unsigned int reg, unsigned int val) = 0;

		virtual unsigned int GetRegisterHigh(unsigned int reg) const = 0;
		virtual void SetRegisterHigh(unsigned int reg, unsigned int val) = 0;

		virtual unsigned int GetMemory(Memory::MemoryOffset& addr, unsigned int size) = 0;
		virtual unsigned int GetMemory(size_t offset, unsigned int size) = 0;

		virtual void SetMemory(Memory::MemoryOffset& addr, unsigned int size, unsigned int val) = 0;
		virtual void SetMemory(size_t offset, unsigned int size, unsigned int val) = 0;

		virtual unsigned int GetIP() const = 0;
		virtual bool InterruptsEnabled() const = 0;

		virtual void SetInterrupt(unsigned char n) = 0;
		virtual void Halt() = 0;

		virtual void PushRegister(unsigned int reg) = 0;
		virtual void PushValue(unsigned int val) = 0;

		virtual void PopRegister(unsigned int reg) = 0;
		virtual void PopSize(unsigned int size) = 0;
		virtual unsigned int PopValue() = 0;

		virtual void Outb(unsigned int port, unsigned char data) = 0;
		virtual void Outw(unsigned int port, unsigned short data) = 0;

		virtual unsigned char Inb(unsigned int port) = 0;
		virtual unsigned short Inw(unsigned int port) = 0;

		const std::vector<IPeripheral*> & GetDevices() { return mDevices; }
		virtual const char* GetRegisterHex(unsigned int reg) const = 0;
		const Instruction* GetNextInstruction() const { return mNextInst; }

		void SetTimer(QTimer* timer) { mTimer = timer; }

		virtual void ProcDump() = 0;
		virtual void MemDump() = 0;
		virtual void DeviceDump() = 0;

	protected:
		Memory&	mMem;
		Instruction* mNextInst;

		std::vector<IPeripheral*> mDevices;

		unsigned int mStartAddr;

		eModel mModel;

		int mInterrupt;
		bool mHalt;
		QTimer* mTimer;
};
