/*-------------------------------------*\
|
|  File Name: VirgoInstruction.hpp
|
|  Creation Date: 23-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Instruction.hpp"

class VirgoInstruction : public Instruction {
	public:
		VirgoInstruction(Prefix* pre, std::string text, std::string inst, int op) :
			Instruction(pre,text,inst,op) {}

		int Execute(Processor*) {
			return 0;
		}
};
