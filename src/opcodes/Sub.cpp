/*-------------------------------------*\
|
|  File Name: Sub.cpp
|
|  Creation Date: 17-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:59:29 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Sub.hpp"
#include "../Prefix.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <assert.h>

#include <cstdlib>
#include <cstdio>

Sub::Sub(Prefix* pre, std::string text, std::string inst, int op) 
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Sub::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction* newSub = 0;

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
		case SUB_AL_IMM8:
			
			sprintf(buf, "SUB AL, 0x%02X", (int)*(opLoc + 1));

			inst.insert(0, (char*)memLoc, prefixLen + 2);	

			newSub = new Sub(prefix, buf, inst, (unsigned char)*opLoc);
			newSub->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1));
			newSub->SetOperand(Operand::DST, new RegisterOperand(REG_AL, proc));

			break;
		case SUB_AX_IMM16:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			sprintf(buf, "SUB AX, 0x%04X", tInt1);

			printf(buf);

			inst.insert(0, (char*)memLoc, prefixLen + 3);

			newSub = new Sub(prefix, buf, inst, (unsigned char)*opLoc);
			newSub->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2));
			newSub->SetOperand(Operand::DST, new RegisterOperand(REG_AX, proc));

			break;

		case GRP1_SUB_MOD8_IMM8:
		case GRP1_SUB_MOD16_IMM16:
		case GRP1_SUB_MOD16_IMM8:
			modrm = *(opLoc + 1);
			if(((modrm & 0x38) >> 3) == 5) {
				unsigned int immSize = (*opLoc == GRP1_SUB_MOD8_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							proc, opLoc, ModrmOperand::MOD, immSize);

				tInt1 = (int)*(opLoc+2+dst->GetBytecodeLen());
				if(immSize == 2) {
					if(*opLoc == GRP1_SUB_MOD16_IMM16) {
						tInt1 += ((int)*(opLoc+3+dst->GetBytecodeLen())) << 8;
					}else {
						tInt1 = (tInt1 >= 0x80) ? 0xFF00 + tInt1 : tInt1;
					}
				}

				if(immSize == 1)
					sprintf(buf, "SUB %s, 0x%02X", "", tInt1);
				else
					sprintf(buf, "SUB %s, 0x%04X", "", tInt1);

				inst.insert(0, (char*)memLoc, prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_SUB_MOD16_IMM8 ? 1 : 0));
				newSub = new Sub(prefix, buf, inst, (unsigned char)*opLoc);
				newSub->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize));
				newSub->SetOperand(Operand::DST, dst);
			}
			break;

		case GRP2_SUB_MOD8_REG8:
		case GRP2_SUB_MOD16_REG16:
			{
				unsigned int size = (*opLoc == GRP2_SUB_MOD8_REG8) ? 1 : 2;
				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::REG, size);
				sprintf(buf, "SUB %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				inst.insert(0, (char*)memLoc, prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newSub = new Sub(prefix, buf, inst, (unsigned char)*opLoc);
				newSub->SetOperand(Operand::SRC, src);
				newSub->SetOperand(Operand::DST, dst);
				
				break;
			}

		case GRP3_SUB_REG8_MOD8:
		case GRP3_SUB_REG16_MOD16:
			{
				unsigned int size = *opLoc == GRP3_SUB_REG8_MOD8 ? 1 : 2;

				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::MOD, size);
				sprintf(buf, "SUB %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
				inst.insert(0, (char*)memLoc, prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newSub = new Sub(prefix, buf, inst, (unsigned char)*opLoc);
				newSub->SetOperand(Operand::SRC, src);
				newSub->SetOperand(Operand::DST, dst);
				
				break;


			}

		default:
			break;
	}

	return newSub;

}

int Sub::Execute(Processor* proc) {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	dst->SetValue(Cmp::compare(proc, dst, src));

	return 0;
}
