/*-------------------------------------*\
|
|  File Name: Cmp.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tues, Oct 9, 2012 12:00:52 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Cmp.hpp"
#include "../Prefix.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <assert.h>

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

Instruction* Cmp::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction* newCmp = 0;

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

			inst.insert(0, (char*)memLoc, prefixLen + 2);	

			newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
			newCmp->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1));
			newCmp->SetOperand(Operand::DST, new RegisterOperand(REG_AL, proc));

			break;
		case CMP_AX_IMM16:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			sprintf(buf, "CMP AX, 0x%04X", tInt1);

			printf(buf);

			inst.insert(0, (char*)memLoc, prefixLen + 3);

			newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
			newCmp->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2));
			newCmp->SetOperand(Operand::DST, new RegisterOperand(REG_AX, proc));

			break;

		case GRP1_CMP_MOD8_IMM8:
		case GRP1_CMP_MOD16_IMM16:
		case GRP1_CMP_MOD16_IMM8:
			modrm = *(opLoc + 1);
			if(((modrm & 0x38) >> 3) == 7) {
				unsigned int immSize = (*opLoc == GRP1_CMP_MOD8_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							proc, opLoc, ModrmOperand::MOD, immSize);

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

				inst.insert(0, (char*)memLoc, prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_CMP_MOD16_IMM8 ? 1 : 0));
				newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
				newCmp->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize));
				newCmp->SetOperand(Operand::DST, dst);
			}
			break;

		case GRP2_CMP_MOD8_REG8:
		case GRP2_CMP_MOD16_REG16:
			{
				unsigned int size = (*opLoc == GRP2_CMP_MOD8_REG8) ? 1 : 2;
				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::REG, size);
				sprintf(buf, "CMP %s, %s", "", "");
				inst.insert(0, (char*)memLoc, prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
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
						proc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::MOD, size);
				sprintf(buf, "CMP %s, %s", "", "");
				inst.insert(0, (char*)memLoc, prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newCmp = new Cmp(prefix, buf, inst, (unsigned char)*opLoc);
				newCmp->SetOperand(Operand::SRC, src);
				newCmp->SetOperand(Operand::DST, dst);
				
				break;


			}

		default:
			break;
	}

	return newCmp;

}

int Cmp::Execute(Processor* proc) {

	unsigned int parity = 0;

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	unsigned int dstVal = dst->GetValue();
	unsigned int srcVal = src->GetValue();
	unsigned int newVal = dstVal - srcVal;
	unsigned int sign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	
	proc->SetFlag(FLAGS_CF, newVal > dstVal);
	newVal &= dst->GetBitmask();

	proc->SetFlag(FLAGS_OF, OverflowSub(newVal, dstVal, srcVal, sign == 0x80 ? 1 : 2));
	proc->SetFlag(FLAGS_SF, newVal >= sign);
	proc->SetFlag(FLAGS_ZF, newVal == 0x00);
	proc->SetFlag(FLAGS_AF, AdjustSub(dstVal, srcVal));

	proc->SetFlag(FLAGS_PF, Parity(newVal));

	return 0;
}