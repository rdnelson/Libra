/*-------------------------------------*\
|
|  File Name: Aam.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tue, Oct  9, 2012  9:12:38 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Aam : public Instruction {
	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			AAM = 0xD4
		};

	protected:
		Aam(Prefix* pre, std::string text, std::string inst, int op);
};
