/*-------------------------------------*\
|
|  File Name: IncDec.hpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Fri, Oct 12, 2012  4:19:00 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class IncDec : public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			DEC_MOD8	= 0xFE,
			DEC_MOD16	= 0xFF,
			DEC_REG16	= 0x48,
			INC_MOD8	= 0xFE,
			INC_MOD16	= 0xFF,
			INC_REG16	= 0x40,
		};

		static const unsigned int DEC_SUB_OPCODE = 0x01;
		static const unsigned int INC_SUB_OPCODE = 0x00;

	protected:
		IncDec(Prefix* pre, std::string text, std::string inst, int op);

	private:
		enum eType {
			INC,
			DEC
		};
		eType mType;
};
