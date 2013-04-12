/*-------------------------------------*\
|
|  File Name: Jcc.hpp
|
|  Creation Date: 06-10-2012
|
|  Last Modified: Sat, Oct  6, 2012  1:31:13 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Jcc : public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			JA	= 0x77,
			JAE	= 0x73,
			JB	= 0x72,
			JBE	= 0x76,
			//JC	= 0x72,
			JCXZ	= 0xE3,
			JE	= 0x74,
			JG	= 0x7F,
			JGE	= 0x7D,
			JL	= 0x7C,
			JLE	= 0x7E,
			//JNA	= 0x76,
			//JNAE	= 0x72,
			//JNB	= 0x73,
			//JNBE	= 0x77,
			//JNC	= 0x73,
			JNE	= 0x75,
			//JNG	= 0x7E,
			//JNGE	= 0x7C,
			//JNL	= 0x7D,
			//JNLE	= 0x7F,
			JNO	= 0x71,
			JNP	= 0x7B,
			JNS	= 0x79,
			//JNZ	= 0x75,
			JO	= 0x70,
			JP	= 0x7A,
			//JPE	= 0x7A,
			//JPO	= 0x7B,
			JS	= 0x78,
			//JZ	= 0x74,
		};

		static const unsigned int TWO_BYTE_OPCODE = 0x0F;
		static const unsigned int TWO_BYTE_OFFSET = 0x10;


	private:
		static const char JA_STR[];
		static const char JB_STR[];
		static const char JE_STR[];
		static const char JG_STR[];
		static const char JL_STR[];
		static const char JNE_STR[];
		static const char JNO_STR[];
		static const char JNP_STR[];
		static const char JNS_STR[];
		static const char JO_STR[];
		static const char JP_STR[];
		static const char JS_STR[];
		static const char JAE_STR[];
		static const char JBE_STR[];
		static const char JCXZ_STR[];
		static const char JGE_STR[];
		static const char JLE_STR[];

		Jcc(Prefix* pre, std::string text, std::string inst, int op);

		static const char* _GetStr(unsigned int opcode);
		 
};	
