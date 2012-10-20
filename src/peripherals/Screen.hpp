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

		bool Put8(unsigned int port, unsigned int data);
		bool Put16(unsigned int port, unsigned int data);

		unsigned int Get8(unsigned int port);
		unsigned int Get16(unsigned int port);

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
};
