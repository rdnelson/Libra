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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Aam::Aam(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Aam::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newAam = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAM) {

		unsigned int val = (int)*(opLoc + 1);
		Operand* dst = new ImmediateOperand(val, 1);

		GETINST(preSize + 1 + dst->GetBytecodeLen());
		snprintf(buf, 65, "AAM %s", dst->GetDisasm().c_str());

		newAam = new Aam(pre, buf, inst, (int)*opLoc);
		newAam->SetOperand(Operand::DST, dst);
	}

	return newAam;

}

int Aam::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}

	unsigned int val = proc->GetRegister(REG_AX);
	proc->SetRegisterHigh(REG_AX, val / dst->GetValue());
	proc->SetRegisterLow(REG_AX, val % dst->GetValue());
	return 0;
}
