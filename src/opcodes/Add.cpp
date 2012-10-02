/*-------------------------------------*\
|
|  File Name: Add.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Mon, Oct  1, 2012 11:32:10 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Add.hpp"
#include "../Prefix.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../AddressOperand.hpp"

#include <cstdlib>
#include <cstdio>

Add::Add(Prefix* pre, std::string text, std::string inst, int op) 
{
	mPrefix = pre;
       	mText = text;
       	mInst = inst;
       	mOpcode = op;
       	mValid = true;
}

Instruction* Add::CreateInstruction(unsigned char* memLoc, Processor* proc) {

	unsigned char* opLoc = memLoc;
	int len = 0;
	char buf[65];
	int tInt1 = 0;
	unsigned char modrm = 0;

	Prefix* prefix = 0;

	Instruction* newAdd = 0;

	//Build a prefix if possible
	prefix = Prefix::GetPrefix(memLoc);

	//Start looking after the prefix
	if(prefix) {
		opLoc += prefix->GetLength();
		len += prefix->GetLength();
	}


	std::string inst;
	//Switch for the different valid opcodes
	switch(*opLoc) {
		case ADD_AL_BYTE:
			sprintf(buf, "ADD AL, 0x%02X", (int)*(opLoc + 1));

			inst.insert(0, (char*)memLoc, len + 2);	

			newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);
			newAdd->SetOperand(Operand::SRC, new ImmediateOperand(*(opLoc + 1), 1));
			newAdd->SetOperand(Operand::DST, new RegisterOperand(REG_AL, proc));

			break;
		case ADD_AX_WORD:
			tInt1 = (unsigned char)*(opLoc + 1);
			tInt1 |= (((unsigned char)*(opLoc + 2)) << 8);
			sprintf(buf, "ADD AX, 0x%04X", tInt1);

			inst.insert(0, (char*)memLoc, len + 3);
			newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);

			break;

		case GRP1_ADD_MOD_IMM8:
			modrm = *(opLoc + 1);

			if(((modrm & 0x38) >> 3) == 0) {
				switch((modrm & 0xC0) >> 6) {
					case 0:
						break;
				}

				tInt1 = (int)*(opLoc+2);
				sprintf(buf, "ADD %s, 0x%02X", "", tInt1);

				inst.insert(0, (char*)memLoc, len + 3);
				newAdd = new Add(prefix, buf, inst, (unsigned char)*opLoc);

			}
			break;
		default:
			break;
	}

	return newAdd;

}

int Add::Execute(Processor* proc) {

	unsigned int parity = 0;

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	unsigned int newVal = dst->GetValue() + src->GetValue();
	proc->SetFlag(FLAGS_CF, newVal > dst->GetBitmask());
	newVal &= dst->GetBitmask();

	proc->SetFlag(FLAGS_OF, newVal >= 0x80 && dst->GetValue() < 0x80);
	proc->SetFlag(FLAGS_SF, newVal >= 0x80);
	proc->SetFlag(FLAGS_ZF, newVal == 0x00);
	proc->SetFlag(FLAGS_AF, (newVal & ~0x0F) != 0);

	parity = newVal;
	parity ^= parity >> 16;
	parity ^= parity >> 8;
	parity ^= parity >> 4;
	parity &= 0x0f;
	proc->SetFlag(FLAGS_PF, (0x6996 >> parity) & 1);

	dst->SetValue(newVal);

	return 0;
}


