/*-------------------------------------*\
|
|  File Name: Or.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:54:44 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Or.hpp"
#include "../Prefix.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdlib>
#include <cstdio>

Or::Or(Prefix* pre, std::string text, std::string inst, int op)
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Or::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction8086* newOr = 0;

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
		case OR_AL_IMM8:
			sprintf(buf, "OR AL, 0x%02X", (int)*(opLoc + 1));

			GETINST(prefixLen + 2);

			newOr = new Or(prefix, buf, inst, (unsigned char)*opLoc);
			newOr->SetProc(mProc);
			newOr->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1,(opLoc + 1).getOffset()));
			newOr->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AL, mProc));

			break;
		case OR_AX_IMM16:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			sprintf(buf, "OR AX, 0x%04X", tInt1);

			GETINST(prefixLen + 3);

			newOr = new Or(prefix, buf, inst, (unsigned char)*opLoc);
			newOr->SetProc(mProc);
			newOr->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2, (opLoc + 1).getOffset()));
			newOr->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AX, mProc));

			break;

		case GRP1_OR_MOD8_IMM8:
		case GRP1_OR_MOD16_IMM16:
		case GRP1_OR_MOD16_IMM8:
			modrm = (*(opLoc + 1) & 0x38) >> 3;
			if(modrm == 1) {
				unsigned int immSize = (*opLoc == GRP1_OR_MOD8_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							mProc, opLoc, ModrmOperand::MOD, immSize);

				tInt1 = (int)*(opLoc+2+dst->GetBytecodeLen());
				if(immSize == 2) {
					if(*opLoc == GRP1_OR_MOD16_IMM16) {
						tInt1 += ((int)*(opLoc+3+dst->GetBytecodeLen())) << 8;
					}else {
						tInt1 = (tInt1 >= 0x80) ? 0xFF00 + tInt1 : tInt1;
					}
				}

				if(immSize == 1)
					sprintf(buf, "OR %s, 0x%02X", "", tInt1);
				else
					sprintf(buf, "OR %s, 0x%04X", "", tInt1);

				GETINST(prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_OR_MOD16_IMM8 ? 1 : 0));
				newOr = new Or(prefix, buf, inst, (unsigned char)*opLoc);
				newOr->SetProc(mProc);
				newOr->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize, (opLoc + 2 + dst->GetBytecodeLen()).getOffset()));
				newOr->SetOperand(Operand::DST, dst);
			}
			break;

		case GRP2_OR_MOD8_REG8:
		case GRP2_OR_MOD16_REG16:
			{
				unsigned int size = (*opLoc == GRP2_OR_MOD8_REG8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				sprintf(buf, "OR %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newOr = new Or(prefix, buf, inst, (unsigned char)*opLoc);
				newOr->SetProc(mProc);
				newOr->SetOperand(Operand::SRC, src);
				newOr->SetOperand(Operand::DST, dst);
				break;
			}

		case GRP3_OR_REG8_MOD8:
		case GRP3_OR_REG16_MOD16:
			{
				unsigned int size = *opLoc == GRP3_OR_REG8_MOD8 ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				sprintf(buf, "OR %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newOr = new Or(prefix, buf, inst, (unsigned char)*opLoc);
				newOr->SetProc(mProc);
				newOr->SetOperand(Operand::SRC, src);
				newOr->SetOperand(Operand::DST, dst);
				break;


			}

		default:
			break;
	}

	return newOr;

}

int Or::Execute() {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	unsigned int dstVal = dst->GetValue();
	unsigned int srcVal = src->GetValue();
	unsigned int newVal = dstVal | srcVal;

	unsigned int sign = (dst->GetBitmask() == 0xFF) ? 0x80 : 0x8000;

	mProc->SetFlag(Processor8086::FLAGS_OF, 0);
	mProc->SetFlag(Processor8086::FLAGS_CF, 0);
	mProc->SetFlag(Processor8086::FLAGS_SF, newVal >= sign);
	mProc->SetFlag(Processor8086::FLAGS_ZF, newVal == 0x00);
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(newVal));

	dst->SetValue(newVal);

	return 0;
}
