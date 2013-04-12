/*-------------------------------------*\
|
|  File Name: Sahf.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Sahf.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Sahf::Sahf(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Sahf::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newSahf = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == SAHF) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "SAHF");

		newSahf = new Sahf(pre, buf, inst, (int)*opLoc);
		newSahf->SetProc(mProc);
	}

	return newSahf;

}

int Sahf::Execute() {

	unsigned int ah = mProc->GetRegister(Processor8086::REG_AH);
	mProc->SetFlag(Processor8086::FLAGS_SF, (ah & 0x80) != 0);
	mProc->SetFlag(Processor8086::FLAGS_ZF, (ah & 0x40) != 0);
	mProc->SetFlag(Processor8086::FLAGS_AF, (ah & 0x10) != 0);
	mProc->SetFlag(Processor8086::FLAGS_PF, (ah & 0x04) != 0);
	mProc->SetFlag(Processor8086::FLAGS_CF, (ah & 0x01) != 0);
	return 0;
}
