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

#include <cstring>
#include <cstdio>

#ifdef WIN32
#define snprintf sprintf_s
#endif

class Register {

	public:
        Register() : mVal(0) { strncpy(mHex, "0000", 7); }
        unsigned int GetValue() const { return mVal; }
        void SetValue(unsigned int newVal) { mVal = newVal & 0xFFFF; snprintf(mHex, 7, "%04X", mVal); }
        void AddValue(unsigned int addVal) { mVal += addVal; mVal &= 0xFFFF; snprintf(mHex, 7, "%04X", mVal); }
        const char* GetHex() const {
            return mHex;
        }

	protected:
        unsigned int mVal;
        char mHex[7];

};
