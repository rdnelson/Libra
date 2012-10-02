/*-------------------------------------*\
|
|  File Name: RegisterOperand.hpp
|
|  Creation Date: 01-10-2012
|
|  Last Modified: Mon, Oct  1, 2012 11:21:21 PM
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

		unsigned int GetBitmask();

	private:
		eRegisters mReg;
		Processor* mProc;

};
