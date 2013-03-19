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

RegisterOperand::RegisterOperand(eRegisters reg, Processor* proc) : mReg(reg), mProc(proc), mSection(0) {
	if(mReg > NumRegisters) {
		if(mReg < LowRegisters) {
			mSection = 1;
		} else if(mReg < HighRegisters) {
			mSection = 2;
		}
	}
}

unsigned int RegisterOperand::GetValue() {

	if(mSection == 0)
		return mProc->GetRegister(mReg);
	else if(mSection == 1)
		return mProc->GetRegisterLow((eRegisters)(mReg - NumRegisters - 1));
	else if(mSection == 2)
		return mProc->GetRegisterHigh((eRegisters)(mReg - LowRegisters - 1));

	//shouldn't hit
	return 0;
}

void RegisterOperand::SetValue(unsigned int val) {
	mProc->SetRegister(mReg, val);
}

unsigned int RegisterOperand::GetBitmask() {
	if(mReg < NumRegisters)
		return 0xFFFF;
	if(mReg < AllRegisters)
		return 0xFF;
	return 0;
}

const std::string RegisterOperand::GetDisasm() {
	switch(mReg) {
		case REG_AX:
			return "AX";
		case REG_BX:
			return "BX";
		case REG_CX:
			return "CX";
		case REG_DX:
			return "DX";
		case REG_SP:
			return "SP";
		case REG_BP:
			return "BP";
		case REG_SI:
			return "SI";
		case REG_DI:
			return "DI";
		case REG_AH:
			return "AH";
		case REG_AL:
			return "AL";
		case REG_BH:
			return "BH";
		case REG_BL:
			return "BL";
		case REG_CH:
			return "CH";
		case REG_CL:
			return "CL";
		case REG_DH:
			return "DH";
		case REG_DL:
			return "DL";
		case REG_CS:
			return "CS";
		case REG_DS:
			return "DS";
		case REG_SS:
			return "SS";
		case REG_ES:
			return "ES";
		case REG_IP:
			return "IP";
		case REG_FLAGS:
			return "FLAGS";
		default:
			return "INVALID";
	};
	return "INVALID";
}

int RegisterOperand::GetUnresolvedValue() const {
	return mReg + 0x10000;
}

