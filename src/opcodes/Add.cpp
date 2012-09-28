/*-------------------------------------*\
|
|  File Name: Add.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Fri, Sep 28, 2012 11:10:52 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Add.hpp"
#include "../Prefix.hpp"
#include "../Processor.hpp"

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

Instruction* Add::CreateInstruction(unsigned char* memLoc) {

	unsigned char* opLoc = memLoc;
	int len = 0;
	char buf[65];
	int tInt1 = 0;

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

			newAdd = new Add(prefix, buf, inst, (int)*opLoc);

			break;
		case ADD_AX_WORD:
			tInt1 = (int)*(opLoc + 1);
			tInt1 |= (int)(*(opLoc + 2) << 8);
			sprintf(buf, "ADD AX, 0x%04X", tInt1);

			inst.insert(0, (char*)memLoc, len + 3);
			newAdd = new Add(prefix, buf, inst, (int)*opLoc);

			break;
		default:
			break;
	}

	return newAdd;

}

int Add::Execute(Processor* proc) {

	int tInt1 = 0;
	int tInt2 = 0;
	unsigned int parity = 0;

	//switch for the different opcodes
	switch(mOpcode) {
		case ADD_AL_BYTE:
			tInt1 = proc->GetRegisterLow(REG_AX) + mInst[1] ;

			//unsigned flag checks
			proc->SetFlag(FLAGS_CF, tInt1 > 0xFF);

			tInt1 &= 0xFF;

			//signed flag checks
			proc->SetFlag(FLAGS_OF, tInt1 > 0x80 && proc->GetRegisterLow(REG_AX) < 0x80);
			proc->SetFlag(FLAGS_SF, tInt1 > 0x80);

			//zero flag
			proc->SetFlag(FLAGS_ZF, tInt1 == 0);

			//Adjust Flag
			proc->SetFlag(FLAGS_AF, tInt1 & 0xF0);

			parity = tInt1;
			parity ^= parity >> 4;
			parity &= 0xf;
			proc->SetFlag(FLAGS_PF, (0x6996 >> parity) & 1);
			

			proc->SetRegisterLow(REG_AX, tInt1);
			break;

		case ADD_AX_WORD:
			tInt1 = proc->GetRegister(REG_AX) + mInst[1] + (mInst[2] << 8);

			proc->SetFlag(FLAGS_CF, tInt1 > 0xFFFF);
			tInt1 &= 0xFFFF;

			proc->SetFlag(FLAGS_OF, tInt1 > 0x8000 && proc->GetRegister(REG_AX) < 0x8000);
			proc->SetFlag(FLAGS_SF, tInt1 > 0x8000);

			proc->SetFlag(FLAGS_ZF, tInt1 == 0x0000);

			proc->SetFlag(FLAGS_AF, tInt1 & 0xF0);

			parity = tInt1;
			parity ^= parity >> 8;
			parity ^= parity >> 4;
			parity &= 0xf;
			proc->SetFlag(FLAGS_PF, (0x6996 >> parity) & 1);

			proc->SetRegister(REG_AX, tInt1);
			break;

		default:
			return -1;
	}


	return 0;
}


