/*-------------------------------------*\
|
|  File Name: Immediate.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Wed, Oct 17, 2012 11:06:31 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"

class ImmediateOperand : public Operand {

	public:
		ImmediateOperand(unsigned int val, unsigned int size) : mValue(val), mSize(size) {}

		unsigned int GetValue() { return mValue; }
		void SetValue(unsigned int val) { mValue = val; }

		unsigned int GetBitmask() { 
			unsigned int bm = 0;
			for(unsigned int i = 0; i < mSize; i++)
				bm = (bm << 8) | 0xFF;
			return bm;
		}
		unsigned int GetBytecodeLen() { return mSize; }


	private:
		unsigned int mValue;
		unsigned int mSize;

};
