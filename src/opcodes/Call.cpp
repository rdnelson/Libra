/*-------------------------------------*\
|
|  File Name: Call.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tue, Oct  9, 2012  3:38:52 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Call.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Call::Call(Prefix* pre, std::string text, std::string inst, int op) {

	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
}

Instruction* Call::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	
	Instruction* newCall = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case CALL_REL16:
		case CALL_ABS16_16:
		{
			unsigned int size = *opLoc == CALL_REL16 ? 2 : 4;
			unsigned int val = (unsigned int)*(opLoc + 1) + ((unsigned int)*(opLoc + 2) << 8);
			unsigned int seg = (unsigned int)*(opLoc + 3) + ((unsigned int)*(opLoc + 4) << 8);

			if(size == 4) {
				val += seg << 4;
			}

			Operand* dst = new ImmediateOperand(val, size);


			sprintf(buf, "CALL %s", "");
			GETINST(preSize + 1 + dst->GetBytecodeLen());

			newCall = new Call(pre, buf, inst, (unsigned int)*opLoc);
			newCall->SetOperand(Operand::DST, dst);
			break;
		}
		case CALL_MOD16:
		//case CALL_IND16_16:
		{
			unsigned int size = (*opLoc) == CALL_MOD16 ? 2 : 4;
			unsigned int digit = (*(opLoc + 1) & 0x38) >> 3;
			if(digit != CALL_MOD_CONST && digit != CALL_IND_CONST)
				break;

			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);

			sprintf(buf, "CALL %s", "");
			GETINST(preSize + 2 + dst->GetBytecodeLen());

			newCall = new Call(pre, buf, inst, (unsigned int)*opLoc);
			newCall->SetOperand(Operand::DST, dst);
			break;
		}
	}
	return newCall;
}

int Call::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return -1;
	}

	switch(mOpcode) {

		case CALL_REL16:
			proc->PushRegister(REG_IP);
			if(dst->GetValue() >= 0x8000) {
				proc->SetRegister(REG_IP, proc->GetRegister(REG_IP) - (0x10000 - dst->GetValue()));
			} else {
				proc->SetRegister(REG_IP, proc->GetRegister(REG_IP) + dst->GetValue());
			}
			break;
		case CALL_ABS16_16:
			proc->PushRegister(REG_IP);
			proc->SetRegister(REG_IP, dst->GetValue());
			break;
		case CALL_MOD16:
		{
		//case CALL_IND16_16:
			proc->PushRegister(REG_IP);
			proc->PushRegister(REG_CS);

			unsigned int newIP = dst->GetValue();
			newIP = (newIP & 0xFFFF) + ((newIP & 0xFFFF0000) >> 0xC);
			proc->SetRegister(REG_IP, newIP & 0xFFFF);
			proc->SetRegister(REG_CS, (newIP & 0xFFFF0000) >> 0x10);
			break;
		}
		default:
			return -1;
	}

	return 0;
}

