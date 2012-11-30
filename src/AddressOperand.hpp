/*-------------------------------------*\
|
|  File Name: AddressOperand.hpp
|
|  Creation Date: 04-10-2012
|
|  Last Modified: Thu, Oct  4, 2012  1:53:06 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "ModrmOperand.hpp"

class AddressOperand : public ModrmOperand {

	public:
		static Operand* GetAddressOperand(Processor* proc, Memory& addr, unsigned int size);

	private:
		AddressOperand(Processor* proc, Memory& addr, unsigned int size, unsigned int bytelen);


};
