/*-------------------------------------*\
|
|  File Name: Aad.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:48:36 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Aad.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Aad::Aad(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Aad::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newAad = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAD) {

		unsigned int val = (int)*(opLoc + 1);
		Operand* dst = new ImmediateOperand(val, 1, (opLoc + 1).getOffset());

		GETINST(preSize + 1 + dst->GetBytecodeLen());
		snprintf(buf, 65, "AAD %s", dst->GetDisasm().c_str());

		newAad = new Aad(pre, buf, inst, (int)*opLoc);
		newAad->SetOperand(Operand::DST, dst);
	}

	return newAad;

}

int Aad::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}

	unsigned int al = proc->GetRegister(REG_AL);
	unsigned int ah = proc->GetRegister(REG_AH);
	proc->SetRegisterHigh(REG_AX, 0);
	proc->SetRegisterLow(REG_AX, al = (al + (ah * dst->GetValue())) & 0xFF);
	proc->SetFlag(FLAGS_SF, al >= 0x80);
	proc->SetFlag(FLAGS_ZF, al == 0);
	proc->SetFlag(FLAGS_PF, Parity(al));
	return 0;
}
