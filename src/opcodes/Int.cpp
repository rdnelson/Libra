/*-------------------------------------*\
|
|  File Name: Int.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Int.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Int::Int(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Int::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newInt = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case INT_3:
		{
			Operand* dst = new ImmediateOperand(3, 1, 0x10000);
			GETINST(preSize + 1);
			snprintf(buf, 65, "INT 3");
			newInt = new Int(pre, buf, inst, (int)*opLoc);
			newInt->SetProc(mProc);
			newInt->SetOperand(Operand::DST, dst);
			break;
		}
		case INT_IMM8:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			GETINST(preSize + 2);
			snprintf(buf, 65, "INT %s", dst->GetDisasm().c_str());

			newInt = new Int(pre, buf, inst, (int)*opLoc);
			newInt->SetProc(mProc);
			newInt->SetOperand(Operand::DST, dst);
			break;
		}
		case INTO:
		{
			Operand* dst = new ImmediateOperand(4,1,0x10000);
			GETINST(preSize + 1);
			snprintf(buf, 65, "INTO");
			newInt = new Int(pre, buf, inst, (int)*opLoc);
			newInt->SetProc(mProc);
			newInt->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newInt;

}

int Int::Execute() {
	Operand* dst = mOperands[Operand::DST];

	if(!dst) {
		return INVALID_ARGS;
	}

	if(mOpcode == INTO) {
		if(mProc->GetFlag(Processor8086::FLAGS_OF)) {
			mProc->PushRegister(Processor8086::REG_FLAGS);
			mProc->SetFlag(Processor8086::FLAGS_IF, false);
			mProc->SetFlag(Processor8086::FLAGS_TF, false);
			mProc->PushRegister(Processor8086::REG_IP);
			mProc->SetRegister(Processor8086::REG_IP, mProc->GetMemory((dst->GetValue() << 2), 2));
		}
		return 0;
	} else {
		mProc->PushRegister(Processor8086::REG_FLAGS);
		mProc->SetFlag(Processor8086::FLAGS_IF, false);
		mProc->SetFlag(Processor8086::FLAGS_TF, false);
		mProc->PushRegister(Processor8086::REG_IP);
		mProc->SetRegister(Processor8086::REG_IP, mProc->GetMemory((dst->GetValue() << 2), 2));
		return 0;
	}

	return INVALID_ARGS;
}
