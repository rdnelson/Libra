/*-------------------------------------*\
|
|  File Name: Xchg.hpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  7:58:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Xchg : public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			XCHG_REG_AX	= 0x90,
			XCHG_REG_CX	= 0x91,
			XCHG_REG_DX	= 0x92,
			XCHG_REG_BX	= 0x93,
			XCHG_REG_SP	= 0x94,
			XCHG_REG_BP	= 0x95,
			XCHG_REG_SI	= 0x96,
			XCHG_REG_DI	= 0x97,
			XCHG_MOD8_REG8	= 0x86,
			XCHG_MOD16_REG16 = 0x87,
		};

	private:
		Xchg(Prefix* pre, std::string text, std::string inst, int op);

};
