/*-------------------------------------*\
|
|  File Name: ModrmOperand.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Thu, Oct  4, 2012  1:47:14 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"

class Processor;

class ModrmOperand : public Operand {

	public:
		enum eModRm {
			MOD,
			REG,
			SEGREG,
		};

		void SetValue(unsigned int addr);
		unsigned int GetValue();

		static Operand* GetModrmOperand(Processor* proc, unsigned char* inst, eModRm modrm, unsigned int size);

		unsigned int GetBitmask();
		unsigned int GetBytecodeLen();


	protected:
		//Pass it the location of the opcode
		ModrmOperand(Processor* proc, unsigned int addr, unsigned int size, unsigned int bytelen);

		static Operand* GetRegister(Processor* proc, unsigned int val, unsigned int size);
		static Operand* GetSegRegister(Processor* proc, unsigned int val);
		unsigned int mAddr;
		unsigned int mSize;
		Processor* mProc;
		unsigned int mByteLen;
};
