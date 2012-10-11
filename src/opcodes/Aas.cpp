/*-------------------------------------*\
|
|  File Name: Aas.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Aas.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Aas::Aas(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Aas::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newAas = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAS) {

		GETINST(preSize + 1);
		sprintf(buf, "AAS");

		newAas = new Aas(pre, buf, inst, (int)*opLoc);
	}

	return newAas;

}

int Aas::Execute(Processor* proc) {
	unsigned int val = proc->GetRegister(REG_AX);
	if((val & 0xF) > 9 || proc->GetFlag(FLAGS_AF)) {
		proc->SetRegister(REG_AX, val - 6);
		proc->SetRegister(REG_AH, proc->GetRegister(REG_AH) - 1);
		proc->SetFlag(FLAGS_AF, 1);
		proc->SetFlag(FLAGS_CF, 1);
		proc->SetRegister(REG_AL, proc->GetRegister(REG_AL) & 0xF);
	} else {
		proc->SetFlag(FLAGS_AF, 0);
		proc->SetFlag(FLAGS_CF, 0);
		proc->SetRegister(REG_AL, val & 0xF);
	}
	return 0;
}
