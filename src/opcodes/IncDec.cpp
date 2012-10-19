/*-------------------------------------*\
|
|  File Name: IncDec.cpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:12:36 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "IncDec.hpp"

#include "../Processor.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

IncDec::IncDec(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op)
{}

Instruction* IncDec::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction* newIncDec = 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	switch(*opLoc) {
		case DEC_MOD8:
		case DEC_MOD16:
		{
			unsigned int subCode = (*(opLoc + 1) & 0x38) >> 3;
			if(subCode == DEC_SUB_OPCODE || subCode == INC_SUB_OPCODE) {
				unsigned int size = (*opLoc == DEC_MOD8 ? 1 : 2);
				Operand* dst = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "%s %s", subCode == DEC_SUB_OPCODE ? "DEC" : "INC", dst->GetDisasm().c_str());
				GETINST(preLen + 2 + dst->GetBytecodeLen());
				newIncDec = new IncDec(pre, buf, inst, (int)*opLoc);
				((IncDec*)newIncDec)->mType = subCode == DEC_SUB_OPCODE ? DEC : INC;
				newIncDec->SetOperand(Operand::DST, dst);
				break;
			}
		}
	}
	if(*opLoc >= DEC_REG16 && *opLoc < DEC_REG16 + 8) {
		Operand* dst = new RegisterOperand((eRegisters)(*opLoc - DEC_REG16), proc);
		snprintf(buf, 65, "DEC %s", dst->GetDisasm().c_str());
		GETINST(preLen + 1);
		newIncDec = new IncDec(pre, buf, inst, (int)*opLoc);
		newIncDec->SetOperand(Operand::DST, dst);
		((IncDec*)newIncDec)->mType = DEC;
	} else if(*opLoc >= INC_REG16 && *opLoc <= INC_REG16 + 8) {
		Operand* dst = new RegisterOperand((eRegisters)(*opLoc - INC_REG16), proc);
		snprintf(buf, 65, "INC %s", dst->GetDisasm().c_str());
		GETINST(preLen + 1);
		newIncDec = new IncDec(pre, buf, inst, (int)*opLoc);
		newIncDec->SetOperand(Operand::DST, dst);
		((IncDec*)newIncDec)->mType = INC;
	}

	return newIncDec;

}

int IncDec::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return -1;
	}

	if(mType == DEC) {
		dst->SetValue(dst->GetValue() - 1);
	} else {
		dst->SetValue(dst->GetValue() + 1);
	}
	return 0;
}
