/*-------------------------------------*\
|
|  File Name: Instruction.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Wed, Sep 26, 2012 10:16:17 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <string>
#include <vector>

class Processor;

class Instruction {

	public:
		static Instruction* ReadInstruction(char* memLoc);

		virtual int Execute(Processor& proc) = 0;

		inline bool IsValid() { return mValid; }

		inline int GetOpcode() { return mOpcode; }
		inline std::string GetInstruction() { return mInst; }
		inline unsigned int GetLength() { return mInst.size(); }


		//Opcode Prefixes grouped by their group numbers
		//One from each group is allowed in any order
		enum eG1Prefixes {
			//Group 1
			//Locks common memory in multi-core
			PRE_LOCK	= 0xF0,

			//only valid with: 
			//movs, cmps, scas, lods,
			//stos, ins, outs	
			PRE_REPNE	= 0xF2,
			PRE_REP		= 0xF3,
		};
		enum eG2NoBranchPrefixes {
			//Group 2 Branch use reserved
			PRE_CS_OVERRIDE	= 0x2E,
			PRE_SS_OVERRIDE	= 0x36,
			PRE_DS_OVERRIDE	= 0x3E,
			PRE_ES_OVERRIDE	= 0x26,
			PRE_FS_OVERRIDE	= 0x64,
			PRE_GS_OVERRIDE = 0x65,
		};
		enum eG2BranchPrefixes {
			//Group 2 Branch only
			
			//Hints to proc what the most likely path is
			PRE_BRANCH_NOT_TAKEN	= 0x2E,
			PRE_BRANCH_TAKEN	= 0x3E,
		};
		enum eG3Prefixes {
			//sets operand size to non-default (16 <-> 32 bit)
			PRE_OP_SIZE	= 0x66,
		};
		enum eG4Prefixes {
			//sets address size to non-default (16 <-> 32 bit)
			PRE_ADDR_SIZE	= 0x67,
		};


		typedef Instruction* (*PCreateInst)(char*);

	protected:
		Instruction();

		bool mValid;
		int mOpcode;
		std::string mInst;

		static std::vector<PCreateInst> AllInstructions;

};
