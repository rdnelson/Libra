/*-------------------------------------*\
|
|  File Name: main.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Tue, Sep 25, 2012  4:13:11 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "VM.hpp"

int main(int argc, char* argv[]) {

	VM virtualMachine(argc, argv);
	return virtualMachine.Run();

}
