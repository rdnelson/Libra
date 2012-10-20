/*-------------------------------------*\
|
|  File Name: Screen.hpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../IPeripheral.hpp"

class Screen : public IPeripheral {
	public:
		Screen();

		void Put8(unsigned int data);
		void Put16(unsigned int data);

		unsigned int Get8();
		unsigned int Get16();

		unsigned int GetVirgoPort();
		unsigned int GetX86Port();

		virtual void Dump();

	protected:

		void _PutChar(char chr);
		void _PutColour(unsigned char colour);

		static const unsigned int NUM_ROWS = 25;
		static const unsigned int NUM_COLS = 40;

		//third index is for character data / color data
		char mScreen[NUM_COLS][NUM_ROWS][2];

		unsigned int mCurX, mCurY;
		unsigned int mFirstRow;

		static const unsigned int VirgoPorts[];
	       	/*= {
			0x04E8, //Control port
			0x04E9, //Data port
			0x04EA, //Cursor X
			0x04EB, //Cursor Y
			0xDEADBEEF, //Terminator
		};*/

		unsigned int mLastVirgoPort;

		static const unsigned int x86Ports[];
	       	/*= {
			0xDEADBEEF, //Terminator
		};*/

		unsigned int mLastx86Port;
};
