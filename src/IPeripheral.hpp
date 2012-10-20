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
		virtual bool Put8(unsigned int port, unsigned int data) = 0;
		virtual bool Put16(unsigned int port, unsigned int data) = 0;

		virtual unsigned int Get8(unsigned int port) = 0;
		virtual unsigned int Get16(unsigned int port) = 0;

		virtual void Dump() = 0;

};

