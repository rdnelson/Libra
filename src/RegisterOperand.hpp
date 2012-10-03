/*-------------------------------------*\
|
|  File Name: RegisterOperand.hpp
|
|  Creation Date: 01-10-2012
|
|  Last Modified: Wed, Oct  3, 2012 12:07:28 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"
#include "Processor.hpp"


class RegisterOperand : public Operand {

	public:
		RegisterOperand(eRegisters reg, Processor* proc);

		unsigned int GetValue();
		void SetValue(unsigned int val);
		unsigned int GetBytecodeLen() { return 0; }

		unsigned int GetBitmask();

	private:
		eRegisters mReg;
		Processor* mProc;
		unsigned int mSection;

};
