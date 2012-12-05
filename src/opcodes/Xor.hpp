/*-------------------------------------*\
|
|  File Name: Xor.hpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Wed, Oct 10, 2012 10:26:05 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Xor : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			XOR_AL_IMM8	= 0x34,
			XOR_AX_IMM16	= 0x35,
			XOR_MOD8_IMM8	= 0x80,
			XOR_MOD16_IMM16	= 0x81,
			XOR_MOD16_IMM8	= 0x83,
			XOR_MOD8_REG8	= 0x30,
			XOR_MOD16_REG16	= 0x31,
			XOR_REG8_MOD8	= 0x32,
			XOR_REG16_MOD16	= 0x33,
		};

		static const unsigned int XOR_REG_CONST = 0x06;

	protected:
		Xor(Prefix* pre, std::string text, std::string inst, int op);
};
