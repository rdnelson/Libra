/*-------------------------------------*\
|
|  File Name: Instruction.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Mon, Oct  1, 2012  9:55:04 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <string>
#include <vector>

#include "Prefix.hpp"
#include "Operand.hpp"

class Processor;

class Instruction {

	public:
		//Create an instruction from a memory location
		static Instruction* ReadInstruction(unsigned char* memLoc, Processor* proc);

		//Register the mnemonics with the ReadInstruction function
		static void InitializeOpcodes();

		//Virtual instruction execute function
		virtual int Execute(Processor* proc) = 0;

		//False if the instruction is malformed
		inline bool IsValid() { return mValid; }

		//returns the opcode byte
		inline int GetOpcode() { return mOpcode; }

		//returns the full bytestream of the instruction
		inline std::string GetInstruction() { return mInst; }

		//returns the number of bytes in the instruction
		inline unsigned int GetLength() { return mInst.size(); }

		//returns the reg/opcode field in modrm
		inline unsigned char GetRegOpcode() { return (modrm & 0x38) >> 3; }

		//returns the RM field in modrm
		inline unsigned char GetRM() { return modrm & 0x07; }

		//returns the Mod field in modrm
		inline unsigned char GetMod() { return (modrm & 0xC0) >> 6; }

		//Typedef for AllInstructions
		typedef Instruction* (*PCreateInst)(unsigned char*, Processor*);

		void SetOperand(const unsigned int operand, Operand* newOp);

		virtual ~Instruction();

	protected:
		Instruction();

		bool mValid;
		int mOpcode;
		std::string mInst;
		std::string mText;
		unsigned char modrm;

		Prefix* mPrefix;

		static std::vector<PCreateInst> AllInstructions;

		Operand* mOperands[2];

};
