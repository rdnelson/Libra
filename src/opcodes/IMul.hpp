/*-------------------------------------*\
|
|  File Name: IMul.hpp
|
|  Creation Date: 17-10-2012
|
|  Last Modified: Wed, Oct 17, 2012 12:27:32 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class IMul: public Instruction8086 {

        public:
                static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
                int Execute();

                enum eValidOpcodes {
                        IMUL_MOD8	= 0xF6,
                        IMUL_MOD16	= 0xF7,
			IMUL_TWO_R16_MOD16	= 0xAF,
			IMUL_R16_MOD16_IMM8	= 0x6B,
			IMUL_R16_MOD16_IMM16	= 0x69,
                };

		static const unsigned int IMUL_SUB_OPCODE = 0x05;
		static const unsigned int TWO_BYTE_OPCODE = 0x0F;

        private:
                IMul(Prefix* pre, std::string text, std::string inst, int op);

};
