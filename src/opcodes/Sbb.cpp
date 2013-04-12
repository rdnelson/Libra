/*-------------------------------------*\
|
|  File Name: Sbb.cpp
|
|  Creation Date: 17-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:59:29 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Sbb.hpp"
#include "../Prefix.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <assert.h>

#include <cstdlib>
#include <cstdio>

Sbb::Sbb(Prefix* pre, std::string text, std::string inst, int op)
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Sbb::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction8086* newSbb = 0;

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
		case SBB_AL_IMM8:

			sprintf(buf, "SBB AL, 0x%02X", (int)*(opLoc + 1));

			GETINST(prefixLen + 2);

			newSbb = new Sbb(prefix, buf, inst, (unsigned char)*opLoc);
			newSbb->SetProc(mProc);
			newSbb->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset()));
			newSbb->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AL, mProc));

			break;
		case SBB_AX_IMM16:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			sprintf(buf, "SBB AX, 0x%04X", tInt1);

			GETINST(prefixLen + 3);

			newSbb = new Sbb(prefix, buf, inst, (unsigned char)*opLoc);
			newSbb->SetProc(mProc);
			newSbb->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2, (opLoc + 1).getOffset()));
			newSbb->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AX, mProc));

			break;

		case GRP1_SBB_MOD8_IMM8:
		case GRP1_SBB_MOD16_IMM16:
		case GRP1_SBB_MOD16_IMM8:
			modrm = *(opLoc + 1);
			if(((modrm & 0x38) >> 3) == 5) {
				unsigned int immSize = (*opLoc == GRP1_SBB_MOD8_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							mProc, opLoc, ModrmOperand::MOD, immSize);

				tInt1 = (int)*(opLoc+2+dst->GetBytecodeLen());
				if(immSize == 2) {
					if(*opLoc == GRP1_SBB_MOD16_IMM16) {
						tInt1 += ((int)*(opLoc+3+dst->GetBytecodeLen())) << 8;
					}else {
						tInt1 = (tInt1 >= 0x80) ? 0xFF00 + tInt1 : tInt1;
					}
				}

				if(immSize == 1)
					sprintf(buf, "SBB %s, 0x%02X", "", tInt1);
				else
					sprintf(buf, "SBB %s, 0x%04X", "", tInt1);

				GETINST(prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_SBB_MOD16_IMM8 ? 1 : 0));
				newSbb = new Sbb(prefix, buf, inst, (unsigned char)*opLoc);
				newSbb->SetProc(mProc);
				newSbb->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize, (opLoc + 2 + dst->GetBytecodeLen()).getOffset()));
				newSbb->SetOperand(Operand::DST, dst);
			}
			break;

		case SBB_MOD8_REG8:
		case SBB_MOD16_REG16:
			{
				unsigned int size = (*opLoc == SBB_MOD8_REG8) ? 1 : 2;
				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				sprintf(buf, "SBB %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newSbb = new Sbb(prefix, buf, inst, (unsigned char)*opLoc);
				newSbb->SetProc(mProc);
				newSbb->SetOperand(Operand::SRC, src);
				newSbb->SetOperand(Operand::DST, dst);

				break;
			}

		case SBB_REG8_MOD8:
		case SBB_REG16_MOD16:
			{
				unsigned int size = *opLoc == SBB_REG8_MOD8 ? 1 : 2;

				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				sprintf(buf, "SBB %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newSbb = new Sbb(prefix, buf, inst, (unsigned char)*opLoc);
				newSbb->SetProc(mProc);
				newSbb->SetOperand(Operand::SRC, src);
				newSbb->SetOperand(Operand::DST, dst);

				break;


			}

		default:
			break;
	}

	return newSbb;

}

int Sbb::Execute() {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];
	src->SetValue(src->GetValue() + mProc->GetFlag(Processor8086::FLAGS_CF) ? 1 : 0);

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	dst->SetValue(Cmp::compare((Processor8086*)mProc, dst, src));

	return 0;
}
