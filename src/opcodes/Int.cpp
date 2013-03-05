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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Int::Int(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Int::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newInt = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case INT_3:
		{
			Operand* dst = new ImmediateOperand(3, 1);
			GETINST(preSize + 1);
			snprintf(buf, 65, "INT 3");
			newInt = new Int(pre, buf, inst, (int)*opLoc);
			newInt->SetOperand(Operand::DST, dst);
			break;
		}
		case INT_IMM8:
		{
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1);
			GETINST(preSize + 2);
			snprintf(buf, 65, "INT %s", dst->GetDisasm().c_str());

			newInt = new Int(pre, buf, inst, (int)*opLoc);
			newInt->SetOperand(Operand::DST, dst);
			break;
		}
		case INTO:
		{
			Operand* dst = new ImmediateOperand(4,1);
			GETINST(preSize + 1);
			snprintf(buf, 65, "INTO");
			newInt = new Int(pre, buf, inst, (int)*opLoc);
			newInt->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newInt;

}

int Int::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];

	if(!dst) {
		return INVALID_ARGS;
	}

	if(mOpcode == INTO) {
		if(proc->GetFlag(FLAGS_OF)) {
			proc->PushRegister(REG_FLAGS);
			proc->SetFlag(FLAGS_IF, false);
			proc->SetFlag(FLAGS_TF, false);
			proc->PushRegister(REG_IP);
			proc->SetRegister(REG_IP, proc->GetMemory((dst->GetValue() << 2), 2));
		}
		return 0;
	} else {
		proc->PushRegister(REG_FLAGS);
		proc->SetFlag(FLAGS_IF, false);
		proc->SetFlag(FLAGS_TF, false);
		proc->PushRegister(REG_IP);
		proc->SetRegister(REG_IP, proc->GetMemory((dst->GetValue() << 2), 2));
		return 0;
	}

	return INVALID_ARGS;
}
