/*-------------------------------------*\
|
|  File Name: Adc.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  5:05:05 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Adc.hpp"
#include "../Prefix.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdlib>
#include <cstdio>

Adc::Adc(Prefix* pre, std::string text, std::string inst, int op) 
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Adc::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int prefixLen = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction* newAdc = 0;

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
		case ADC_AL_BYTE:
			snprintf(buf, 65, "ADC AL, 0x%02X", (int)*(opLoc + 1));

			inst.insert(0, (char*)memLoc, prefixLen + 2);	

			newAdc = new Adc(prefix, buf, inst, (unsigned char)*opLoc);
			newAdc->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1));
			newAdc->SetOperand(Operand::DST, new RegisterOperand(REG_AL, proc));

			break;
		case ADC_AX_WORD:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);

			snprintf(buf, 65, "ADC AX, 0x%04X", tInt1);

			inst.insert(0, (char*)memLoc, prefixLen + 3);

			newAdc = new Adc(prefix, buf, inst, (unsigned char)*opLoc);
			newAdc->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, 2));
			newAdc->SetOperand(Operand::DST, new RegisterOperand(REG_AX, proc));

			break;

		case GRP1_ADC_MOD_IMM8:
		case GRP1_ADC_MOD_IMM16:
		case GRP1_ADC_MOD_SIMM8:

			modrm = *(opLoc + 1);

			if(((modrm & 0x38) >> 3) == ADC_GRP_CONST) {
				unsigned int immSize = (*opLoc == GRP1_ADC_MOD_IMM8) ? 1 : 2;

				Operand* dst = ModrmOperand::GetModrmOperand(
							proc, opLoc, ModrmOperand::MOD, immSize);

				tInt1 = (int)*(opLoc+2+dst->GetBytecodeLen());
				if(immSize == 2) {
					if(*opLoc == GRP1_ADC_MOD_IMM16) {
						tInt1 += ((int)*(opLoc+3+dst->GetBytecodeLen())) << 8;
					}else {
						tInt1 = (tInt1 >= 0x80) ? 0xFF00 + tInt1 : tInt1;
					}
				}

				if(immSize == 1)
					snprintf(buf, 65, "ADC %s, 0x%02X", "", tInt1);
				else
					snprintf(buf, 65, "ADC %s, 0x%04X", "", tInt1);

				inst.insert(0, (char*)memLoc, prefixLen + 2 + immSize + dst->GetBytecodeLen() - (*opLoc == GRP1_ADC_MOD_SIMM8 ? 1 : 0));
				newAdc = new Adc(prefix, buf, inst, (unsigned char)*opLoc);
				newAdc->SetOperand(Operand::SRC, new ImmediateOperand(tInt1, immSize));
				newAdc->SetOperand(Operand::DST, dst);
			}
			break;

		case ADC_MOD_REG8:
		case ADC_MOD_REG16:
			{
				unsigned int size = *opLoc == ADC_MOD_REG8 ? 1 : 2;
				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::MOD, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::REG, size);
				snprintf(buf, 65, "ADC %s, %s", "", "");
				inst.insert(0, (char*)memLoc, prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newAdc = new Adc(prefix, buf, inst, (unsigned char)*opLoc);
				newAdc->SetOperand(Operand::SRC, src);
				newAdc->SetOperand(Operand::DST, dst);
				
				break;
			}

		case ADC_REG8_MOD:
		case ADC_REG16_MOD:
			{
				unsigned int size = *opLoc == ADC_REG8_MOD ? 1 : 2;

				modrm = *(opLoc + 1);
				Operand* dst = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::REG, size);
				Operand* src = ModrmOperand::GetModrmOperand(
						proc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "ADC %s, %s", "", "");
				inst.insert(0, (char*)memLoc, prefixLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
				newAdc = new Adc(prefix, buf, inst, (unsigned char)*opLoc);
				newAdc->SetOperand(Operand::SRC, src);
				newAdc->SetOperand(Operand::DST, dst);
				
				break;


			}

		default:
			break;
	}

	return newAdc;

}

int Adc::Execute(Processor* proc) {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	unsigned int dstVal = dst->GetValue();
	unsigned int srcVal = src->GetValue();
	unsigned int sign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	unsigned int newVal = dstVal + srcVal + (proc->GetFlag(FLAGS_CF) ? 1 : 0);
	proc->SetFlag(FLAGS_CF, newVal > dst->GetBitmask());
	newVal &= dst->GetBitmask();

	proc->SetFlag(FLAGS_OF, OverflowAdd(dstVal, srcVal, sign == 0x80 ? 1 : 2));
	proc->SetFlag(FLAGS_SF, newVal >= sign);
	proc->SetFlag(FLAGS_ZF, newVal == 0x00);
	proc->SetFlag(FLAGS_AF, AdjustAdd(dstVal, srcVal));
	proc->SetFlag(FLAGS_PF, Parity(newVal));

	dst->SetValue(newVal);

	return 0;
}


