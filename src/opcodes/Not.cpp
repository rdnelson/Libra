/*-------------------------------------*\
|
|  File Name: Not.cpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thurs, Oct 18, 2012  7:19:00 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Not.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Not::Not(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op)
{}

Instruction* Not::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {

	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction* newNot = 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	switch(*opLoc) {
		case NOT_MOD8:
		case NOT_MOD16:
		{
			unsigned int modrm = (*(opLoc + 1) & 0x38) >> 3;
			if(modrm == 2) {
				unsigned int size = (*opLoc == NOT_MOD8 ? 1 : 2);
				Operand* dst = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "NOT %s", dst->GetDisasm().c_str());
				GETINST(preLen + 2 + dst->GetBytecodeLen());
				newNot = new Not(pre, buf, inst, (int)*opLoc);
				newNot->SetOperand(Operand::DST, dst);
				break;
			}
		}
	}
	return newNot;

}

int Not::Execute(Processor*) {
	Operand* dst = mOperands[Operand::DST];

	if(!dst) {
		return INVALID_ARGS;
	}

	unsigned int dstVal = dst->GetValue();

	dst->SetValue((~dstVal) & dst->GetBitmask());
	return 0;
}
