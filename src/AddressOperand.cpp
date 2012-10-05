/*-------------------------------------*\
|
|  File Name: AddressOperand.cpp
|
|  Creation Date: 04-10-2012
|
|  Last Modified: Thu, Oct  4, 2012  1:50:47 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/


#include "AddressOperand.hpp"

Operand* AddressOperand::GetAddressOperand(Processor* proc, unsigned int addr, unsigned int size) {
	return new AddressOperand(proc, addr, size, size);
}

AddressOperand::AddressOperand(Processor* proc, unsigned int addr, unsigned int size, unsigned int bytelen)
	: ModrmOperand(proc, addr, size, bytelen)
{

}
