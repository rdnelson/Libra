/*-------------------------------------*\
|
|  File Name: Lahf.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Lahf.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Lahf::Lahf(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Lahf::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newLahf = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == LAHF) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "LAHF");

		newLahf = new Lahf(pre, buf, inst, (int)*opLoc);
		newLahf->SetProc(mProc);
	}

	return newLahf;

}

int Lahf::Execute() {

	unsigned int val = 0;
	val |= mProc->GetFlag(Processor8086::FLAGS_SF) ? 0x80 : 0;
	val |= mProc->GetFlag(Processor8086::FLAGS_ZF) ? 0x40 : 0;
	val |= mProc->GetFlag(Processor8086::FLAGS_AF) ? 0x10 : 0;
	val |= mProc->GetFlag(Processor8086::FLAGS_PF) ? 0x04 : 0;
	val |= 0x02;
	val |= mProc->GetFlag(Processor8086::FLAGS_CF) ? 0x01 : 0;
	mProc->SetRegister(Processor8086::REG_AH, val);
	return 0;
}
