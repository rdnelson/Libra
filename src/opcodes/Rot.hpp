/*-------------------------------------*\
|
|  File Name: Rot.hpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  11:19:00 AM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Rot : public Instruction {

	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		int mModrm;

		enum eValidOpcodes {
			ROT_MOD8_1		= 0xD0,
			ROT_MOD16_1		= 0xD1,
			ROT_MOD8_CL		= 0xD2,
			ROT_MOD16_CL	= 0xD3,
			ROT_MOD8_IMM8	= 0xC0,
			ROT_MOD16_IMM8	= 0xC1,
		};

		enum eValidModrms {
			ROL_MODRM = 0x00,
			ROR_MODRM = 0x01,
			RCL_MODRM = 0x02,
			RCR_MODRM = 0x03,
		};

	protected:
		Rot(Prefix* pre, std::string text, std::string inst, int op, int modrm);
};
