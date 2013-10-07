/*-------------------------------------*\
|
|  File Name: IPeripheral.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Tue, Sep 25, 2012  3:50:46 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <string>

class IPeripheral {

	public:
		IPeripheral() : mError(0) {}
		virtual ~IPeripheral() {}
		virtual bool Put8(unsigned int port, unsigned int data) = 0;
		virtual bool Put16(unsigned int port, unsigned int data) = 0;

		virtual unsigned int Get8(unsigned int port) = 0;
		virtual unsigned int Get16(unsigned int port) = 0;

		virtual void Dump() = 0;
		virtual void Update() {}

		virtual unsigned int GetType() = 0;
		virtual std::string GetStr() { return ""; }

		unsigned int GetError() { return mError; }

		enum ePeriphTypes {
			PERIPH_SCREEN,
			PERIPH_KEYBOARD,
			PERIPH_TIMER
		};

    static const unsigned int SCREEN_UNPRINTABLE = 0x00010001;

    protected:
        unsigned int mError;
};

