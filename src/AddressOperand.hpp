/*-------------------------------------*\
|
|  File Name: Address.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Mon, Oct  1, 2012 12:39:38 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"

class Processor;

class AddressOperand : public Operand {

	public:
		AddressOperand(unsigned char* addr);

		void SetValue(unsigned int addr);
		unsigned int GetValue();

		static unsigned int GetAddress(Processor* proc, unsigned char modrm, unsigned int disp);

	private:
		unsigned char* mAddr;

};
