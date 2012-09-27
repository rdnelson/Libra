/*-------------------------------------*\
|
|  File Name: Processor.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Wed, Sep 26, 2012 10:47:33 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>

#include "Instruction.hpp"
#include "Register.hpp"

enum eRegisters {
	REG_AX,
	REG_BX,
	REG_CX,
	REG_DX,
	REG_SI,
	REG_DI,
	REG_BP,
	REG_SP,
	REG_CS,
	REG_DS,
	REG_SS,
	REG_ES,
	REG_IP,
	REG_FLAGS,
	NumRegisters,
};

enum eFlags {
	FLAGS_CF = 0,
	FLAGS_PF = 2,
	FLAGS_AF = 4,
	FLAGS_ZF = 6,
	FLAGS_SF = 7,
	FLAGS_TF = 8,
	FLAGS_IF = 9,
	FLAGS_DF = 10,
	FLAGS_OF = 11,
};
	

class Processor {

	public:
		int Initialize(unsigned int startAddr = 0x0000);
		Processor(char* mem);
		int Step();
		
	private:
		
		int Execute(Instruction* inst);
		bool GetFlag(eFlags flag);
		bool SetFlag(eFlags flag);

		Register	mRegisters[NumRegisters];
		char*		mMem;

};
