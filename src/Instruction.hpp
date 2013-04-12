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

#include "Memory.hpp"

class Processor;

class Instruction {

	public:
		virtual bool IsValid() const = 0;
		//returns the number of bytes in the instruction
		virtual size_t GetLength() const = 0;

		virtual void SetAddress(const unsigned int) = 0;
		virtual unsigned int GetAddress() const = 0;

		virtual std::string GetDisasm() const = 0;
		virtual std::string GetText() const = 0;



		virtual int Execute() = 0;

	public:
		enum eExecuteRetCode {
			INVALID_ARGS = -1,
			SUCCESS,
			RET_CALLED,
			CALL_CALLED,
			RES_BREAKPOINT,
			PERIPH_WRITE,
		};

};