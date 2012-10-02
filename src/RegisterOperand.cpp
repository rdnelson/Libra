/*-------------------------------------*\
|
|  File Name: RegisterOperand.cpp
|
|  Creation Date: 01-10-2012
|
|  Last Modified: Mon, Oct  1, 2012 11:21:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "RegisterOperand.hpp"

RegisterOperand::RegisterOperand(eRegisters reg, Processor* proc) : mReg(reg), mProc(proc) {
}

unsigned int RegisterOperand::GetValue() {

	return mProc->GetRegister(mReg);
}

void RegisterOperand::SetValue(unsigned int val) {
	mProc->SetRegister(mReg, val);
}

unsigned int RegisterOperand::GetBitmask() {
	if(mReg < NumRegisters)
		return 0xFFFF;
	if(mReg < AllRegisters)
		return 0xFF;
}
