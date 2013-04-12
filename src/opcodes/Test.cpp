/*-------------------------------------*\
|
|  File Name: Test.cpp
|
|  Creation Date: 06-10-2012
|
|  Last Modified: Fri, Oct 12, 2012  5:23:59 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Test.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../Processor8086.hpp"

#include <cstdio>

Test::Test(Prefix* pre, std::string text, std::string inst, int op) {
	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
}

Instruction* Test::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newTest = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case TEST_AL_IMM8:
		case TEST_AX_IMM16:
		{
			unsigned int size = (*opLoc == TEST_AL_IMM8 ? 1 : 2);

			unsigned int val = (int)*(opLoc + 1);
			if(size == 2) {
				val += (int)*(opLoc + 2) << 8;
			}

			GETINST(preSize + 1 + size);


			Operand* src = new ImmediateOperand(val, size, (opLoc + 1).getOffset());
			Operand* dst = new RegisterOperand(*opLoc == TEST_AL_IMM8 ? Processor8086::REG_AL : Processor8086::REG_AX, mProc);

			snprintf(buf, 65, "TEST %s, %s", size == 1 ? "AL" : "AH", src->GetDisasm().c_str());
			newTest = new Test(pre, buf, inst, (unsigned char)*opLoc);
			newTest->SetProc(mProc);
			newTest->SetOperand(Operand::SRC, src);
			newTest->SetOperand(Operand::DST, dst);
			break;
		}
		case TEST_MOD8_IMM8:
		case TEST_MOD16_IMM16:
		{
			if((unsigned int)((*(opLoc + 1) & 0x38) >> 3) != TEST_SUB_OPCODE)
				return 0;

			unsigned int size = (*opLoc == TEST_MOD8_IMM8 ? 1 : 2);

			unsigned int val = (int)*(opLoc + 1);
			if(size == 2) {
				val += (int)*(opLoc + 2) << 8;
			}


			Operand* src = new ImmediateOperand(val, size, (opLoc + 1).getOffset());
			Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, size);

			snprintf(buf, 65, "TEST %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
			GETINST(preSize + 2 + size + dst->GetBytecodeLen());
			newTest = new Test(pre, buf, inst, (unsigned char)*opLoc);
			newTest->SetProc(mProc);
			newTest->SetOperand(Operand::SRC, src);
			newTest->SetOperand(Operand::DST, dst);
			break;
		}
		case TEST_MOD8_REG8:
		case TEST_MOD16_REG16:
		{
			unsigned int size = (*opLoc == TEST_MOD8_REG8 ? 1 : 2);
			
			Operand* src = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::REG, size);
			Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, size);

			snprintf(buf, 65, "TEST %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
			GETINST(preSize + 2 + dst->GetBytecodeLen() + src->GetBytecodeLen());
			newTest = new Test(pre, buf, inst, (unsigned char)*opLoc);
			newTest->SetProc(mProc);
			newTest->SetOperand(Operand::SRC, src);
			newTest->SetOperand(Operand::DST, dst);
			break;
		}

	}

	return newTest;

}

int Test::Execute() {

	if(mOperands[Operand::SRC] == 0 || mOperands[Operand::DST] == 0)
		return INVALID_ARGS;

	unsigned int val = mOperands[Operand::SRC]->GetValue() & mOperands[Operand::DST]->GetValue();
	unsigned int sign = mOperands[Operand::DST]->GetBitmask() == 0xFF ? 0x80 : 0x8000;

	mProc->SetFlag(Processor8086::FLAGS_CF, false);
	mProc->SetFlag(Processor8086::FLAGS_OF, false);
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(val));
	mProc->SetFlag(Processor8086::FLAGS_ZF, val == 0);
	mProc->SetFlag(Processor8086::FLAGS_SF, val >= sign);

	return 0;
}
