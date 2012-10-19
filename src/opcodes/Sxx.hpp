/*-------------------------------------*\
|
|  File Name: Sxx.hpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Sxx : public Instruction {
	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			SXX_MOD8_1	= 0xD0,
			SXX_MOD8_CL	= 0xD2,
			SXX_MOD8_IMM8	= 0xC0,
			SXX_MOD16_1	= 0xD1,
			SXX_MOD16_CL	= 0xD3,
			SXX_MOD16_IMM8	= 0xC1,
		};

		static const unsigned int SAL_SUB_OPCODE = 0x04;
		static const unsigned int SAR_SUB_OPCODE = 0x07;
		static const unsigned int SHR_SUB_OPCODE = 0x05;

	protected:
		Sxx(Prefix* pre, std::string text, std::string inst, int op);

		unsigned int mType;
};
