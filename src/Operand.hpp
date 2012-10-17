/*-------------------------------------*\
|
|  File Name: Operand.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Wed, Oct 17, 2012 10:02:31 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

class Operand {

	public:
		virtual void SetValue(unsigned int val) = 0;
		virtual unsigned int GetValue(unsigned int size) { return GetValue(); }
		virtual unsigned int GetValue() = 0;
		virtual unsigned int GetBitmask() = 0;
		virtual unsigned int GetBytecodeLen() = 0;

		unsigned int GetType() { return mType; }
		static const unsigned int DST	= 0;
		static const unsigned int SRC	= 1;
		static const unsigned int OP3	= 2;
		static const unsigned int OP4	= 3;

	protected:
		Operand();

		unsigned int mType;

};
