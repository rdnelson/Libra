/*-------------------------------------*\
|
|  File Name: Xlat.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Xlat.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Xlat::Xlat(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Xlat::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newXlat = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case XLAT:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "XLAT");
			newXlat = new Xlat(pre, buf, inst, (int)*opLoc);
			newXlat->SetProc(mProc);
			break;
		}
	}

	return newXlat;

}

int Xlat::Execute() {
	unsigned int al = mProc->GetRegister(Processor8086::REG_AL);
	al = mProc->GetRegister(Processor8086::REG_BX) + al;
	al = mProc->GetMemory(al, 1);
	mProc->SetRegister(Processor8086::REG_AL, al);
	return 0;
}
