/*-------------------------------------*\
|
|  File Name: Add.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:52:41 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Add.hpp"
#include "../Prefix.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdlib>
#include <cstdio>

Add::Add(Prefix* pre, std::string text, std::string inst, int op)
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Add::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction8086* newAdd = 0;

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
		case ADD_AL_BYTE:
			snprintf(buf, 65, "ADD AL, 0x%02X", (int)*(opLoc + 1));

			GETINST(prefixLen + 2);

			newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);
			newAdd->SetProc(mProc);
			newAdd->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset()));
			newAdd->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AL, mProc));

			break;
		case ADD_AX_WORD:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			snprintf(buf, 65, "ADD AX, 0x%04X", tInt1);

			GETINST(prefixLen + 3);

			newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);
			newAdd->SetProc(mProc);
			newAdd->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2, (opLoc + 1).getOffset()));
			newAdd->SetOperand(Operand::DST, new RegisterOperand(Processor8086::REG_AX, mProc));

			break;

		case GRP1_ADD_MOD_IMM8:
		case GRP1_ADD_MOD_IMM16:
		case GRP1_ADD_MOD_SIMM8:

			modrm = *(opLoc + 1);

			if(((modrm & 0x38) >> 3) == 0) {
				unsigned int immSize = (*opLoc == GRP1_ADD_MOD_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							mProc, opLoc, ModrmOperand::MOD, immSize);

				tInt1 = (int)*(opLoc+2+dst->GetBytecodeLen());
				if(immSize == 2) {
					if(*opLoc == GRP1_ADD_MOD_IMM16) {
						tInt1 += ((int)*(opLoc+3+dst->GetBytecodeLen())) << 8;
					}else {
						tInt1 = (tInt1 >= 0x80) ? 0xFF00 + tInt1 : tInt1;
					}
				}

				if(immSize == 1)
					snprintf(buf, 65, "ADD %s, 0x%02X", dst->GetDisasm().c_str(), tInt1);
				else
					snprintf(buf, 65, "ADD %s, 0x%04X", dst->GetDisasm().c_str(), tInt1);

				GETINST(prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_ADD_MOD_SIMM8 ? 1 : 0));
				newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);
				newAdd->SetProc(mProc);
				newAdd->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize, (opLoc + 2 + dst->GetBytecodeLen()).getOffset()));
				newAdd->SetOperand(Operand::DST, dst);
			}
			break;

		case ADD_MOD_REG8:
		case ADD_MOD_REG16:
			{
				unsigned int size = *opLoc == ADD_MOD_REG8 ? 1 : 2;
				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				snprintf(buf, 65, "ADD %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);
				newAdd->SetProc(mProc);
				newAdd->SetOperand(Operand::SRC, src);
				newAdd->SetOperand(Operand::DST, dst);
				
				break;
			}

		case ADD_REG8_MOD:
		case ADD_REG16_MOD:
			{
				unsigned int size = *opLoc == ADD_REG8_MOD ? 1 : 2;

				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						mProc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "ADD %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				GETINST(prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);
				newAdd->SetProc(mProc);
				newAdd->SetOperand(Operand::SRC, src);
				newAdd->SetOperand(Operand::DST, dst);
				
				break;


			}

		default:
			break;
	}

	return newAdd;

}

int Add::Execute() {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	unsigned int dstVal = dst->GetValue();
	unsigned int srcVal = src->GetValue();
	unsigned int newVal = dstVal + srcVal;
	unsigned int sign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	mProc->SetFlag(Processor8086::FLAGS_CF, newVal > dst->GetBitmask());
	newVal &= dst->GetBitmask();

	mProc->SetFlag(Processor8086::FLAGS_OF, OverflowAdd(dstVal, srcVal, sign == 0x80 ? 1 : 2));
	mProc->SetFlag(Processor8086::FLAGS_SF, newVal >= sign);
	mProc->SetFlag(Processor8086::FLAGS_ZF, newVal == 0x00);
	mProc->SetFlag(Processor8086::FLAGS_AF, AdjustAdd(dstVal, srcVal));

	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(newVal));

	dst->SetValue(newVal);

	return 0;
}


