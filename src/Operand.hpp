/*-------------------------------------*\
|
|  File Name: Operand.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Tue, Oct  2, 2012  2:07:13 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

class Operand {

	public:
		virtual void SetValue(unsigned int val) = 0;
		virtual unsigned int GetValue() = 0;
		virtual unsigned int GetBitmask() = 0;
		virtual unsigned int GetBytecodeLen() = 0;

		unsigned int GetType() { return mType; }
		static const unsigned int SRC = 0;
		static const unsigned int DST = 1;

	protected:
		Operand();

		unsigned int mType;

};
