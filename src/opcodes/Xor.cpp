/*-------------------------------------*\
|
|  File Name: Xor.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:00:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Xor.hpp"
#include "../Prefix.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../ModrmOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdlib>
#include <cstdio>

Xor::Xor(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Xor::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	unsigned int preSize = 0;
	char buf[65];
	std::string inst;
	Instruction8086* newXor = 0;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case XOR_AL_IMM8:
		case XOR_AX_IMM16:
		{
			unsigned int size = (*opLoc) == XOR_AL_IMM8 ? 1 : 2;
			unsigned int val = (int)*(opLoc + 1);
			if(size == 2) {
				val += (int)*(opLoc + 2) << 8;
			}

			GETINST(preSize + 1 + size);

			Operand* dst = new RegisterOperand(size == 1 ? Processor8086::REG_AL : Processor8086::REG_AX, mProc);
			Operand* src = new ImmediateOperand(val, size, (opLoc + 2).getOffset());
			snprintf(buf, 65, "XOR %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
			newXor = new Xor(pre, buf, inst, (int)*opLoc);
			newXor->SetProc(mProc);
			newXor->SetOperand(Operand::SRC, src);
			newXor->SetOperand(Operand::DST, dst);
			break;
		}
		case XOR_MOD8_IMM8:
		case XOR_MOD16_IMM16:
		case XOR_MOD16_IMM8:
		{
			if((unsigned int)((*(opLoc + 1) & 0x38) >> 3) != XOR_REG_CONST)
				break;

			unsigned int size = *opLoc == XOR_MOD8_IMM8 ? 1 : 2;
			Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, size);

			unsigned int val = (int)*(opLoc + 2 +dst->GetBytecodeLen());
			if(size == 2) {
				if(*opLoc == XOR_MOD16_IMM16)
					val += (int)*(opLoc + 3 + dst->GetBytecodeLen()) << 8;
				else
					val += val >= 0x80 ? 0xFF00 : 0x0000;
			}

			Operand* src = new ImmediateOperand(val, size, (opLoc + 2 + dst->GetBytecodeLen()).getOffset());
			GETINST(preSize + 2 + (*opLoc == XOR_MOD16_IMM8 ? 1 : size) + dst->GetBytecodeLen());
			snprintf(buf, 65, "XOR %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());

			newXor = new Xor(pre, buf, inst, (int)*opLoc);
			newXor->SetProc(mProc);
			newXor->SetOperand(Operand::SRC, src);
			newXor->SetOperand(Operand::DST, dst);
			break;
		}
		case XOR_MOD8_REG8:
		case XOR_MOD16_REG16:
		case XOR_REG8_MOD8:
		case XOR_REG16_MOD16:
		{
			unsigned int size = (*opLoc == XOR_MOD8_REG8 || *opLoc == XOR_REG8_MOD8) ? 1 : 2;
			
			Operand* dst = ModrmOperand::GetModrmOperand(
					mProc,
					opLoc,
					(*opLoc == XOR_MOD8_REG8 || *opLoc == XOR_MOD16_REG16) ? 
						ModrmOperand::MOD : ModrmOperand::REG,
					size);
			Operand* src = ModrmOperand::GetModrmOperand(
					mProc,
					opLoc,
					(*opLoc == XOR_REG8_MOD8 || *opLoc == XOR_REG16_MOD16) ? 
						ModrmOperand::MOD : ModrmOperand::REG,
					size);

			GETINST(preSize + 2 + src->GetBytecodeLen() + dst->GetBytecodeLen());
			snprintf(buf, 65, "XOR %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
			newXor = new Xor(pre, buf, inst, (int)*opLoc);
			newXor->SetProc(mProc);
			newXor->SetOperand(Operand::SRC, src);
			newXor->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newXor;

}

int Xor::Execute() {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(dst == 0 || src == 0)
		return INVALID_ARGS;

	unsigned int val = dst->GetValue() ^ src->GetValue();

	mProc->SetFlag(Processor8086::FLAGS_OF, 0);
	mProc->SetFlag(Processor8086::FLAGS_CF, 0);

	mProc->SetFlag(Processor8086::FLAGS_ZF, val == 0);
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(val));
	mProc->SetFlag(Processor8086::FLAGS_SF, val >= (dst->GetBitmask() == 0xFF ? 0x80U : 0x8000U));

	dst->SetValue(val);

	return 0;
}
