/*-------------------------------------*\
|
|  File Name: Rot.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:19:00 AM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Rot.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Rot::Rot(Prefix* pre, std::string text, std::string inst, int op, int modrm) : Instruction(pre,text,inst,op)
{
	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
	mModrm = modrm;
}

Instruction* Rot::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction* newRot = 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	unsigned char modrm = (*(opLoc + 1) & 0x38) >> 3;
	if (modrm >= 0 && modrm <= 4){
		switch(*opLoc) {
		case(ROT_MOD8_1):
		case(ROT_MOD16_1):
			{
				unsigned int size = (*opLoc == ROT_MOD8_1 ? 1 : 2);
				Operand* dst = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);

				snprintf(buf,65, "%s %s, 1", 
					modrm == ROL_MODRM ? "ROL" : 
						(modrm == ROR_MODRM ? "ROR" : 
						(modrm == RCL_MODRM ? "RCL" : "RCR")), 
					dst->GetDisasm().c_str());

				Operand* src = new ImmediateOperand(1, 1);
				GETINST(preSize + 2 + dst->GetBytecodeLen() + 0);

				newRot = new Rot(pre, buf, inst, (unsigned char)*opLoc, modrm);
				newRot->SetOperand(Operand::SRC, src);
				newRot->SetOperand(Operand::DST, dst);
				break;
			}
		case(ROT_MOD8_CL):
		case(ROT_MOD16_CL):
			{
				unsigned int size = (*opLoc == ROT_MOD8_CL ? 1 : 2);
				Operand* dst = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);
				
				snprintf(buf,65, "%s %s, CL", 
					modrm == ROL_MODRM ? "ROL" : 
						(modrm == ROR_MODRM ? "ROR" : 
						(modrm == RCL_MODRM ? "RCL" : "RCR")), 
					dst->GetDisasm().c_str());

				Operand* src = new RegisterOperand(REG_CL, proc);
				GETINST(preSize + 2 + dst->GetBytecodeLen());

				newRot = new Rot(pre, buf, inst, (unsigned char)*opLoc, modrm);
				newRot->SetOperand(Operand::SRC, src);
				newRot->SetOperand(Operand::DST, dst);
				break;
			}
		case(ROT_MOD8_IMM8):
		case(ROT_MOD16_IMM8):
			{
				unsigned int size = (*opLoc == ROT_MOD8_IMM8 ? 1 : 2);
				Operand* dst = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);
				unsigned int val = int(*(opLoc + 2 + dst->GetBytecodeLen())) & 0xFF;
				
				snprintf(buf,65, "%s %s, %d", 
					modrm == ROL_MODRM ? "ROL" : 
						(modrm == ROR_MODRM ? "ROR" : 
						(modrm == RCL_MODRM ? "RCL" : "RCR")), 
					dst->GetDisasm().c_str(), val);

				Operand* src = new ImmediateOperand(val, size);
				GETINST(preSize + 2 + dst->GetBytecodeLen() + 1);

				newRot = new Rot(pre, buf, inst, (unsigned char)*opLoc, modrm);
				newRot->SetOperand(Operand::SRC, src);
				newRot->SetOperand(Operand::DST, dst);
				break;
			}
		}
	}
	return newRot;
}

int Rot::Execute(Processor* proc) {

	unsigned int dstVal = mOperands[Operand::DST]->GetValue();
	unsigned int srcVal = mOperands[Operand::SRC]->GetValue();
	unsigned int newVal = 0;
	unsigned int bitMask = mOperands[Operand::DST]->GetBitmask();
	unsigned int sign =  bitMask == 0xFF ? 0x80 : 0x8000;
	unsigned int size = mOperands[Operand::DST]->GetBitmask() == 0xFF ? 8 : 16;
	unsigned int count;
	bool CF = proc->GetFlag(FLAGS_CF);
	bool tempCF = 0;
	bool OF = proc->GetFlag(FLAGS_OF);

	switch(mModrm) {
	case(RCL_MODRM):
		for (count = ((srcVal & 0x1F) % (size + 1)); count > 0; count--) {
			tempCF = (dstVal & sign) != 0;
			dstVal =  dstVal * 2 + CF;
			CF = tempCF;
		}
		if ((srcVal % (size * 8)) == 1) {
			OF = ((dstVal & sign) != 0) ^ CF;
		}
		break;
	case(RCR_MODRM):
		for (count = ((srcVal & 0x1F) % (size + 1)); count > 0; count--) {
			tempCF = (dstVal & 1) != 0;
			dstVal =  dstVal/2 + CF * (sign << 1);
			CF = tempCF;
		}
		if ((srcVal % (size * 8)) == 1) {
			OF = ((dstVal & sign) != 0) ^ CF;
		}
		break;
	case(ROL_MODRM):
		for (count = ((srcVal & 0x1F) % (size)); count > 0; count--) {
			tempCF = (dstVal & sign) != 0;
			dstVal =  dstVal*2 + tempCF;
		}
		if ((srcVal % (size * 8)) == 1) {
			OF = ((dstVal & sign) != 0) ^ CF;
		}
		break;
	case(ROR_MODRM):
		for (count = ((srcVal & 0x1F) % (size)); count > 0; count--) {
			tempCF = (dstVal & 1) != 0;
			dstVal =  dstVal/2 + tempCF * (sign);
			CF = tempCF;
		}
		if ((srcVal % (size * 8)) == 1) {
			OF = ((dstVal & sign) != 0) ^ ((dstVal & (sign >> 1)) != 0);
		}
		break;
	}

	proc->SetFlag(FLAGS_CF, CF);
	proc->SetFlag(FLAGS_OF, OF);
	mOperands[Operand::DST]->SetValue(dstVal & bitMask);
	return 0;
}