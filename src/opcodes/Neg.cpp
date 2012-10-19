/*-------------------------------------*\
|
|  File Name: Neg.cpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thurs, Oct 18, 2012  4:19:00 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Neg.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Neg::Neg(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op)
{}

Instruction* Neg::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction* newNeg = 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	switch(*opLoc) {
		case NEG_MOD8:
		case NEG_MOD16:
		{
			unsigned int modrm = (*(opLoc + 1) & 0x38) >> 3;
			if(modrm == 3) {
				unsigned int size = (*opLoc == NEG_MOD8 ? 1 : 2);
				Operand* dst = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "NEG %s", dst->GetDisasm().c_str());
				GETINST(preLen + 2 + dst->GetBytecodeLen());
				newNeg = new Neg(pre, buf, inst, (int)*opLoc);
				newNeg->SetOperand(Operand::DST, dst);
				break;
			}
		}
	}
	return newNeg;

}

int Neg::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	unsigned int dstVal = ~dst->GetValue() + 1;
	unsigned int sign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	proc->SetFlag(FLAGS_CF, dstVal != 0);
	proc->SetFlag(FLAGS_SF, dstVal >= sign);
	proc->SetFlag(FLAGS_ZF, dstVal == 0x00);
	proc->SetFlag(FLAGS_PF, Parity(dstVal));
	proc->SetFlag(FLAGS_AF, AdjustSub(dstVal, dstVal*2));
	proc->SetFlag(FLAGS_OF, dstVal == (~dstVal + 1) & dst->GetBitmask());

	dst->SetValue(dstVal & dst->GetBitmask());
	return 0;
}
