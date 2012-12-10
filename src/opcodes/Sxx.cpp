/*-------------------------------------*\
|
|  File Name: Sxx.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Sxx.hpp"

#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../Processor.hpp"

#include <cstdio>

Sxx::Sxx(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Sxx::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	int preLen = 0;	
	char buf[65];
	unsigned char subCode;
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);

	Sxx* newSxx = 0;

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	subCode = (*(opLoc + 1) & 0x38) >> 3;

	if(subCode != SAL_SUB_OPCODE && subCode != SAR_SUB_OPCODE && subCode != SHR_SUB_OPCODE)
		return newSxx;

	switch(*opLoc) {
		case SXX_MOD8_1:
		case SXX_MOD16_1:
		{
			unsigned int size = *opLoc == SXX_MOD8_1 ? 1 : 2;
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			snprintf(buf,65, "%s %s, 1", 
					subCode == SAL_SUB_OPCODE ? "SAL" : 
						(subCode == SAR_SUB_OPCODE ? "SAR" : "SHR"), 
					dst->GetDisasm().c_str());

			GETINST(preLen + 2 + dst->GetBytecodeLen());
			newSxx = new Sxx(pre, buf, inst, (int)*opLoc);
			newSxx->SetOperand(Operand::DST, dst);
			newSxx->SetOperand(Operand::SRC, new ImmediateOperand(1, 1));
			newSxx->mType = subCode;
			break;
		}
		case SXX_MOD8_CL:
		case SXX_MOD16_CL:
		{
			unsigned int size = *opLoc == SXX_MOD8_CL ? 1 : 2;
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			snprintf(buf,65, "%s %s, CL", 
					subCode == SAL_SUB_OPCODE ? "SAL" : 
						(subCode == SAR_SUB_OPCODE ? "SAR" : "SHR"), 
					dst->GetDisasm().c_str());

			GETINST(preLen + 2 + dst->GetBytecodeLen());
			newSxx = new Sxx(pre, buf, inst, (int)*opLoc);
			newSxx->SetOperand(Operand::DST, dst);
			newSxx->SetOperand(Operand::SRC, new RegisterOperand(REG_CL, proc));
			newSxx->mType = subCode;
			break;
		}
		case SXX_MOD8_IMM8:
		case SXX_MOD16_IMM8:
		{
			unsigned int size = *opLoc == SXX_MOD8_IMM8 ? 1 : 2;
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			unsigned int val = *(opLoc + 2 + dst->GetBytecodeLen());
			Operand* src = new ImmediateOperand(val, 1);
			snprintf(buf,65, "%s %s, %s", 
					subCode == SAL_SUB_OPCODE ? "SAL" : 
						(subCode == SAR_SUB_OPCODE ? "SAR" : "SHR"), 
					dst->GetDisasm().c_str(),
					src->GetDisasm().c_str());

			GETINST(preLen + 3 + dst->GetBytecodeLen());
			newSxx = new Sxx(pre, buf, inst, (int)*opLoc);
			newSxx->SetOperand(Operand::DST, dst);
			newSxx->SetOperand(Operand::SRC, src);
			newSxx->mType = subCode;
			break;
		}
	}

	return newSxx;
}

int Sxx::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src)
		return INVALID_ARGS;

	unsigned int tmpCount = src->GetValue() & 0x1F;
	unsigned int dstVal = dst->GetValue();
	unsigned int dstSign = dst->GetBitmask() == 0xFF ? 0x80 : 0x8000;
	bool cf;

	switch(mType) {
		case SAL_SUB_OPCODE:
		{
			while(tmpCount--) {
				cf = dstVal & dstSign;
				dstVal = (dstVal * 2) & dst->GetBitmask();
			}
			if((src->GetValue() & 0x1F) == 1) {
				proc->SetFlag(FLAGS_OF, dstVal ^ cf ? dstSign : 0x0000);
			}
			proc->SetFlag(FLAGS_CF, cf);
			break;
		}
		case SAR_SUB_OPCODE:
		{
			while(tmpCount--) {
				cf = dstVal & 0x01;
				dstVal = (dstVal / 2) | ((dstVal & dstSign) == dstSign ? dstSign : 0);
			}
			if((src->GetValue() & 0x1F) == 1) {
				proc->SetFlag(FLAGS_OF, 0);
			}
			proc->SetFlag(FLAGS_CF, cf);
			break;
		}
		case SHR_SUB_OPCODE:
		{
			while(tmpCount--) {
				cf = dstVal & 0x01;
				//ensure a zero is shifted in
				dstVal = (dstVal / 2) & (dst->GetBitmask() ^ dstSign);
			}
			if((src->GetValue() & 0x1F) == 1) {
				proc->SetFlag(FLAGS_OF, dst->GetValue() & dstSign);
			}
			proc->SetFlag(FLAGS_CF, cf);
			break;
		}
	}

	dst->SetValue(dstVal);
	if((src->GetValue() & 0x1F) != 0) {
		proc->SetFlag(FLAGS_ZF, dstVal == 0);
		proc->SetFlag(FLAGS_SF, (dstVal & dstSign) != 0);
		proc->SetFlag(FLAGS_PF, Parity(dstVal));
	}

	return 0;
}
