/*-------------------------------------*\
|
|  File Name: IDiv.cpp
|
|  Creation Date: 16-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:55:13 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "IDiv.hpp"

#include "../Processor8086.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

IDiv::IDiv(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* IDiv::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	Instruction8086* newIDiv = 0;

	if((*opLoc == IDIV_MOD8 || *opLoc == IDIV_MOD16) &&
			((unsigned int)((*(opLoc + 1) & 0x38) >> 3) == IDIV_SUB_OPCODE)) {
		unsigned int size = *opLoc == IDIV_MOD8 ? 1 : 2;
		Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, size);
		snprintf(buf, 65, "IDIV %s", dst->GetDisasm().c_str());
		GETINST(preLen + 2 + dst->GetBytecodeLen());
		newIDiv = new IDiv(pre, buf, inst, (int)*opLoc);
		newIDiv->SetProc(mProc);
		newIDiv->SetOperand(Operand::DST, dst);
	}
	return newIDiv;

}

int IDiv::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0)
		return INVALID_ARGS;

	if(dst->GetValue() == 0) {
		return IDIV_BY_ZERO;
	}

	unsigned int dividend = mProc->GetRegister(Processor8086::REG_AX) + (mOpcode == IDIV_MOD8 ? 0 :
	       	mProc->GetRegister(Processor8086::REG_DX) << 0x10);
	unsigned int divisor = dst->GetValue();
	unsigned int divisorNeg = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	unsigned int dividendNeg = dst->GetBitmask() == 0xFF ? 0x8000 : 0x80000000;
	unsigned int dividendBm = dst->GetBitmask() == 0xFF ? 0xFFFF : 0xFFFFFFFF;
	int rem, val;
	bool neg = (((dividend & dividendNeg) >> 8) ^ (divisor & divisorNeg)) != 0;
	//positivize everything
	if(dividend & dividendNeg) { //if dividend is negative
		dividend = (~dividend + 1) & dividendBm; // +ve it and ensure within bitmask
	}
	if(divisor & divisorNeg) {
		divisor = (~divisor + 1) & dst->GetBitmask(); //all operands are positive
		rem = -(int)(dividend % divisor); //remainder is negative
	} else {
		rem = (dividend % divisor); //remainder is positive
	}

	//calc result
	val = dividend / divisor; //
	if((unsigned int)val >= divisorNeg) { //operands are positive, so anything this big is overflow
		return IDIV_DIV_ERR;
	}
	//invert if necessary
	if(neg)
		val = -val;

	val &= dst->GetBitmask();

	mProc->SetRegister(mOpcode == IDIV_MOD8 ? Processor8086::REG_AL : Processor8086::REG_AX, val);
	mProc->SetRegister(mOpcode == IDIV_MOD8 ? Processor8086::REG_AH : Processor8086::REG_DX, rem);

	return 0;
}
