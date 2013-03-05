/*-------------------------------------*\
|
|  File Name: Iret.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Iret.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Iret::Iret(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Iret::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newIret = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case IRET:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "IRET");
			newIret = new Iret(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newIret;

}

int Iret::Execute(Processor* proc) {

	proc->PopRegister(REG_IP);
	proc->PopRegister(REG_FLAGS);
	return 0;
}
