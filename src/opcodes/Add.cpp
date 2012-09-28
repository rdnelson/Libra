/*-------------------------------------*\
|
|  File Name: Add.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Thu, Sep 27, 2012  6:13:16 PM
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

	Prefix* prefix = 0;

	Instruction* newAdd = 0;

	prefix = Prefix::GetPrefix(memLoc);

	if(prefix) {
		opLoc += prefix->GetLength();
		len += prefix->GetLength();
	}


	std::string inst;
	switch(*opLoc) {
		case ADD_AL_BYTE:
			sprintf(buf, "ADD AL, 0x%x", (int)*(opLoc + 1));

			inst.insert(0, (char*)memLoc, len + 2);	

			//create new add object
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

	switch(mOpcode) {
		case ADD_AL_BYTE:
			tInt1 = proc->GetRegisterLow(REG_AX) + mInst[1] ;

			//unsigned flag checks
			proc->SetFlag(FLAGS_CF, tInt1 > 0xFF);

			tInt1 &= 0xFF;

			//signed flag checks
			proc->SetFlag(FLAGS_OF, tInt1 > 0x80);
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
		default:
			return -1;
	}


	return 0;
}


