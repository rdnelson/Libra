/*-------------------------------------*\
|
|  File Name: Prefix.cpp
|
|  Creation Date: 27-09-2012
|
|  Last Modified: Fri, Sep 28, 2012 10:39:06 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Prefix.hpp"


//Try to create a prefix from the current memory location
//If not valid prefix, return 0
Prefix* Prefix::GetPrefix(unsigned char* memLoc, size_t size) {
	Memory tmpMem(size, memLoc, 0);
	return Prefix::GetPrefix(tmpMem);
}
Prefix* Prefix::GetPrefix(Memory& memLoc) {

	char g1 = 0, g2 = 0, g3 = 0, g4 = 0;
	int offset = 0;

	//Maximum number of prefixes
	for(int i = 0; i < 4; i++) {
		switch(*(memLoc + offset)) {
			//Group 1 Prefixes
			case PRE_LOCK:
			case PRE_REPNE:
			case PRE_REP:
				if(g1 != 0) {
					return 0;
				}
				g1 = *(memLoc + offset++);
				break;

			//Group 2 Prefixes
			case PRE_CS_OVERRIDE:
			//case PRE_BRANCH_NOT_TAKEN: same as PRE_CS_OVERRIDE
			case PRE_SS_OVERRIDE:
			case PRE_DS_OVERRIDE:
			//case PRE_BRANCH_TAKEN: same as PRE_DS_OVERRIDE
			case PRE_ES_OVERRIDE:
			case PRE_FS_OVERRIDE:
			case PRE_GS_OVERRIDE:
				if(g2 != 0) {
					return 0;
				}
				g2 = *(memLoc + offset++);
				break;

			//Group 3 Prefixes
			case PRE_OP_SIZE:
				if(g3 != 0) {
					return 0;
				}
				g3 = *(memLoc + offset++);
				break;

			//Group 4 Prefixes
			case PRE_ADDR_SIZE:
				if(g4 != 0) {
					return 0;
				}
				g4 = *(memLoc + offset++);
				break;

			//Things that aren't prefixes
			default:
				//First Byte isn't a valid prefix
				if(offset == 0)
					return 0;
				//There's at least one byte of prefix
				else
					i = 4; //quits the for loop
		}
	}

	return new Prefix(g1, g2, g3, g4);

}
