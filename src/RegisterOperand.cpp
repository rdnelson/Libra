/*-------------------------------------*\
|
|  File Name: RegisterOperand.cpp
|
|  Creation Date: 01-10-2012
|
|  Last Modified: Wed, Oct  3, 2012 12:29:21 PM
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
