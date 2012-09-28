/*-------------------------------------*\
|
|  File Name: Prefix.hpp
|
|  Creation Date: 27-09-2012
|
|  Last Modified: Thu, Sep 27, 2012  2:02:14 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

class Prefix {

	public:
		static Prefix* GetPrefix(unsigned char* memLoc);
		unsigned char GetGroup1() { return mG1; }
		unsigned char GetGroup2() { return mG2; }
		unsigned char GetGroup3() { return mG3; }
		unsigned char GetGroup4() { return mG4; }
		unsigned int GetLength() { return mLen; }

		enum eG1Prefixes {
			PRE_LOCK	= 0xF0,

			PRE_REPNE	= 0xF2,
			PRE_REP		= 0xF3,
		};

		enum eG2NoBranchPrefixes {
			PRE_CS_OVERRIDE	= 0x2E,
			PRE_SS_OVERRIDE	= 0x36,
			PRE_DS_OVERRIDE	= 0x3E,
			PRE_ES_OVERRIDE	= 0x26,
			PRE_FS_OVERRIDE	= 0x64,
			PRE_GS_OVERRIDE	= 0x65,
		};

		enum eG2BranchPrefixes {
			PRE_BRANCH_NOT_TAKEN	= 0x2E,
			PRE_BRANCH_TAKEN	= 0x3E,
		};

		enum eG3Prefixes {
			PRE_OP_SIZE	= 0x66,
		};

		enum eG4Prefixes {
			PRE_ADDR_SIZE	= 0x67,
		};

	protected:
		Prefix(unsigned char g1, unsigned char g2, unsigned char g3, unsigned char g4) :
			mG1(g1), mG2(g2), mG3(g3), mG4(g4), mLen(0) {

			if(mG1 != 0)
				mLen++;
			if(mG2 != 0)
				mLen++;
			if(mG3 != 0)
				mLen++;
			if(mG4 != 0)
				mLen++;
		}

		unsigned char mG1;
		unsigned char mG2;
		unsigned char mG3;
		unsigned char mG4;

		unsigned int mLen;
};
