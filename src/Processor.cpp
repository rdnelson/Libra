/*-------------------------------------*\
|
|  File Name: Processor.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Fri, Sep 28, 2012 10:40:47 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Processor.hpp"

#include <iostream>

Processor::Processor(unsigned char* mem): mMem(mem) {

}

//Allows the starting address to be changed
int Processor::Initialize(unsigned int startAddr) {


	if(startAddr >= 0x10000) {
		return PROC_ERR_INV_ADDR;
	}

	SetRegister(REG_IP, startAddr);

	return PROC_SUCCESS;

}

//Execute a single instruction
int Processor::Step() {

	//Fetch
	Instruction* inst = Instruction::ReadInstruction(mMem + GetRegister(REG_IP));

	//Ensure it exists and is valid 
	if(inst && inst->IsValid()) {
		//Increment IP
		SetRegister(REG_IP, GetRegister(REG_IP) + inst->GetLength());

		//Execute
		if(inst->Execute(this)) {
			return PROC_ERR_INST;
		}

	} else {
		return PROC_ERR_INV_INST;
	}

	return PROC_SUCCESS;

}

bool Processor::GetFlag(eFlags flag) {
	return (GetRegister(REG_FLAGS) & (1 << flag)) != 0;
}

void Processor::SetFlag(eFlags flag, bool val) {
	if(val)
		SetRegister(REG_FLAGS, GetRegister(REG_FLAGS) | (1 << flag));
	else
		SetRegister(REG_FLAGS, GetRegister(REG_FLAGS) & ~(1 << flag));
}


unsigned int Processor::GetRegister(eRegisters reg) {
	if(reg == NumRegisters) {
		return 0;
	}

	return mRegisters[reg].GetValue();
}

void Processor::SetRegister(eRegisters reg, unsigned int val) {
	if(reg == NumRegisters) {
		return;
	}

	mRegisters[reg].SetValue(val);

}

unsigned int Processor::GetRegisterLow(eRegisters reg) {
	if(reg == NumRegisters) {
		return 0;
	}

	return mRegisters[reg].GetValue() & 0xFF;
}

unsigned int Processor::GetRegisterHigh(eRegisters reg) {
	if(reg == NumRegisters) {
		return 0;
	}

	return mRegisters[reg].GetValue() & 0xFF00;
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


void Processor::ProcDump() {

	std::cout << "++++++++++++++++++++++++ BEGIN DUMP ++++++++++++++++++++++++" << std::endl; 

	std::cout << "Registers:" << std::endl;
	std::cout << std::hex;
	std::cout << "\tAX = 0x" << mRegisters[REG_AX].GetValue();
	std::cout << "\tBX = 0x" << mRegisters[REG_BX].GetValue() << std::endl;
	std::cout << "\tCX = 0x" << mRegisters[REG_CX].GetValue();
	std::cout << "\tDX = 0x" << mRegisters[REG_DX].GetValue() << std::endl;
	std::cout << "\tSI = 0x" << mRegisters[REG_SI].GetValue();
	std::cout << "\tDI = 0x" << mRegisters[REG_DI].GetValue() << std::endl;
	std::cout << "\tBP = 0x" << mRegisters[REG_BP].GetValue();
	std::cout << "\tSP = 0x" << mRegisters[REG_SP].GetValue() << std::endl;

	std::cout << "\tCS = 0x" << mRegisters[REG_CS].GetValue();
	std::cout << "\tDS = 0x" << mRegisters[REG_DS].GetValue() << std::endl;
	std::cout << "\tSS = 0x" << mRegisters[REG_SS].GetValue();
	std::cout << "\tES = 0x" << mRegisters[REG_ES].GetValue() << std::endl;
	std::cout << "\tIP = 0x" << mRegisters[REG_IP].GetValue();
	std::cout << "\tFLAGS = 0x" << mRegisters[REG_FLAGS].GetValue() << std::endl;

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

	std::cout << std::endl;


	std::cout << "++++++++++++++++++++++++  END DUMP  ++++++++++++++++++++++++" << std::endl; 

}

