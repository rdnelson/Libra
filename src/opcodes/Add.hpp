/*-------------------------------------*\
|
|  File Name: Add.hpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Mon, Oct  1, 2012  9:54:26 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Processor;

class Add: public Instruction {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			ADD_AL_BYTE	= 0x04,
			ADD_AX_WORD	= 0x05,
			ADD_MOD_REG8	= 0x00,
			ADD_MOD_REG16	= 0x01,
			ADD_REG8_MOD	= 0x02,
			ADD_REG16_MOD	= 0x03,
		};

		enum eValidGroup {
			GRP1_ADD_MOD_IMM8	= 0x80,
			GRP1_ADD_MOD_IMM16	= 0x81,
			GRP1_ADD_MOD_SIMM8	= 0x83,
		};


	private:
		Add(Prefix* pre, std::string text, std::string inst, int op);

};

