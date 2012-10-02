/*-------------------------------------*\
|
|  File Name: Operand.hpp
|
|  Creation Date: 28-09-2012
|
|  Last Modified: Mon, Oct  1, 2012 11:14:48 PM
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

		unsigned int GetType() { return mType; }
		static const unsigned int SRC = 0;
		static const unsigned int DST = 1;

	protected:
		Operand();

		unsigned int mType;

};
