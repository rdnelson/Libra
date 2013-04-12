/*-------------------------------------*\
|
|  File Name: StiCli.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "StiCli.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

StiCli::StiCli(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* StiCli::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newStiCli = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case STI:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "STI");
			newStiCli = new StiCli(pre, buf, inst, (int)*opLoc);
			newStiCli->SetProc(mProc);
			break;
		}
		case CLI:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "CLI");
			newStiCli = new StiCli(pre, buf, inst, (int)*opLoc);
			newStiCli->SetProc(mProc);
			break;
		}
	}

	return newStiCli;

}

int StiCli::Execute() {

	if(mOpcode == STI) {
		mProc->SetFlag(Processor8086::FLAGS_IF, true);
	} else if(mOpcode == CLI) {
		mProc->SetFlag(Processor8086::FLAGS_IF, false);
	}
	return 0;
}
