/*-------------------------------------*\
|
|  File Name: Register.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Sep 27, 2012  1:17:54 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

class Register {

	public: 
		Register() : mVal(0) { }
		unsigned int GetValue() { return mVal; }
		void SetValue(unsigned int newVal) { mVal = newVal; }
		void AddValue(unsigned int addVal) { mVal += addVal; }

	protected:
		unsigned int mVal;		
};
