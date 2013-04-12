/*-------------------------------------*\
|
|  File Name: IDiv.hpp
|
|  Creation Date: 16-10-2012
|
|  Last Modified: Tue, Oct 16, 2012  2:14:24 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class IDiv: public Instruction8086 {

        public:
                static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
                int Execute();

                enum eValidOpcodes {
                        IDIV_MOD8	= 0xF6,
                        IDIV_MOD16	= 0xF7,
                };

		static const unsigned int IDIV_SUB_OPCODE = 0x07;
		static const unsigned int IDIV_BY_ZERO = 0xDE;
		static const unsigned int IDIV_DIV_ERR = 0xDE;

        private:
                IDiv(Prefix* pre, std::string text, std::string inst, int op);

};
