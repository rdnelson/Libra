/*-------------------------------------*\
|
|  File Name: Aad.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:48:36 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Aad.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Aad::Aad(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Aad::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newAad = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAD) {

		unsigned int val = (int)*(opLoc + 1);
		Operand* dst = new ImmediateOperand(val, 1, (opLoc + 1).getOffset());

		GETINST(preSize + 1 + dst->GetBytecodeLen());
		snprintf(buf, 65, "AAD %s", dst->GetDisasm().c_str());

		newAad = new Aad(pre, buf, inst, (int)*opLoc);
		newAad->SetProc(mProc);
		newAad->SetOperand(Operand::DST, dst);
	}

	return newAad;

}

int Aad::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0) {
		return INVALID_ARGS;
	}

	unsigned int al = mProc->GetRegister(Processor8086::REG_AL);
	unsigned int ah = mProc->GetRegister(Processor8086::REG_AH);
	mProc->SetRegisterHigh(Processor8086::REG_AX, 0);
	mProc->SetRegisterLow(Processor8086::REG_AX, al = (al + (ah * dst->GetValue())) & 0xFF);
	mProc->SetFlag(Processor8086::FLAGS_SF, al >= 0x80);
	mProc->SetFlag(Processor8086::FLAGS_ZF, al == 0);
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(al));
	return 0;
}
