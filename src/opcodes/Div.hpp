/*-------------------------------------*\
|
|  File Name: Div.hpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Fri, Oct 12, 2012  5:14:35 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Div: public Instruction8086 {

        public:
                static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
                int Execute();

                enum eValidOpcodes {
                        DIV_MOD8	= 0xF6,
                        DIV_MOD16	= 0xF7,
                };

		static const unsigned int DIV_SUB_OPCODE = 0x06;
		static const unsigned int DIV_BY_ZERO = 0xDE;

        private:
                Div(Prefix* pre, std::string text, std::string inst, int op);

};
