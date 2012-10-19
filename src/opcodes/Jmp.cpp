/*-------------------------------------*\
|
|  File Name: Jmp.cpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:56:21 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Jmp.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Jmp::Jmp(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Jmp::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preLen = 0;

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	Instruction* newJmp = 0;

	switch(*opLoc) {
		case JMP_REL8:
		case JMP_REL16:
		{
			unsigned int size = *opLoc == JMP_REL8 ? 1 : 2;
			unsigned int val = *(opLoc + 1);
			if(size == 2) {
				val += *(opLoc + 2) << 8;
			}
			Operand* dst = new ImmediateOperand(val, size);
			snprintf(buf, 65, "JMP %s", dst->GetDisasm().c_str());
			GETINST(preLen + 1 + size);
			newJmp = new Jmp(pre, buf, inst, (int)*opLoc);
			newJmp->SetOperand(Operand::DST, dst);
			break;
		}
		case JMP_MOD16:
		//case JMP_MOD16_16
		if(((*(opLoc + 1) & 0x38) >> 3) == JMP_SUB_OPCODE || 
				((*(opLoc + 1) & 0x38) >> 3) == JMP_SUB_SEG_OPCODE)
		{
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
			snprintf(buf, 65, "JMP %s", dst->GetDisasm().c_str());
			GETINST(preLen + 3 + dst->GetBytecodeLen());
			newJmp = new Jmp(pre,buf, inst, (int)*opLoc);
			newJmp->SetOperand(Operand::DST, dst);
			((Jmp*)newJmp)->mType = (*(opLoc + 1) & 0x38) >> 3;
			break;
		}
		case JMP_PTR16_16:
		{
			unsigned int val = *(opLoc + 1) +
				(*(opLoc + 2) << 0x08) + ((
				(*(opLoc + 3)) +
				(*(opLoc + 4) << 0x08)) << 0x4);
			Operand* dst = new ImmediateOperand(val & 0xFFFF, 2);
			snprintf(buf, 65, "JMP %s", dst->GetDisasm().c_str());
			GETINST(preLen + 4);
			newJmp = new Jmp(pre, buf, inst, (int)*opLoc);
			newJmp->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newJmp;
			

}

int Jmp::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return -1;
	}

	unsigned int ip = proc->GetRegister(REG_IP);
	switch(mOpcode) {
		case JMP_REL8:
			if(dst->GetValue() >= 0x80)
				ip -= (~dst->GetValue() + 1) & 0xFF;
			else
				ip += dst->GetValue();
			proc->SetRegister(REG_IP, ip & 0xFFFF);
			break;
		case JMP_REL16:
			if(dst->GetValue() >= 0x8000)
				ip -= (~dst->GetValue() + 1) & 0xFFFF;
			else
				ip += dst->GetValue();
			proc->SetRegister(REG_IP, ip & 0xFFFF);
			break;
		case JMP_MOD16:
		//case JMP_MOD16_16
		case JMP_PTR16_16:
			if(mType == JMP_SUB_SEG_OPCODE || mOpcode == JMP_PTR16_16) {		
				ip = (dst->GetValue(4) & 0xFFFF) + 
						(((dst->GetValue(4) & 0xFFFF0000) >> 0x0C) & 0xFFFF);
				proc->SetRegister(REG_IP, ip & 0xFFFF);
				proc->SetRegister(REG_CS, (ip & 0xFFFF0000) >> 0x10);
			} else if(mType == JMP_SUB_OPCODE) {
				ip = dst->GetValue();
				proc->SetRegister(REG_IP, ip);
			}
			break;
	}
	return 0;
}
