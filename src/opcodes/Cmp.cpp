/*-------------------------------------*\
|
|  File Name: Cmp.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:54:44 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Cmp.hpp"
#include "../Prefix.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdlib>
#include <cstdio>

Cmp::Cmp(Prefix* pre, std::string text, std::string inst, int op)
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Cmp::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction8086* newCmp = 0;

	//Build a prefix if possible
	prefix = Prefix::GetPrefix(memLoc);

	//Start looking after the prefix
	if(prefix) {
		opLoc += prefix->GetLength();
		prefixLen += prefix->GetLength();
	}


	std::string inst;
	//Switch for the different valid opcodes
	switch(*opLoc) {
		case CMP_AL_IMM8:
			sprintf(buf, "CMP AL, 0x%02X", (int)*(opLoc + 1));

			GETINST(prefixLen + 2);

			newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
			newCmp->SetProc(mProc);
			newCmp->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset()));
			newCmp->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AL, mProc));

			break;
		case CMP_AX_IMM16:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			sprintf(buf, "CMP AX, 0x%04X", tInt1);

			GETINST(prefixLen + 3);

			newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
			newCmp->SetProc(mProc);
			newCmp->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2, (opLoc + 1).getOffset()));
			newCmp->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AX, mProc));

			break;

		case GRP1_CMP_MOD8_IMM8:
		case GRP1_CMP_MOD16_IMM16:
		case GRP1_CMP_MOD16_IMM8:
			modrm = *(opLoc + 1);
			if(((modrm & 0x38) >> 3) == 7) {
				unsigned int immSize = (*opLoc == GRP1_CMP_MOD8_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							mProc, opLoc, ModrmOperand::MOD, immSize);

				tInt1 = (int)*(opLoc+2+dst->GetBytecodeLen());
				if(immSize == 2) {
					if(*opLoc == GRP1_CMP_MOD16_IMM16) {
						tInt1 += ((int)*(opLoc+3+dst->GetBytecodeLen())) << 8;
					}else {
						tInt1 = (tInt1 >= 0x80) ? 0xFF00 + tInt1 : tInt1;
					}
				}

				if(immSize == 1)
					sprintf(buf, "CMP %s, 0x%02X", "", tInt1);
				else
					sprintf(buf, "CMP %s, 0x%04X", "", tInt1);

				GETINST(prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_CMP_MOD16_IMM8 ? 1 : 0));
				newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
				newCmp->SetProc(mProc);
				newCmp->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize, (opLoc + 2 + dst->GetBytecodeLen()).getOffset()));
				newCmp->SetOperand(Operand::DST, dst);
			}
			break;

		case GRP2_CMP_MOD8_REG8:
		case GRP2_CMP_MOD16_REG16:
			{
				unsigned int size = (*opLoc == GRP2_CMP_MOD8_REG8) ? 1 : 2;
				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				sprintf(buf, "CMP %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
				newCmp->SetProc(mProc);
				newCmp->SetOperand(Operand::SRC, src);
				newCmp->SetOperand(Operand::DST, dst);
				break;
			}

		case GRP3_CMP_REG8_MOD8:
		case GRP3_CMP_REG16_MOD16:
			{
				unsigned int size = *opLoc == GRP3_CMP_REG8_MOD8 ? 1 : 2;

				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				sprintf(buf, "CMP %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
				newCmp->SetProc(mProc);
				newCmp->SetOperand(Operand::SRC, src);
				newCmp->SetOperand(Operand::DST, dst);
				break;


			}

		default:
			break;
	}

	return newCmp;

}

int Cmp::Execute() {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}
	compare(mProc, dst, src);

	return 0;
}

unsigned int Cmp::compare(Processor* proc, Operand* dst, Operand* src) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return ~0;
	Processor8086* mProc = (Processor8086*)proc;
	if(!dst || !src) {
		return 0xFFFFFFFF;
	}

	unsigned int dstVal = dst->GetValue();
	unsigned int srcVal = src->GetValue();
	unsigned int newVal = dstVal - srcVal;
	unsigned int sign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;

	mProc->SetFlag(Processor8086::FLAGS_CF, newVal > dstVal);
	newVal &= dst->GetBitmask();

	mProc->SetFlag(Processor8086::FLAGS_OF, OverflowSub(dstVal, srcVal, sign == 0x80 ? 1 : 2));
	mProc->SetFlag(Processor8086::FLAGS_SF, newVal >= sign);
	mProc->SetFlag(Processor8086::FLAGS_ZF, newVal == 0x00);
	mProc->SetFlag(Processor8086::FLAGS_AF, AdjustSub(dstVal, srcVal));

	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(newVal));

	return newVal;
}

