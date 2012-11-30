/*-------------------------------------*\
|
|  File Name: Adc.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  5:46:47 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Processor;

class Adc: public Instruction {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			ADC_AL_BYTE	= 0x14,
			ADC_AX_WORD	= 0x15,
			ADC_MOD_REG8	= 0x10,
			ADC_MOD_REG16	= 0x11,
			ADC_REG8_MOD	= 0x12,
			ADC_REG16_MOD	= 0x13,
		};

		enum eValidGroup {
			GRP1_ADC_MOD_IMM8	= 0x80,
			GRP1_ADC_MOD_IMM16	= 0x81,
			GRP1_ADC_MOD_SIMM8	= 0x83,
		};

		static const unsigned int ADC_GRP_CONST = 0x02;


	private:
		Adc(Prefix* pre, std::string text, std::string inst, int op);

};

