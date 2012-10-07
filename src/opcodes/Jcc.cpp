/*-------------------------------------*\
|
|  File Name: Jcc.cpp
|
|  Creation Date: 06-10-2012
|
|  Last Modified: Sat, Oct  6, 2012  1:34:55 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Jcc.hpp"
#include "../Processor.hpp"
#include "../Instruction.hpp"
#include "../ImmediateOperand.hpp"
#include "../Prefix.hpp"

#include <cstdio>

#define GETINST(len) inst.insert(0, (char*)memLoc, len)

Jcc::Jcc(Prefix* pre, std::string text, std::string inst, int op) {
	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
}

Instruction* Jcc::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	unsigned int opcodeOffset = 0;

	Instruction* newJcc = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	unsigned int val = (int)*(opLoc + 1);
	if(*opLoc == TWO_BYTE_OPCODE) {
		opLoc++;
		opcodeOffset = TWO_BYTE_OFFSET;
		val += (int)*(opLoc + 2) << 8;
	}

	Operand* src = 0;

	switch(*opLoc - opcodeOffset) {

		case JA:
		case JAE:
		case JB:
		case JBE:
		case JCXZ:
		case JE:
		case JG:
		case JGE:
		case JL:
		case JLE:
		case JNE:
		case JNO:
		case JNP:
		case JNS:
		case JO:
		case JP:
		case JS:

			src = new ImmediateOperand(val, opcodeOffset == TWO_BYTE_OFFSET ? 2 : 1);
			newJcc = new Jcc(pre, buf, inst, (unsigned char)*opLoc - opcodeOffset);
			newJcc->SetOperand(Operand::SRC, src);
			break;

		default:
			break;

	}

	return newJcc;
}

int Jcc::Execute(Processor* proc) {
	bool jmp = false;

	switch(mOpcode) {
		case JA:
			jmp = !proc->GetFlag(FLAGS_CF) && !proc->GetFlag(FLAGS_ZF);
			break;
		case JAE:
			jmp = !proc->GetFlag(FLAGS_CF);
			break;
		case JB:
			jmp = proc->GetFlag(FLAGS_CF);
			break;
		case JBE:
			jmp = proc->GetFlag(FLAGS_CF) && proc->GetFlag(FLAGS_ZF);
			break;
		case JCXZ:
			jmp = proc->GetRegister(REG_CX) == 0;
			break;
		case JE:
			jmp = proc->GetFlag(FLAGS_ZF);
			break;
		case JG:
			jmp = !proc->GetFlag(FLAGS_ZF) && proc->GetFlag(FLAGS_SF) == proc->GetFlag(FLAGS_OF);
			break;
		case JGE:
			jmp = proc->GetFlag(FLAGS_SF) == proc->GetFlag(FLAGS_OF);
			break;
		case JL:
			jmp = proc->GetFlag(FLAGS_SF) != proc->GetFlag(FLAGS_OF);
			break;
		case JLE:
			jmp = proc->GetFlag(FLAGS_SF) != proc->GetFlag(FLAGS_OF) && proc->GetFlag(FLAGS_ZF);
			break;
		case JNE:
			jmp = !proc->GetFlag(FLAGS_ZF);
			break;
		case JNO:
			jmp = !proc->GetFlag(FLAGS_OF);
			break;
		case JNP:
			jmp = !proc->GetFlag(FLAGS_PF);
			break;
		case JNS:
			jmp = !proc->GetFlag(FLAGS_SF);
			break;
		case JO:
			jmp = proc->GetFlag(FLAGS_OF);
			break;
		case JP:
			jmp = proc->GetFlag(FLAGS_PF);
			break;
		case JS:
			jmp = proc->GetFlag(FLAGS_SF);

		default:
			break;		
	}

	if(jmp) {
		unsigned int newIP = proc->GetRegister(REG_IP);
		unsigned int relAddr = mOperands[Operand::SRC]->GetValue();
		if(relAddr < (mOperands[Operand::SRC]->GetBitmask() == 0xFF ? 0x80 : 0x8000))
			newIP += relAddr;
		else
			newIP -= relAddr;
		newIP &= 0xFFFF;
		proc->SetRegister(REG_IP, newIP);
	}

	return 0;
}
