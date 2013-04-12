/*-------------------------------------*\
|
|  File Name: RegisterOperand.cpp
|
|  Creation Date: 01-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:28:09 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "RegisterOperand.hpp"

RegisterOperand::RegisterOperand(Processor8086::eRegisters reg, Processor8086* proc) : mReg(reg), mProc(proc), mSection(0) {
	if(mReg > Processor8086::NumRegisters) {
		if(mReg < Processor8086::LowRegisters) {
			mSection = 1;
		} else if(mReg < Processor8086::HighRegisters) {
			mSection = 2;
		}
	}
}

unsigned int RegisterOperand::GetValue() {

	if(mSection == 0)
		return mProc->GetRegister(mReg);
	else if(mSection == 1)
		return mProc->GetRegisterLow((Processor8086::eRegisters)(mReg - Processor8086::NumRegisters - 1));
	else if(mSection == 2)
		return mProc->GetRegisterHigh((Processor8086::eRegisters)(mReg - Processor8086::LowRegisters - 1));

	//shouldn't hit
	return 0;
}

void RegisterOperand::SetValue(unsigned int val) {
	mProc->SetRegister(mReg, val);
}

unsigned int RegisterOperand::GetBitmask() {
	if(mReg < Processor8086::NumRegisters)
		return 0xFFFF;
	if(mReg < Processor8086::AllRegisters)
		return 0xFF;
	return 0;
}

const std::string RegisterOperand::GetDisasm() {
	switch(mReg) {
		case Processor8086::REG_AX:
			return "AX";
		case Processor8086::REG_BX:
			return "BX";
		case Processor8086::REG_CX:
			return "CX";
		case Processor8086::REG_DX:
			return "DX";
		case Processor8086::REG_SP:
			return "SP";
		case Processor8086::REG_BP:
			return "BP";
		case Processor8086::REG_SI:
			return "SI";
		case Processor8086::REG_DI:
			return "DI";
		case Processor8086::REG_AH:
			return "AH";
		case Processor8086::REG_AL:
			return "AL";
		case Processor8086::REG_BH:
			return "BH";
		case Processor8086::REG_BL:
			return "BL";
		case Processor8086::REG_CH:
			return "CH";
		case Processor8086::REG_CL:
			return "CL";
		case Processor8086::REG_DH:
			return "DH";
		case Processor8086::REG_DL:
			return "DL";
		case Processor8086::REG_CS:
			return "CS";
		case Processor8086::REG_DS:
			return "DS";
		case Processor8086::REG_SS:
			return "SS";
		case Processor8086::REG_ES:
			return "ES";
		case Processor8086::REG_IP:
			return "IP";
		case Processor8086::REG_FLAGS:
			return "FLAGS";
		default:
			return "INVALID";
	};
	return "INVALID";
}

int RegisterOperand::GetUnresolvedValue() const {
	return mReg + 0x10000;
}

