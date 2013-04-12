/*-------------------------------------*\
|
|  File Name: Processor8086.hpp
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

#include "Processor.hpp"
//#include "Instruction.hpp"
#include "Memory.hpp"
#include "Register.hpp"
#include "IPeripheral.hpp"

class QTimer;




class Processor8086 : public Processor {
	public:
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


		int Initialize(unsigned int startAddr = 0x0000);
		Processor8086(Memory& mem);
		~Processor8086();
		int Step();
		void Stop();

		static const int PROC_SUCCESS		=  0;
		static const int PROC_HALT		=  1000;
		static const int PROC_ERR_INV_ADDR 	= -1;
		static const int PROC_ERR_INV_INST 	= -2;
		static const int PROC_ERR_INST		= -3;

		bool GetFlag(unsigned int flag) const;
		void SetFlag(unsigned int flag, bool val);

		unsigned int GetRegister(unsigned int reg) const;
		void SetRegister(unsigned int reg, unsigned int val);

		unsigned int GetRegisterLow(unsigned int reg) const;
		void SetRegisterLow(unsigned int reg, unsigned int val);

		unsigned int GetRegisterHigh(unsigned int reg) const;
		void SetRegisterHigh(unsigned int reg, unsigned int val);

		unsigned int GetMemory(Memory::MemoryOffset& addr, unsigned int size);
		unsigned int GetMemory(size_t offset, unsigned int size);

		void SetMemory(Memory::MemoryOffset& addr, unsigned int size, unsigned int val);
		void SetMemory(size_t offset, unsigned int size, unsigned int val);

		unsigned int GetIP() const { return GetRegister(REG_IP); }
		bool InterruptsEnabled() const { return GetFlag(FLAGS_IF); }

		void SetInterrupt(unsigned char n);
		void Halt() { mHalt = true; }

		void PushRegister(unsigned int reg);
		void PushValue(unsigned int val);

		void PopRegister(unsigned int reg);
		void PopSize(unsigned int size);
		unsigned int PopValue();

		void Outb(unsigned int port, unsigned char data);
		void Outw(unsigned int port, unsigned short data);

		unsigned char Inb(unsigned int port);
		unsigned short Inw(unsigned int port);

		const std::vector<IPeripheral*> & GetDevices() { return mDevices; }
		const char* GetRegisterHex(unsigned int reg) const;
		const Instruction* GetNextInstruction() const { return mNextInst; }

		void SetTimer(QTimer* timer) { mTimer = timer; }

		void ProcDump();
		void MemDump();
		void DeviceDump();



	private:

		int Execute(Instruction* inst);

		void _InitializeDevices();

		Register	mRegisters[NumRegisters];

		//Last Accessed port# and it's associated device
		unsigned int mLastPort;
		IPeripheral* mLastDevice;
};
