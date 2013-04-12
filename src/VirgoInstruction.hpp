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

#include "Instruction8086.hpp"
#include "Prefix.hpp"

class VirgoInstruction : public Instruction8086 {
	public:
		VirgoInstruction(Prefix* pre, std::string text, std::string inst, int op) :
			Instruction8086(pre,text,inst,op) {}

		int Execute() {
			return 0;
		}
};
