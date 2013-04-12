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

#include "../Processor8086.hpp"
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

Instruction* Call::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newCall = 0;

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

			Operand* dst = new ImmediateOperand(val, size, (opLoc + 1).getOffset());


			snprintf(buf, 65, "CALL %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());

			newCall = new Call(pre, buf, inst, (unsigned int)*opLoc);
			newCall->SetProc(mProc);
			newCall->SetOperand(Operand::DST, dst);
			break;
		}
		case CALL_MOD16:
		//case CALL_IND16_16:
		{
			unsigned int digit = (*(opLoc + 1) & 0x38) >> 3;
			if(digit != CALL_MOD_CONST && digit != CALL_IND_CONST)
				break;

			Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, 2);

			snprintf(buf, 65, "CALL %s", dst->GetDisasm().c_str());
			GETINST(preSize + 2 + dst->GetBytecodeLen());

			newCall = new Call(pre, buf, inst, (unsigned int)*opLoc);
			newCall->SetProc(mProc);
			newCall->SetOperand(Operand::DST, dst);
			((Call*)newCall)->mType = digit;
			break;
		}
	}
	return newCall;
}

int Call::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}

	switch(mOpcode) {

		case CALL_REL16:
			mProc->PushRegister(Processor8086::REG_IP);
			if(dst->GetValue() >= 0x8000) {
				mProc->SetRegister(Processor8086::REG_IP, mProc->GetRegister(Processor8086::REG_IP) - (0x10000 - dst->GetValue()));
			} else {
				mProc->SetRegister(Processor8086::REG_IP, mProc->GetRegister(Processor8086::REG_IP) + dst->GetValue());
			}
			break;
		case CALL_ABS16_16:
			mProc->PushRegister(Processor8086::REG_IP);
			mProc->SetRegister(Processor8086::REG_IP, dst->GetValue());
			break;
		case CALL_MOD16:
		{
		//case CALL_IND16_16:

			if(mType == CALL_IND_CONST) {
				mProc->PushRegister(Processor8086::REG_CS);
				mProc->PushRegister(Processor8086::REG_IP);
				unsigned int newIP = dst->GetValue(4);
				newIP = (newIP & 0xFFFF) + ((newIP & 0xFFFF0000) >> 0xC);
				mProc->SetRegister(Processor8086::REG_IP, newIP & 0xFFFF);
				mProc->SetRegister(Processor8086::REG_CS, (newIP & 0xFFFF0000) >> 0x10);
			} else if(mType == CALL_MOD_CONST) {
				mProc->PushRegister(Processor8086::REG_IP);
				unsigned int newIP = dst->GetValue();
				mProc->SetRegister(Processor8086::REG_IP, newIP);
			}
			break;
		}
		default:
			return INVALID_ARGS;
	}

	return CALL_CALLED;
}
