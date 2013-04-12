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
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Iret::Iret(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Iret::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newIret = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case IRET:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "IRET");
			newIret = new Iret(pre, buf, inst, (int)*opLoc);
			newIret->SetProc(mProc);
			break;
		}
	}

	return newIret;

}

int Iret::Execute() {

	mProc->PopRegister(Processor8086::REG_IP);
	mProc->PopRegister(Processor8086::REG_FLAGS);
	return 0;
}
