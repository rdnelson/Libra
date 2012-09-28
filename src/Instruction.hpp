/*-------------------------------------*\
|
|  File Name: Instruction.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Fri, Sep 28, 2012 10:37:33 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <string>
#include <vector>

#include "Prefix.hpp"

class Processor;

class Instruction {

	public:
		//Create an instruction from a memory location
		static Instruction* ReadInstruction(unsigned char* memLoc);

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

		//Typedef for AllInstructions
		typedef Instruction* (*PCreateInst)(unsigned char*);

	protected:
		Instruction();

		bool mValid;
		int mOpcode;
		std::string mInst;
		std::string mText;

		Prefix* mPrefix;

		static std::vector<PCreateInst> AllInstructions;

};
