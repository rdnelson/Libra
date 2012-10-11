/*-------------------------------------*\
|
|  File Name: Aad.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  1:58:35 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Aad : public Instruction {
	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			AAD = 0xD5
		};

	protected:
		Aad(Prefix* pre, std::string text, std::string inst, int op);
};
