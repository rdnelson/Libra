/*-------------------------------------*\
|
|  File Name: Immediate.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:06:47 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "Operand.hpp"
#include <sstream>
#include <iomanip>

#define min(a, b) ((a) < (b) ? (a) : (b))

class ImmediateOperand : public Operand {

	public:
		ImmediateOperand(unsigned int val, unsigned int size, unsigned int offset) : mValue(val), mSize(size) {
			std::stringstream ss;
            ss << "0x" << std::uppercase << std::setw(size * 2) << std::setfill('0') << std::hex << val;
			mText = ss.str();
			mOffset = offset;
		}


		unsigned int GetValue() { return mValue; }
		void SetValue(unsigned int val) {
			std::stringstream ss;
            ss << "0x" << std::uppercase << std::hex << std::setw(mSize * 2) << std::setfill('0') << val;
			mText = ss.str();
			mValue = val;
		}

		unsigned int GetBitmask() {
			unsigned int bm = 0;
			for(unsigned int i = 0; i < mSize; i++)
				bm = (bm << 8) | 0xFF;
			return bm;
		}
		unsigned int GetBytecodeLen() { return min(mSize, 2); }

		const std::string GetDisasm() {
			return mText;
		}

		int GetUnresolvedValue() const {
			return -mSize;
		}



	private:
		unsigned int mValue;
		unsigned int mSize;
};
