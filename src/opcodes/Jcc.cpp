/*-------------------------------------*\
|
|  File Name: Jcc.cpp
|
|  Creation Date: 06-10-2012
|
|  Last Modified: Wed, Oct 17, 2012 11:20:24 AM
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

const char Jcc::JA_STR[] = "JA";
const char Jcc::JB_STR[] = "JB";
const char Jcc::JE_STR[] = "JE";
const char Jcc::JG_STR[] = "JG";
const char Jcc::JL_STR[] = "JL";
const char Jcc::JNE_STR[] = "JNE";
const char Jcc::JNO_STR[] = "JNO";
const char Jcc::JNP_STR[] = "JNP";
const char Jcc::JNS_STR[] = "JNS";
const char Jcc::JO_STR[] = "JO";
const char Jcc::JP_STR[] = "JP";
const char Jcc::JS_STR[] = "JS";
const char Jcc::JAE_STR[] = "JAE";
const char Jcc::JBE_STR[] = "JBE";
const char Jcc::JCXZ_STR[] = "JCXZ";
const char Jcc::JGE_STR[] = "JGE";
const char Jcc::JLE_STR[] = "JLE";


Jcc::Jcc(Prefix* pre, std::string text, std::string inst, int op) {
	mPrefix = pre;
	mText = text;
	mInst = inst;
	mOpcode = op;
	mValid = true;
}

Instruction* Jcc::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
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
		preSize++;
		opLoc++;
		opcodeOffset = TWO_BYTE_OFFSET;
		val = (int)*(opLoc + 1) + ((int)*(opLoc + 2) << 8);
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

			GETINST(preSize + 1 + (opcodeOffset == TWO_BYTE_OFFSET ? 2 : 1));
			src = new ImmediateOperand(val, opcodeOffset == TWO_BYTE_OFFSET ? 2 : 1, (opLoc + 1).getOffset());
			snprintf(buf, 65, "%s %s", _GetStr(*opLoc - opcodeOffset), src->GetDisasm().c_str());
			newJcc = new Jcc(pre, buf, inst, (unsigned char)*opLoc - opcodeOffset);
			newJcc->SetOperand(Operand::SRC, src);
			break;

		default:
			break;

	}

	return newJcc;
}

int Jcc::Execute(Processor* proc) {
	Operand* src = mOperands[Operand::SRC];

	if(!src) {
		return INVALID_ARGS;
	}

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
		unsigned int relAddr = src->GetValue();
		if (relAddr & 0x80)
			newIP -= 0xFF & (~relAddr + 1);
		else 
			newIP += relAddr;
		newIP &= 0xFFFF;
		proc->SetRegister(REG_IP, newIP);
	}

	return 0;
}

const char* Jcc::_GetStr(unsigned int opcode) {
	switch(opcode) {
		case JA:
			return JA_STR;
			break;
		case JAE:
			return JAE_STR;
			break;
		case JB:
			return JB_STR;
			break;
		case JBE:
			return JBE_STR;
			break;
		case JCXZ:
			return JCXZ_STR;
			break;
		case JE:
			return JE_STR;
			break;
		case JG:
			return JG_STR;
			break;
		case JGE:
			return JGE_STR;
			break;
		case JL:
			return JL_STR;
			break;
		case JLE:
			return JLE_STR;
			break;
		case JNE:
			return JNE_STR;
			break;
		case JNO:
			return JNO_STR;
			break;
		case JNP:
			return JNP_STR;
			break;
		case JNS:
			return JNS_STR;
			break;
		case JO:
			return JO_STR;
			break;
		case JP:
			return JP_STR;
			break;
		case JS:
			return JS_STR;
			break;
	}

	return 0;
}
