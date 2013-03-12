/*-------------------------------------*\
|
|  File Name: Processor.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct 18, 2012  7:54:57 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>

#include "Instruction.hpp"
#include "Memory.hpp"
#include "Register.hpp"
#include "IPeripheral.hpp"

class QTimer;

enum eRegisters {
	REG_AX,
	REG_CX,
	REG_DX,
	REG_BX,
	REG_SP,
	REG_BP,
	REG_SI,
	REG_DI,
	REG_CS,
	REG_DS,
	REG_SS,
	REG_ES,
	REG_IP,
	REG_FLAGS,
	NumRegisters,
	REG_AL,
	REG_CL,
	REG_DL,
	REG_BL,
	LowRegisters,
	REG_AH,
	REG_CH,
	REG_DH,
	REG_BH,
	HighRegisters,
    AllRegisters=HighRegisters
};

enum eFlags {
	FLAGS_CF = 0,
	FLAGS_PF = 2,
	FLAGS_AF = 4,
	FLAGS_ZF = 6,
	FLAGS_SF = 7,
	FLAGS_TF = 8,
	FLAGS_IF = 9,
	FLAGS_DF = 10,
	FLAGS_OF = 11
};


class Processor {

	public:
		int Initialize(unsigned int startAddr = 0x0000);
		Processor(Memory& mem);
		~Processor();
		int Step();
		void Stop();

		static const int PROC_SUCCESS		=  0;
		static const int PROC_HALT		=  1;
		static const int PROC_PERIPH_WRITE	=  2;
		static const int PROC_ERR_INV_ADDR 	= -1;
		static const int PROC_ERR_INV_INST 	= -2;
		static const int PROC_ERR_INST		= -3;

		bool GetFlag(eFlags flag) const;
		void SetFlag(eFlags flag, bool val);

		virtual unsigned int GetRegister(eRegisters reg) const;
		virtual void SetRegister(eRegisters reg, unsigned int val);

		unsigned int GetRegisterLow(eRegisters reg) const;
		void SetRegisterLow(eRegisters reg, unsigned int val);

		unsigned int GetRegisterHigh(eRegisters reg) const;
		void SetRegisterHigh(eRegisters reg, unsigned int val);

		virtual unsigned int GetMemory(Memory::MemoryOffset& addr, unsigned int size);
		virtual unsigned int GetMemory(size_t offset, unsigned int size);

		virtual void SetMemory(Memory::MemoryOffset& addr, unsigned int size, unsigned int val);
		virtual void SetMemory(size_t offset, unsigned int size, unsigned int val);

		void SetInterrupt(unsigned char n);
		void Halt() { mHalt = true; }

		void PushRegister(eRegisters reg);
		void PushValue(unsigned int val);

		void PopRegister(eRegisters reg);
		void PopSize(unsigned int size);
		unsigned int PopValue();

		void Outb(unsigned int port, unsigned char data);
		void Outw(unsigned int port, unsigned short data);

		unsigned char Inb(unsigned int port);
		unsigned short Inw(unsigned int port);

		const std::vector<IPeripheral*> & GetDevices() { return mDevices; }
		const char* GetRegisterHex(eRegisters reg) const;

		void SetTimer(QTimer* timer) { mTimer = timer; }

		void ProcDump();
		void MemDump();
		void DeviceDump();



	private:

		int Execute(Instruction* inst);

		void _InitializeDevices();

		Register	mRegisters[NumRegisters];
		Memory&	mMem;

		std::vector<IPeripheral*> mDevices;

		//Last Accessed port# and it's associated device
		unsigned int mLastPort;
		IPeripheral* mLastDevice;

		unsigned int mStartAddr;

		int mInterrupt;
		bool mHalt;
		QTimer* mTimer;

};
