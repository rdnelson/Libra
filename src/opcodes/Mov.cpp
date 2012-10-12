/*-------------------------------------*\
|
|  File Name: Mov.cpp
|
|  Creation Date: 03-10-2012
|
|  Last Modified: Thu, Oct  4, 2012  7:13:51 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Mov.hpp"
#include "../Instruction.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../AddressOperand.hpp"
#include "../Prefix.hpp"

#include <cstdio>
#include <string>

#define GETINST(len) inst.insert(0, (char*)memLoc, len)

Mov::Mov(Prefix* pre, std::string text, std::string inst, int op)
{
        mPrefix = pre;
        mText = text;
        mInst = inst;
        mOpcode = op;
        mValid = true;
}

Instruction* Mov::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	Instruction* newMov = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case MOV_MOD8_REG8:
		case MOV_MOD16_REG16:
		{
			unsigned int size = (*opLoc == MOV_MOD8_REG8 ? 1 : 2);
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, size);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);

			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}

		case MOV_REG8_MOD8:
		case MOV_REG16_MOD16:
		{
			unsigned int size = (*opLoc == MOV_REG8_MOD8 ? 1 : 2);
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, size);

			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}
		case MOV_MOD16_SEGREG:
		{
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::SEGREG, 2);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);

			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}

		case MOV_SEGREG_MOD16:
		{
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::SEGREG, 2);

			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}

		case MOV_AL_MOFF8:
		case MOV_AX_MOFF16:
		{
			unsigned int size = (*opLoc == MOV_AL_MOFF8 ? 1 : 2);
			Operand* dst = new RegisterOperand(*opLoc == MOV_AL_MOFF8 ? REG_AL : REG_AX, proc);

			unsigned int val = (int)*(opLoc + 1);
			val += ((int)*(opLoc + 2)) << 8;

			Operand* src = AddressOperand::GetAddressOperand(proc, val, size);

			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 3);

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}

		case MOV_MOFF8_AL:
		case MOV_MOFF16_AX:
		{
			unsigned int size = (*opLoc == MOV_MOFF8_AL ? 1 : 2);
			Operand* src = new RegisterOperand(*opLoc == MOV_MOFF8_AL ? REG_AL : REG_AX, proc);

			unsigned int val = (int)*(opLoc + 1);
			val += ((int)*(opLoc + 2)) << 8;

			Operand* dst = AddressOperand::GetAddressOperand(proc, val, size);

			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 3);

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}

		case MOV_AL_IMM8:
		case MOV_CL_IMM8:
		case MOV_DL_IMM8:
		case MOV_BL_IMM8:
		case MOV_AH_IMM8:
		case MOV_CH_IMM8:
		case MOV_DH_IMM8:
		case MOV_BH_IMM8:
		case MOV_AX_IMM16:
		case MOV_CX_IMM16:
		case MOV_DX_IMM16:
		case MOV_BX_IMM16:
		case MOV_SP_IMM16:
		case MOV_BP_IMM16:
		case MOV_SI_IMM16:
		case MOV_DI_IMM16:
		{
			unsigned int size = 0;
			Operand* dst = 0;

			if(*opLoc <= MOV_BL_IMM8) {
				//The 8 bit omes
				size = 1;
				dst = new RegisterOperand((eRegisters)(*opLoc - MOV_AL_IMM8 + REG_AL), proc);
			} else if(*opLoc <= MOV_BH_IMM8) {
				size = 1;
				dst = new RegisterOperand((eRegisters)(*opLoc - MOV_AH_IMM8 + REG_AH), proc);
			} else {
				size = 2;
				dst = new RegisterOperand((eRegisters)(*opLoc - MOV_AX_IMM16), proc);
			}

			unsigned int val = (int)*(opLoc + 1 + dst->GetBytecodeLen());
			if(size == 2) {
				val += (int)*(opLoc + 2 + dst->GetBytecodeLen()) << 8;
			}
			Operand* src = new ImmediateOperand(val, size);
			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 1 + dst->GetBytecodeLen() + src->GetBytecodeLen());

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
			break;
		}
		
		case MOV_MOD8_IMM8:
		case MOV_MOD16_IMM16:
		{
			unsigned int size = (*opLoc == MOV_MOD8_IMM8 ? 1 : 2);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);

			unsigned int val = (int)*(opLoc + 2 + dst->GetBytecodeLen());
			if(size == 2) {
				val += (int)*(opLoc + 3 + dst->GetBytecodeLen()) << 8;
			}
			Operand* src = new ImmediateOperand(val, size);
			snprintf(buf, 65, "MOV %s, %s", "", "");
			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());

			newMov = new Mov(pre, buf, inst, (unsigned char)*opLoc);
			newMov->SetOperand(Operand::SRC, src);
			newMov->SetOperand(Operand::DST, dst);
		}
		break;
	}

	return newMov;
}

int Mov::Execute(Processor* proc) {
	if(mOperands[Operand::SRC] && mOperands[Operand::DST]) {
		mOperands[Operand::DST]->SetValue(mOperands[Operand::SRC]->GetValue());
		return 0;
	}
	return -1;

}

