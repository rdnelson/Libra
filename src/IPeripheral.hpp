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

class IPeripheral {

	public:
		virtual void Put8(unsigned int data) = 0;
		virtual void Put16(unsigned int data) = 0;

		virtual unsigned int Get8() = 0;
		virtual unsigned int Get16() = 0;

		virtual unsigned int GetVirgoPort() = 0;
		virtual unsigned int GetX86Port() = 0;

		virtual void Dump() = 0;

};

