/*-------------------------------------*\
|
|  File Name: Processor.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:57:12 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Processor.hpp"
#include "peripherals/AllPeripherals.hpp"
#include "IPeripheral.hpp"

#include <iostream>
#include <cstring>
#include <cstdio>

Processor::Processor(Memory& mem): mMem(mem), mNextInst(0), mLastPort(0xFFFFFFFF), mLastDevice(0), mInterrupt(-1), mHalt(false) {

}

Processor::~Processor() {
	for(size_t i = 0; i < mDevices.size(); i++) {
		delete mDevices[i];
	}
}

//Allows the starting address to be changed
int Processor::Initialize(unsigned int startAddr) {


	if(startAddr >= 0x10000) {
		return PROC_ERR_INV_ADDR;
	}

	mStartAddr = startAddr;

	SetRegister(REG_AX, 0x0000);
	SetRegister(REG_BX, 0x0000);
	SetRegister(REG_CX, 0x0000);
	SetRegister(REG_DX, 0x0000);
	SetRegister(REG_SP, 0x0000);
	SetRegister(REG_BP, 0x0000);
	SetRegister(REG_SI, 0x0000);
	SetRegister(REG_DI, 0x0000);
	SetRegister(REG_FLAGS, 0x0000);
	SetRegister(REG_IP, startAddr);
	mHalt = false;

	mInterrupt = -1;

	_InitializeDevices();

	//Fetch first instruction
	Memory::MemoryOffset curMem = mMem.getOffset(GetRegister(REG_IP));
	mNextInst = Instruction::ReadInstruction(curMem, this);

	return PROC_SUCCESS;

}

void Processor::Stop() {
	Initialize(mStartAddr);
}

void Processor::_InitializeDevices() {

	for(size_t i = 0; i < mDevices.size(); i++) {
		delete mDevices[i];
	}
	mDevices.clear();
	mLastPort = 0xFFFFFFFF;
	mLastDevice = 0;

	mDevices.push_back(new Screen());
	mDevices.push_back(new Keyboard(const_cast<Processor*>(this)));
	mDevices.push_back(new Timer(const_cast<Processor*>(this)));
}

//Execute a single instruction
int Processor::Step() {
	int retVal = PROC_SUCCESS;

	for(size_t i = 0; i < mDevices.size(); i++) {
		mDevices[i]->Update();
	}

	//Check for interrupts
	if(GetFlag(FLAGS_IF) && mInterrupt >= 0 && mInterrupt <= 255) {
		PushRegister(REG_FLAGS);
		SetFlag(FLAGS_IF, false);
		SetFlag(FLAGS_TF, false);
		PushRegister(REG_IP);
		SetRegister(REG_IP, GetMemory(mInterrupt << 2, 2));
		Memory::MemoryOffset curMem = mMem.getOffset(GetRegister(REG_IP));
		mNextInst = Instruction::ReadInstruction(curMem, this);
		mInterrupt = -1;
		mHalt = false;
		return PROC_SUCCESS;
	}

	if(mHalt == true) {
		return PROC_HALT;
	}


	//Ensure it exists and is valid
	if(mNextInst && mNextInst->IsValid()) {
		//Increment IP
		SetRegister(REG_IP, GetRegister(REG_IP) + mNextInst->GetLength());

#ifdef DEBUG
		mNextInst->AddLengthToDisasm();

		std::cout << mNextInst->GetDisasm() << std::endl;

#endif
		//Execute
		if((retVal = mNextInst->Execute(this)) < 0) {
			delete mNextInst;
			return PROC_ERR_INST;
		}
		delete mNextInst;

		Memory::MemoryOffset curMem = mMem.getOffset(GetRegister(REG_IP));
		mNextInst = Instruction::ReadInstruction(curMem, this);

	} else {
		return PROC_ERR_INV_INST;
	}

    return retVal;

}

bool Processor::GetFlag(eFlags flag) const {
	return (GetRegister(REG_FLAGS) & (1 << flag)) != 0;
}

void Processor::SetFlag(eFlags flag, bool val) {
	if(val)
		SetRegister(REG_FLAGS, GetRegister(REG_FLAGS) | (1 << flag));
	else
		SetRegister(REG_FLAGS, GetRegister(REG_FLAGS) & ~(1 << flag));
}


unsigned int Processor::GetRegister(eRegisters reg) const {
	if(reg == NumRegisters || reg == HighRegisters || reg == LowRegisters) {
		return 0;
	}

	if(reg < NumRegisters) {
		return mRegisters[reg].GetValue();
	}
	else if(reg < LowRegisters) {
		return GetRegisterLow((eRegisters)(reg - NumRegisters - 1));
	}
	else if(reg < HighRegisters) {
		return GetRegisterHigh((eRegisters)(reg - LowRegisters - 1));
	}
	return 0;
}

const char* Processor::GetRegisterHex(eRegisters reg) const {
    if(reg == NumRegisters || reg == HighRegisters || reg == LowRegisters) {
        return 0;
    }
    if(reg < NumRegisters) {
        return mRegisters[reg].GetHex();
    }
    return 0;
}

void Processor::SetRegister(eRegisters reg, unsigned int val) {
	if(reg == NumRegisters) {
		return;
	}

	if(reg < NumRegisters) {
		mRegisters[reg].SetValue(val);
	} else if(reg < LowRegisters) {
		SetRegisterLow((eRegisters)(reg - NumRegisters - 1), val);
	} else if(reg < HighRegisters) {
		SetRegisterHigh((eRegisters)(reg - LowRegisters - 1), val);
	}

}

unsigned int Processor::GetRegisterLow(eRegisters reg) const {
	if(reg == NumRegisters) {
		return 0;
	}

	return mRegisters[reg].GetValue() & 0xFF;
}

unsigned int Processor::GetRegisterHigh(eRegisters reg) const {
	if(reg == NumRegisters) {
		return 0;
	}

	return (mRegisters[reg].GetValue() & 0xFF00) >> 8;
};

void Processor::SetRegisterLow(eRegisters reg, unsigned int val) {
	if(reg == NumRegisters)
		return;

	mRegisters[reg].SetValue((mRegisters[reg].GetValue() & 0xFF00) | (val & 0xFF));
}

void Processor::SetRegisterHigh(eRegisters reg, unsigned int val) {
	if(reg == NumRegisters)
		return;

	mRegisters[reg].SetValue((mRegisters[reg].GetValue() & 0xFF) | ((val & 0xFF) << 8));
}

unsigned int Processor::GetMemory(Memory::MemoryOffset& addr, unsigned int size) {
	unsigned int temp = 0;
	addr.read(&temp, size);
	return temp;
}

unsigned int Processor::GetMemory(size_t addr, unsigned int size) {
	Memory::MemoryOffset tmpMem = mMem.getOffset(addr);
	return GetMemory(tmpMem, size);
}


void Processor::SetMemory(Memory::MemoryOffset& addr, unsigned int size, unsigned int val) {
	addr.write(&val, size);
}

void Processor::SetMemory(size_t addr, unsigned int size, unsigned int val) {
	Memory::MemoryOffset tmpMem = mMem.getOffset(addr);
	SetMemory(tmpMem, size, val);
}

void Processor::PushRegister(eRegisters reg) {
	SetRegister(REG_SP, (GetRegister(REG_SP) - 2) & 0xFFFF);
	Memory::MemoryOffset tmpMem = mMem.getOffset(GetRegister(REG_SP) + (GetRegister(REG_SS) << 4));
	SetMemory(tmpMem, 2, GetRegister(reg));
}

void Processor::PushValue(unsigned int val) {
	SetRegister(REG_SP, (GetRegister(REG_SP) - 2) & 0xFFFF);
	Memory::MemoryOffset tmpMem = mMem.getOffset(GetRegister(REG_SP) + (GetRegister(REG_SS) << 4));
	SetMemory(tmpMem, 2, val & 0xFFFF);
}

void Processor::PopRegister(eRegisters reg) {
	Memory::MemoryOffset tmpMem = mMem.getOffset(GetRegister(REG_SP) + (GetRegister(REG_SS) << 4));
	SetRegister(reg, GetMemory(tmpMem, 2));
	SetRegister(REG_SP, (GetRegister(REG_SP) + 2) & 0xFFFF);
}

void Processor::PopSize(unsigned int size) {
	SetRegister(REG_SP, (GetRegister(REG_SP) + size) & 0xFFFF);
}

unsigned int Processor::PopValue() {
	Memory::MemoryOffset tmpMem = mMem.getOffset(GetRegister(REG_SP) + (GetRegister(REG_SS) << 4));
	unsigned int val = GetMemory(tmpMem, 2) & 0xFFFF;
	SetRegister(REG_SP, (GetRegister(REG_SP) + 2) & 0xFFFF);
	return val;
}

void Processor::Outb(unsigned int port, unsigned char data) {
	if(mLastPort == port && mLastDevice) {
		mLastDevice->Put8(port, data);
		return;
	}
	for(unsigned int i = 0; i < mDevices.size(); i++) {
		if(mDevices[i]->Put8(port, data)){
			mLastDevice = mDevices[i];
			mLastPort = port;
			break;
		}
	}
}

void Processor::Outw(unsigned int port, unsigned short data) {
	if(mLastPort == port && mLastDevice) {
		mLastDevice->Put16(port, data);
		return;
	}
	for(unsigned int i = 0; i < mDevices.size(); i++) {
		if(mDevices[i]->Put16(port, data)){
			mLastDevice = mDevices[i];
			mLastPort = port;
			return;
		}
	}
}

unsigned char Processor::Inb(unsigned int port) {
	if(mLastPort == port && mLastDevice) {
		return mLastDevice->Get8(port);
	}
	unsigned int tmpRet = 0;
	for(unsigned int i = 0; i < mDevices.size(); i++) {
		if((tmpRet = mDevices[i]->Get8(port)) != 0xFFFFFFFF) {
			mLastPort = port;
			mLastDevice = mDevices[i];
			return tmpRet;
		}
	}
	return 0x00;
}

unsigned short Processor::Inw(unsigned int port) {
	if(mLastPort == port && mLastDevice) {
		return mLastDevice->Get16(port);
	}
	unsigned int tmpRet = 0;
	for(unsigned int i = 0; i < mDevices.size(); i++) {
		if((tmpRet = mDevices[i]->Get16(port)) != 0xFFFFFFFF) {
			mLastPort = port;
			mLastDevice = mDevices[i];
			return tmpRet;
		}
	}
	return 0x00;
}
void Processor::ProcDump() {

	std::cout << "++++++++++++++++++++++++ BEGIN DUMP ++++++++++++++++++++++++" << std::endl; 

	std::cout << "Registers:" << std::endl;
	std::cout << std::hex;
	printf( "\tAX = 0x%04X" , mRegisters[REG_AX].GetValue());
	printf( "\tBX = 0x%04X\n" , mRegisters[REG_BX].GetValue());
	printf( "\tCX = 0x%04X" , mRegisters[REG_CX].GetValue());
	printf( "\tDX = 0x%04X\n" , mRegisters[REG_DX].GetValue());
	printf( "\tSI = 0x%04X" , mRegisters[REG_SI].GetValue());
	printf( "\tDI = 0x%04X\n" , mRegisters[REG_DI].GetValue());
	printf( "\tBP = 0x%04X" , mRegisters[REG_BP].GetValue());
	printf( "\tSP = 0x%04X\n" , mRegisters[REG_SP].GetValue());

	printf( "\tCS = 0x%04X" , mRegisters[REG_CS].GetValue());
	printf( "\tDS = 0x%04X\n" , mRegisters[REG_DS].GetValue());
	printf( "\tSS = 0x%04X" , mRegisters[REG_SS].GetValue());
	printf( "\tES = 0x%04X\n" , mRegisters[REG_ES].GetValue());
	printf( "\tIP = 0x%04X" , mRegisters[REG_IP].GetValue());
	printf( "\tFLAGS = 0x%04X\n" , mRegisters[REG_FLAGS].GetValue());

	std::cout << std::endl << "Flags:" << std::endl;
	std::cout << "\tCF = " << GetFlag(FLAGS_CF);
	std::cout << "\tPF = " << GetFlag(FLAGS_PF) << std::endl;
	std::cout << "\tAF = " << GetFlag(FLAGS_AF);
	std::cout << "\tZF = " << GetFlag(FLAGS_ZF) << std::endl;
	std::cout << "\tSF = " << GetFlag(FLAGS_SF);
	std::cout << "\tTF = " << GetFlag(FLAGS_TF) << std::endl;
	std::cout << "\tIF = " << GetFlag(FLAGS_IF);
	std::cout << "\tDF = " << GetFlag(FLAGS_DF) << std::endl;
	std::cout << "\tOF = " << GetFlag(FLAGS_OF) << std::endl;

	std::cout << std::dec;
	std::cout << std::endl;


	std::cout << "++++++++++++++++++++++++  END DUMP  ++++++++++++++++++++++++" << std::endl; 

}

void Processor::MemDump() {
	std::cout << "++++++++++++++++++++++ BEGIN MEM DUMP ++++++++++++++++++++++" << std::endl;
	std::cout << std::hex;
	int lastSame = 0;
	for(unsigned int i = 0; i < (0x10000 / 0x10); i++) {
		bool same = true;
		if(i != (mMem.getSize() / 0x10) - 1) {
			for(int j = 0; j < 0x10; j++) {
				if (mMem[i * 0x10 + j] != mMem[(i + 1) * 0x10 + j]) {
					same = false;
					break;
				}
			}
		}
		//skip if same, not first, not last
		if(same && (i != 0) && (i != (mMem.getSize() / 0x10) - 1)) {
			if(lastSame == 0) {
				printf("0x%04X: ", i * 0x10);
				for(int j = 0; j < 0x10; j++) {
					printf("%02X ", mMem[i*0x10 + j]);
				}
				std::cout << std::endl;
				lastSame++;
			} else if(lastSame == 1) {
				std::cout << "..." << std::endl;
				lastSame++;
			}
		} else {
			lastSame = 0;
			printf("0x%04X: ", i * 0x10);
			for(int j = 0; j < 0x10; j++) {
				printf("%02X ", mMem[i*0x10 + j]);
			}
			std::cout << std::endl;
		}
	}
	std::cout << "++++++++++++++++++++++  END MEM DUMP  ++++++++++++++++++++++" << std::endl;
}

void Processor::DeviceDump() {
	for(unsigned int i = 0; i < mDevices.size(); i++) {
		mDevices[i]->Dump();
	}
}

void Processor::SetInterrupt(unsigned char n) {

	mInterrupt = n;
}
