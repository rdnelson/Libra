/*-------------------------------------*\
|
|  File Name: AddressOperand.cpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Mon, Oct  1, 2012 12:40:28 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "AddressOperand.hpp"
#include "Processor.hpp"

#include <cstring>

AddressOperand::AddressOperand(unsigned char* addr) : mAddr(addr) {

}

unsigned int AddressOperand::GetValue() {

	unsigned int tmp;
	memcpy(&tmp, mAddr, sizeof (unsigned int));
	return tmp;
}

void AddressOperand::SetValue(unsigned int val) {

	if(val & 0xFF000000) { //copy 16 bits
		memcpy(mAddr, &val, 2);
	} else {
		memcpy(mAddr, &val, 1);
	}
}

unsigned int AddressOperand::GetAddress(Processor* proc, unsigned char modrm, unsigned int disp) {

	unsigned int addr = 0;

	switch((modrm & 0xC0) >> 6) {
		case 0x00:
			if((modrm & 0x07) != 6)
				disp = 0;
			else
				return ((disp & 0xFFFF)) % 0x10000;
			break;
		case 0x01:
			disp &= 0xFF;
			break;
		case 0x02:
			disp &= 0xFFFF;
			break;
		case 0x03:
			return -1;
			break;

	}

	switch(modrm & 0x07) {
		case 0x00:
			addr = proc->GetRegister(REG_BX) + proc->GetRegister(REG_SI);
			break;
		case 0x01:
			addr = proc->GetRegister(REG_BX) + proc->GetRegister(REG_DI);
			break;
		case 0x02:
			addr = proc->GetRegister(REG_BP) + proc->GetRegister(REG_SI);
			break;
		case 0x03:
			addr = proc->GetRegister(REG_BP) + proc->GetRegister(REG_DI);
			break;
		case 0x04:
			addr = proc->GetRegister(REG_SI);
			break;
		case 0x05:
			addr = proc->GetRegister(REG_DI);
			break;
		case 0x06:
			addr = proc->GetRegister(REG_BP);
			break;
		case 0x07:
			addr = proc->GetRegister(REG_BX);
			break;
	}

	addr = (addr + disp) % 0x10000;
	return addr;
}
