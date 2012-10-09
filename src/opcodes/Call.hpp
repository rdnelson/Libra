/*-------------------------------------*\
|
|  File Name: Call.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tue, Oct  9, 2012 12:35:52 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Call : public Instruction {
	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			CALL_REL16	= 0xE8,
			CALL_MOD16	= 0xFF,
			CALL_ABS16_16	= 0x9A,
			CALL_IND16_16	= 0xFF,
		};

		static const unsigned int CALL_MOD_CONST = 0x02;
		static const unsigned int CALL_IND_CONST = 0x03;

	protected:
		Call(Prefix* pre, std::string text, std::string inst, int op);

};
