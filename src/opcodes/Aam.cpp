/*-------------------------------------*\
|
|  File Name: Aam.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:48:56 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Aam.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Aam::Aam(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Aam::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newAam = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAM) {

		unsigned int val = (int)*(opLoc + 1);
		Operand* dst = new ImmediateOperand(val, 1, (opLoc + 1).getOffset());

		GETINST(preSize + 1 + dst->GetBytecodeLen());
		snprintf(buf, 65, "AAM %s", dst->GetDisasm().c_str());

		newAam = new Aam(pre, buf, inst, (int)*opLoc);
		newAam->SetProc(mProc);
		newAam->SetOperand(Operand::DST, dst);
	}

	return newAam;

}

int Aam::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}

	unsigned int val = mProc->GetRegister(Processor8086::REG_AX);
	mProc->SetRegisterHigh(Processor8086::REG_AX, val / dst->GetValue());
	mProc->SetRegisterLow(Processor8086::REG_AX, val % dst->GetValue());
	return 0;
}
