/*-------------------------------------*\
|
|  File Name: Lea.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Lea.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Lea::Lea(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Lea::CreateInstruction(Memory& memLoc, Processor* proc) {
	Memory opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newLea = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == LEA) {

		Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, 2);
		Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
		GETINST(preSize + 2 + src->GetBytecodeLen());
		snprintf(buf, 65, "LEA %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());

		newLea = new Lea(pre, buf, inst, (int)*opLoc);
		newLea->SetOperand(Operand::SRC, src);
		newLea->SetOperand(Operand::DST, dst);
	} 


	return newLea;

}

int Lea::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	ModrmOperand* src = (ModrmOperand*)mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	dst->SetValue(src->GetAddress().getOffset());

	return 0;
}
