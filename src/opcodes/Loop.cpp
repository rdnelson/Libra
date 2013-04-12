/*-------------------------------------*\
|
|  File Name: Loop.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Loop.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Loop::Loop(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Loop::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newLoop = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case LOOP:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			GETINST(preSize + 2);
			snprintf(buf, 65, "LOOP %s", dst->GetDisasm().c_str());
			newLoop = new Loop(pre, buf, inst, (int)*opLoc);
			newLoop->SetProc(mProc);
			newLoop->SetOperand(Operand::DST, dst);
			break;
		}
		case LOOPE:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			GETINST(preSize + 2);
			snprintf(buf, 65, "LOOPE %s", dst->GetDisasm().c_str());
			newLoop = new Loop(pre, buf, inst, (int)*opLoc);
			newLoop->SetProc(mProc);
			newLoop->SetOperand(Operand::DST, dst);
			break;
		}
		case LOOPNE:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			GETINST(preSize + 2);
			snprintf(buf, 65, "LOOPNE %s", dst->GetDisasm().c_str());
			newLoop = new Loop(pre, buf, inst, (int)*opLoc);
			newLoop->SetProc(mProc);
			newLoop->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newLoop;

}

int Loop::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}
	//Always have to decrement CX
	unsigned int cx = (mProc->GetRegister(Processor8086::REG_CX) - 1) & 0xFFFF;
	mProc->SetRegister(Processor8086::REG_CX, cx);

	//Check various conditions for not jumping, and return if they're met
	switch(mOpcode) {
		case LOOP:
			if(cx == 0) {
				return 0;
			}
		case LOOPE:
			if(cx == 0 || !mProc->GetFlag(Processor8086::FLAGS_ZF))
				return 0;
		case LOOPNE:
			if(cx == 0 || mProc->GetFlag(Processor8086::FLAGS_ZF))
				return 0;
	}

	//Jump has to happen
	unsigned int ip = mProc->GetRegister(Processor8086::REG_IP);
	if(dst->GetValue() >= 0x80)
		ip -= (~dst->GetValue() + 1) & 0xFF;
	else
		ip += dst->GetValue();

	mProc->SetRegister(Processor8086::REG_IP, ip);

	return 0;
}
