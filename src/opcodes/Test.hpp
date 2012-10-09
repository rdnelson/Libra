/*-------------------------------------*\
|
|  File Name: Test.hpp
|
|  Creation Date: 06-10-2012
|
|  Last Modified: Sat, Oct  6, 2012 10:33:39 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Test : public Instruction {

	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			TEST_AL_IMM8		= 0xA8,
			TEST_AX_IMM16		= 0xA9,
			TEST_MOD8_IMM8		= 0xF6,
			TEST_MOD16_IMM16	= 0xF7,
			TEST_MOD8_REG8		= 0x84,
			TEST_MOD16_REG16	= 0x85,
		};

		static const unsigned int TEST_SUB_OPCODE = 0x00;

	private:
		Test(Prefix* pre, std::string text, std::string inst, int op);

};
