/*-------------------------------------*\
|
|  File Name: RegisterOperand.hpp
|
|  Creation Date: 01-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:27:05 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"
#include "Processor8086.hpp"


class RegisterOperand : public Operand {

	public:
		RegisterOperand(Processor8086::eRegisters reg, Processor8086* proc);

		unsigned int GetValue();
		void SetValue(unsigned int val);
		unsigned int GetBytecodeLen() { return 0; }
		int GetUnresolvedValue() const;

		unsigned int GetBitmask();
		const std::string GetDisasm();

	private:
		Processor8086::eRegisters mReg;
		Processor* mProc;
		unsigned int mSection;

};
