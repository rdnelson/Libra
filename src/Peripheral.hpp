/*-------------------------------------*\
|
|  File Name: Peripheral.hpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Tue, Sep 25, 2012  3:50:46 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

class Peripheral {

	public:
		Peripheral();

		virtual void	Out(int data);
		virtual int	In();

};

