/*-------------------------------------*\
|
|  File Name: And.cpp
|
|  Creation Date: 06-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  3:42:42 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "And.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../Processor.hpp"

And::And(Prefix* pre, std::string text, std::string inst, int op) {
	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
}

Instruction* And::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	unsigned char modrm = 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newAnd = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case AND_AL_IMM8:
		case AND_AX_IMM16:
		{
			unsigned int size = (*opLoc == AND_AL_IMM8 ? 1 : 2);

			unsigned int val = (int)*(opLoc + 1);
			if(size == 2) {
				val += (int)*(opLoc + 2) << 8;
			}

			GETINST(preSize + 1 + size);

			Operand* src = new ImmediateOperand(val, size, (opLoc + 1).getOffset());
			Operand* dst = new RegisterOperand(*opLoc == AND_AL_IMM8 ? REG_AL : REG_AX, proc);

			newAnd = new And(pre, buf, inst, (unsigned char)*opLoc);
			newAnd->SetOperand(Operand::SRC, src);
			newAnd->SetOperand(Operand::DST, dst);
			break;
		}
		case AND_MOD8_IMM8:
		case AND_MOD16_IMM16:
		case AND_MOD16_IMM8:
		modrm = *(opLoc + 1);
		if(((modrm & 0x38) >> 3) == 4)
		{
			unsigned int size = (*opLoc == AND_MOD8_IMM8 ? 1 : 2);

			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);

			unsigned int val = (int)*(opLoc + 2 + dst->GetBytecodeLen());
			if(size == 2) {
				if(*opLoc == AND_MOD16_IMM8)
					val += ((val >= 0x80) ? 0xFF00 : 0);
				else
					val += (int)*(opLoc + 3 + dst->GetBytecodeLen()) << 8;
			}


			Operand* src = new ImmediateOperand(val, size, (opLoc + 2 + dst->GetBytecodeLen()).getOffset());

			GETINST(preSize + 2 + (*opLoc == AND_MOD16_IMM8 ? 1 : size) + dst->GetBytecodeLen());
			newAnd = new And(pre, buf, inst, (unsigned char)*opLoc);
			newAnd->SetOperand(Operand::SRC, src);
			newAnd->SetOperand(Operand::DST, dst);
		}
		break;

		case AND_MOD8_REG8:
		case AND_MOD16_REG16:
		{
			unsigned int size = (*opLoc == AND_MOD8_REG8 ? 1 : 2);
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, size);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);

			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
			newAnd = new And(pre, buf, inst, (unsigned char)*opLoc);
			newAnd->SetOperand(Operand::SRC, src);
			newAnd->SetOperand(Operand::DST, dst);
			break;
		}
		case AND_REG8_MOD8:
		case AND_REG16_MOD16:
		{
			unsigned int size = (*opLoc == AND_REG8_MOD8 ? 1 : 2);

			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, size);

			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
			newAnd = new And(pre, buf, inst, (unsigned char)*opLoc);
			newAnd->SetOperand(Operand::SRC, src);
			newAnd->SetOperand(Operand::DST, dst);
			break;
		}


	}

	return newAnd;

}

int And::Execute(Processor* proc) {

	if(mOperands[Operand::SRC] == 0 || mOperands[Operand::DST] == 0)
		return INVALID_ARGS;

	unsigned int val = mOperands[Operand::SRC]->GetValue() & mOperands[Operand::DST]->GetValue();
	unsigned int sign = mOperands[Operand::DST]->GetBitmask() == 0xFF ? 0x80 : 0x8000;

	unsigned int parity = val;
	parity ^= parity >> 16;
	parity ^= parity >> 8;
	parity ^= parity >> 4;
	parity &= 0x0f;

	proc->SetFlag(FLAGS_CF, false);
	proc->SetFlag(FLAGS_OF, false);
	proc->SetFlag(FLAGS_PF, (0x6996 >> parity) & 1);
	proc->SetFlag(FLAGS_ZF, val == 0);
	proc->SetFlag(FLAGS_SF, val >= sign);
	mOperands[Operand::DST]->SetValue(val);

	return 0;
}
