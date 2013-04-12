/*-------------------------------------*\
|
|  File Name: Cbw.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  6:15:25 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Cbw : public Instruction8086 {
	public: 
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			CBW = 0x98,
		};

	protected:
		Cbw(Prefix* pre, std::string text, std::string inst, int op);
};
