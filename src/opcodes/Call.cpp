/*-------------------------------------*\
|
|  File Name: Call.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:54:25 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Call.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Call::Call(Prefix* pre, std::string text, std::string inst, int op) : mType(0) {

	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
}

Instruction* Call::CreateInstruction(Memory& memLoc, Processor* proc) {
	Memory opLoc = memLoc;
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


			snprintf(buf, 65, "CALL %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());

			newCall = new Call(pre, buf, inst, (unsigned int)*opLoc);
			newCall->SetOperand(Operand::DST, dst);
			break;
		}
		case CALL_MOD16:
		//case CALL_IND16_16:
		{
			unsigned int digit = (*(opLoc + 1) & 0x38) >> 3;
			if(digit != CALL_MOD_CONST && digit != CALL_IND_CONST)
				break;

			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);

			snprintf(buf, 65, "CALL %s", dst->GetDisasm().c_str());
			GETINST(preSize + 2 + dst->GetBytecodeLen());

			newCall = new Call(pre, buf, inst, (unsigned int)*opLoc);
			newCall->SetOperand(Operand::DST, dst);
			((Call*)newCall)->mType = digit;
			break;
		}
	}
	return newCall;
}

int Call::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
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

			if(mType == CALL_IND_CONST) {
				proc->PushRegister(REG_CS);
				proc->PushRegister(REG_IP);
				unsigned int newIP = dst->GetValue(4);
				newIP = (newIP & 0xFFFF) + ((newIP & 0xFFFF0000) >> 0xC);
				proc->SetRegister(REG_IP, newIP & 0xFFFF);
				proc->SetRegister(REG_CS, (newIP & 0xFFFF0000) >> 0x10);
			} else if(mType == CALL_MOD_CONST) {
				proc->PushRegister(REG_IP);
				unsigned int newIP = dst->GetValue();
				proc->SetRegister(REG_IP, newIP);
			}
			break;
		}
		default:
			return INVALID_ARGS;
	}

	return CALL_CALLED;
}
