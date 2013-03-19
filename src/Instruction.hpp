/*-------------------------------------*\
|
|  File Name: Instruction.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:30:26 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "Memory.hpp"
#include "Prefix.hpp"
#include "Operand.hpp"

//TODO: doesn't deal with memory wrap
#define GETINST(len) inst.insert(0, (char*)memLoc(), len)

#ifdef WIN32
#define snprintf sprintf_s
#endif

class Processor;

class Instruction {

	public:
		//Create an instruction from a memory location
		static Instruction* ReadInstruction(Memory::MemoryOffset& memLoc, Processor* proc);

		static Instruction* CreateSubcodeInstruction(Memory::MemoryOffset& memLoc, Processor*);

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
		inline size_t GetLength() { return mInst.size(); }

		//returns the reg/opcode field in modrm
		inline unsigned char GetRegOpcode() { return (modrm & 0x38) >> 3; }

		//returns the RM field in modrm
		inline unsigned char GetRM() { return modrm & 0x07; }

		//returns the Mod field in modrm
		inline unsigned char GetMod() { return (modrm & 0xC0) >> 6; }

		//Typedef for AllInstructions
		typedef Instruction* (*PCreateInst)(Memory::MemoryOffset&, Processor*);

		void SetOperand(const unsigned int operand, Operand* newOp);

		void SetAddress(const unsigned int addr) { mAddress = addr; }
		unsigned int GetAddress() const { return mAddress; }

		virtual ~Instruction();
		
		static bool Parity(unsigned int val);
		static bool OverflowAdd(unsigned int dst, unsigned int src, unsigned int size);
		static bool OverflowSub(unsigned int dst, unsigned int src, unsigned int size);
		static bool AdjustAdd(unsigned int op1, unsigned int op2);
		static bool AdjustSub(unsigned int op1, unsigned int op2);

		inline std::string GetDisasm() { return mText; }

		void AddLengthToDisasm() {
			std::stringstream ss;
			ss << mInst.size();
			mText += " (" + ss.str() + ")";
		}

		enum eExecuteRetCode {
			INVALID_ARGS = -1,
			SUCCESS,
			RET_CALLED,
			CALL_CALLED,
			RES_BREAKPOINT,
			PERIPH_WRITE,
		};


	protected:
		Instruction();
		Instruction(Prefix* pre, std::string text, std::string inst, int op);

		bool mValid;
		int mOpcode;
		std::string mInst;
		std::string mText;
		unsigned char modrm;

		Prefix* mPrefix;

		static PCreateInst AllInstructions[0x100][9];
		static std::map<unsigned int, PCreateInst> SubcodeMap;

		Operand* mOperands[4];
		unsigned int mAddress;

		static unsigned int NumOpcodes;

};
