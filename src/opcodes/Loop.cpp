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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Loop::Loop(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Loop::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newLoop = 0;

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
			newLoop->SetOperand(Operand::DST, dst);
			break;
		}
		case LOOPE:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			GETINST(preSize + 2);
			snprintf(buf, 65, "LOOPE %s", dst->GetDisasm().c_str());
			newLoop = new Loop(pre, buf, inst, (int)*opLoc);
			newLoop->SetOperand(Operand::DST, dst);
			break;
		}
		case LOOPNE:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			GETINST(preSize + 2);
			snprintf(buf, 65, "LOOPNE %s", dst->GetDisasm().c_str());
			newLoop = new Loop(pre, buf, inst, (int)*opLoc);
			newLoop->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newLoop;

}

int Loop::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}
	//Always have to decrement CX
	unsigned int cx = (proc->GetRegister(REG_CX) - 1) & 0xFFFF;
	proc->SetRegister(REG_CX, cx);

	//Check various conditions for not jumping, and return if they're met
	switch(mOpcode) {
		case LOOP:
			if(cx == 0) {
				return 0;
			}
            break;
		case LOOPE:
			if(cx == 0 || !proc->GetFlag(FLAGS_ZF))
				return 0;
            break;
		case LOOPNE:
			if(cx == 0 || proc->GetFlag(FLAGS_ZF))
				return 0;
            break;
	}

	//Jump has to happen
	unsigned int ip = proc->GetRegister(REG_IP);
	if(dst->GetValue() >= 0x80)
		ip -= (~dst->GetValue() + 1) & 0xFF;
	else
		ip += dst->GetValue();

	proc->SetRegister(REG_IP, ip);

	return 0;
}
