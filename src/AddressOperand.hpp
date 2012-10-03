/*-------------------------------------*\
|
|  File Name: Address.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Wed, Oct  3, 2012 11:13:54 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"

class Processor;

enum eModRm {
	MOD,
	RM,
};

class AddressOperand : public Operand {

	public:
		enum eModRm {
			MOD,
			REG,
		};

		void SetValue(unsigned int addr);
		unsigned int GetValue();

		static Operand* GetAddressOperand(Processor* proc, unsigned char* inst, eModRm modrm, unsigned int size);

		unsigned int GetBitmask();
		unsigned int GetBytecodeLen();


	private:
		//Pass it the location of the opcode
		AddressOperand(Processor* proc, unsigned int addr, unsigned int size, unsigned int bytelen);

		static Operand* GetRegister(Processor* proc, unsigned int val, unsigned int size);
		unsigned int mAddr;
		unsigned int mSize;
		Processor* mProc;
		unsigned int mByteLen;
};
