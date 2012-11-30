/*-------------------------------------*\
|
|  File Name: Lxs.hpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:41:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Lxs : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			LDS = 0xC5,
			LES = 0xC4
		};

	protected:
		Lxs(Prefix* pre, std::string text, std::string inst, int op);
};
