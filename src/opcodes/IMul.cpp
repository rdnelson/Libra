/*-------------------------------------*\
|
|  File Name: IMul.cpp
|
|  Creation Date: 16-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:55:50 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "IMul.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

IMul::IMul(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* IMul::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	bool twoByte = false;
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	Instruction* newIMul = 0;

	if(*opLoc == TWO_BYTE_OPCODE) {
		opLoc++;
		preLen++;
		twoByte = true;
	}

	switch(*opLoc) {
		case IMUL_MOD8:
		case IMUL_MOD16:
		{
			if(((unsigned int)((*(opLoc + 1) & 0x38) >> 3) != IMUL_SUB_OPCODE) || twoByte) {
				return 0;
			}

			unsigned int size = *opLoc == IMUL_MOD8 ? 1 : 2;
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			snprintf(buf, 65, "IMUL %s", dst->GetDisasm().c_str());
			GETINST(preLen + 2 + dst->GetBytecodeLen());
			newIMul = new IMul(pre, buf, inst, (int)*opLoc);
			newIMul->SetOperand(Operand::DST, dst);
			break;
		}
		case IMUL_TWO_R16_MOD16:
		{
			if(!twoByte) {
				return 0;
			}
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, 2);
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
			snprintf(buf, 65, "IMUL %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
			GETINST(preLen + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
			newIMul = new IMul(pre, buf, inst, (int)*opLoc);
			newIMul->SetOperand(Operand::DST, dst);
			newIMul->SetOperand(Operand::SRC, src);
			break;
		}
		case IMUL_R16_MOD16_IMM8:
		case IMUL_R16_MOD16_IMM16:
		{
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, 2);
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
			unsigned int val = (int)*(opLoc + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
			unsigned int size = *opLoc == IMUL_R16_MOD16_IMM8 ? 1 : 2;
			if(size == 2) {
				val += (int)*(opLoc + 3 + dst->GetBytecodeLen() + src->GetBytecodeLen());
			}
			Operand* op3 = new ImmediateOperand(val, size, (opLoc + 2 + dst->GetBytecodeLen()).getOffset());
			snprintf(buf, 65, "IMUL %s, %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str(), op3->GetDisasm().c_str());
			GETINST(preLen + 2 + dst->GetBytecodeLen() +
					src->GetBytecodeLen() + op3->GetBytecodeLen());
			newIMul = new IMul(pre, buf, inst, (int)*opLoc);
			newIMul->SetOperand(Operand::DST, dst);
			newIMul->SetOperand(Operand::SRC, src);
			newIMul->SetOperand(Operand::OP3, op3);
			break;
		}
	}
	return newIMul;

}

int IMul::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0)
		return INVALID_ARGS;

	switch(mOpcode) {
		case IMUL_MOD8:
		case IMUL_MOD16:
		{
			unsigned int al = proc->GetRegister(mOpcode == IMUL_MOD8 ? REG_AL : REG_AX);
			unsigned int val = dst->GetValue();
			unsigned int bitMask = dst->GetBitmask();
			unsigned int signMask = bitMask == 0xFF ? 0x80 : 0x8000;
			unsigned int sign = ((al & signMask) ^ (val & signMask)) != 0;


			if(al >= signMask) { //negative
				al = (~al + 1) & bitMask;
			}
			if(val >= signMask) {
				val = (~val + 1) & bitMask;
			}

			val *= al;
			if(val > bitMask) { 
				proc->SetFlag(FLAGS_CF, 1);
				proc->SetFlag(FLAGS_OF, 1);
			}
			if(sign)
				val *= -1;
			proc->SetRegister(REG_AX, val & 0xFFFF);
			if(bitMask == 0xFFFF) {
				proc->SetRegister(REG_DX, (val & 0xFFFF0000) >> 0x10);
			}
			break;
		}
		case IMUL_TWO_R16_MOD16:
		{
			unsigned int reg = dst->GetValue();
			unsigned int mod = mOperands[Operand::SRC]->GetValue();
			unsigned int sign = ((reg & 0x8000) ^ (mod & 0x8000)) != 0;
			if(reg >= 0x8000) {
				reg = (~reg + 1) & 0xFFFF;
			}
			if(mod >= 0x8000) {
				mod = (~mod + 1) & 0xFFFF;
			}
			reg *= mod;
			if(reg != (reg & 0xFFFF)) {
				proc->SetFlag(FLAGS_CF, 1);
				proc->SetFlag(FLAGS_OF, 1);
			} else {
				proc->SetFlag(FLAGS_CF, 0);
				proc->SetFlag(FLAGS_OF, 0);
			}
			if(sign)
				reg *= -1;
			dst->SetValue(reg & 0xFFFF);
			break;
		}
		case IMUL_R16_MOD16_IMM8:
		case IMUL_R16_MOD16_IMM16:
		{
			unsigned int mod = mOperands[Operand::SRC]->GetValue();
			unsigned int val = mOperands[Operand::OP3]->GetValue();
			if(mOpcode == IMUL_R16_MOD16_IMM8 || val >= 0x80) {
				val |= 0xFF00;
			}
			unsigned int sign = ((mod & 0x8000) ^ (val & 0x8000)) != 0;
			if(mod >= 0x8000) { 
				mod = (~mod + 1) & 0xFFFF;
			}
			if(val >= 0x8000) {
				val = (~val + 1) & 0xFFFF;
			}
			mod *= val;
			if(mod != (mod & 0xFFFF)) {
				proc->SetFlag(FLAGS_CF, 1);
				proc->SetFlag(FLAGS_OF, 1);
			} else {
				proc->SetFlag(FLAGS_CF, 0);
				proc->SetFlag(FLAGS_OF, 0);
			}
			if(sign)
				mod *= -1;
			dst->SetValue(mod & 0xFFFF);
			break;
		}
		default:
			return INVALID_ARGS;
	}

	return 0;
}
