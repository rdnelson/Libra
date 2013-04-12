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

#include "../Processor8086.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

IncDec::IncDec(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op)
{}

Instruction* IncDec::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction8086* newIncDec = 0;

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
					(mProc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "%s %s", subCode == DEC_SUB_OPCODE ? "DEC" : "INC", dst->GetDisasm().c_str());
				GETINST(preLen + 2 + dst->GetBytecodeLen());
				newIncDec = new IncDec(pre, buf, inst, (int)*opLoc);
				newIncDec->SetProc(mProc);
				((IncDec*)newIncDec)->mType = subCode == DEC_SUB_OPCODE ? DEC : INC;
				newIncDec->SetOperand(Operand::DST, dst);
				break;
			}
		}
	}
	if(*opLoc >= DEC_REG16 && *opLoc < DEC_REG16 + 8) {
		Operand* dst = new RegisterOperand((Processor8086::eRegisters)(*opLoc - DEC_REG16), mProc);
		snprintf(buf, 65, "DEC %s", dst->GetDisasm().c_str());
		GETINST(preLen + 1);
		newIncDec = new IncDec(pre, buf, inst, (int)*opLoc);
		newIncDec->SetProc(mProc);
		newIncDec->SetOperand(Operand::DST, dst);
		((IncDec*)newIncDec)->mType = DEC;
	} else if(*opLoc >= INC_REG16 && *opLoc <= INC_REG16 + 8) {
		Operand* dst = new RegisterOperand((Processor8086::eRegisters)(*opLoc - INC_REG16), mProc);
		snprintf(buf, 65, "INC %s", dst->GetDisasm().c_str());
		GETINST(preLen + 1);
		newIncDec = new IncDec(pre, buf, inst, (int)*opLoc);
		newIncDec->SetProc(mProc);
		newIncDec->SetOperand(Operand::DST, dst);
		((IncDec*)newIncDec)->mType = INC;
	}

	return newIncDec;

}

int IncDec::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}

	unsigned int dstSign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	unsigned int oriVal = dst->GetValue();

	if(mType == DEC) {
		dst->SetValue(dst->GetValue() - 1);
	} else {
		dst->SetValue(dst->GetValue() + 1);
	}

	mProc->SetFlag(Processor8086::FLAGS_OF, dst->GetValue() == (mType == DEC ? dstSign - 1 : dstSign));
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(dst->GetValue()));
	mProc->SetFlag(Processor8086::FLAGS_AF, mType == DEC ? AdjustSub(oriVal, 1) : AdjustAdd(oriVal, 1));
	mProc->SetFlag(Processor8086::FLAGS_SF, (dst->GetValue() & dstSign) != 0);
	mProc->SetFlag(Processor8086::FLAGS_ZF, dst->GetValue() == 0);
	return 0;
}
