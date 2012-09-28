/*-------------------------------------*\
|
|  File Name: Instruction.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Sep 27, 2012  4:23:42 PM
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
		static Instruction* ReadInstruction(unsigned char* memLoc);
		static void InitializeOpcodes();

		virtual int Execute(Processor* proc) = 0;

		inline bool IsValid() { return mValid; }

		inline int GetOpcode() { return mOpcode; }
		inline std::string GetInstruction() { return mInst; }
		inline unsigned int GetLength() { return mInst.size(); }

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
