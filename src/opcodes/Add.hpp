/*-------------------------------------*\
|
|  File Name: Add.hpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Wed, Sep 26, 2012 10:15:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Add: public Instruction {

	public:
		static Instruction* CreateInstruction(char* memLoc);
		int Execute(Processor& proc);

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
		Add();

};

