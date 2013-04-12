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
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Lea::Lea(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Lea::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newLea = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == LEA) {

		Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::REG, 2);
		Operand* src = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, 2);
		GETINST(preSize + 2 + src->GetBytecodeLen());
		snprintf(buf, 65, "LEA %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());

		newLea = new Lea(pre, buf, inst, (int)*opLoc);
		newLea->SetProc(mProc);
		newLea->SetOperand(Operand::SRC, src);
		newLea->SetOperand(Operand::DST, dst);
	} 


	return newLea;

}

int Lea::Execute() {
	Operand* dst = mOperands[Operand::DST];
	ModrmOperand* src = (ModrmOperand*)mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	dst->SetValue(src->GetAddress().getOffset());

	return 0;
}
