/*-------------------------------------*\
|
|  File Name: Ret.cpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:59:11 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Ret.hpp"
#include "../ImmediateOperand.hpp"
#include "../Processor.hpp"

#include <cstdio>

Ret::Ret(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Ret::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	Instruction* newRet = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case RET_NEAR:
		case RET_FAR:
			GETINST(preSize + 1);
			snprintf(buf, 65, "RET");
			newRet = new Ret(pre, buf, inst, (unsigned int)*opLoc);
			break;
		case RET_NEAR_POP:
		case RET_FAR_POP:
		{
			unsigned int val = (unsigned int)*(opLoc + 1) + ((unsigned int)*(opLoc + 2) << 8);
			Operand* dst = new ImmediateOperand(val, 2, (opLoc + 1).getOffset());
			snprintf(buf, 65, "RET %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newRet = new Ret(pre, buf, inst, (unsigned int)*opLoc);
			newRet->SetOperand(Operand::DST, dst);
			break;
		}
	}
	return newRet;
}

int Ret::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];


	switch(mOpcode) {
		case RET_NEAR:
			proc->PopRegister(REG_IP);
			return RET_CALLED;
			break;
		case RET_FAR:
			proc->PopRegister(REG_IP);
			proc->PopRegister(REG_CS);
			return RET_CALLED;
			break;
		case RET_NEAR_POP:
			if(dst == 0) {
				return INVALID_ARGS;
			}
			proc->PopRegister(REG_IP);
			proc->PopSize(dst->GetValue());
			return RET_CALLED;
			break;
		case RET_FAR_POP:
			if(dst == 0) {
				return INVALID_ARGS;
			}
			proc->PopRegister(REG_IP);
			proc->PopRegister(REG_CS);
			proc->PopSize(dst->GetValue());
			return RET_CALLED;
			break;
	}
	return INVALID_ARGS;

}
