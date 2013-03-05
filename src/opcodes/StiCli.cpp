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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

StiCli::StiCli(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* StiCli::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newStiCli = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case STI:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "STI");
			newStiCli = new StiCli(pre, buf, inst, (int)*opLoc);
			break;
		}
		case CLI:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "CLI");
			newStiCli = new StiCli(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newStiCli;

}

int StiCli::Execute(Processor* proc) {

	if(mOpcode == STI) {
		proc->SetFlag(FLAGS_IF, true);
	} else if(mOpcode == CLI) {
		proc->SetFlag(FLAGS_IF, false);
	}
	return 0;
}
