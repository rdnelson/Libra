/*-------------------------------------*\
|
|  File Name: Screen.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Screen.hpp"

#include <cstring>
#include <iostream>

const unsigned int Screen::VirgoPorts[] = {
	0x04E8, //Control
	0x04E9, //Data
	0x04EA, //Cursor X
	0x04EB, //Cursor Y
	0xDEADBEEF, //Terminator
};

const unsigned int Screen::x86Ports[] = {
	0xDEADBEEF,
};

Screen::Screen() : mCurX(0), mCurY(0), mFirstRow(0), mLastVirgoPort(0), mLastx86Port(0) {
	memset(mScreen, ' ', sizeof(mScreen));
}

void Screen::Dump() {
	unsigned int row = mFirstRow;

	std::cout << "++++++++++++++++++++++   SCREEN DUMP  ++++++++++++++++++++++" << std::endl;
	do {
		std::cout << "| ";
		for(unsigned int col = 0; col < NUM_COLS; col++) { 
			std::cout << mScreen[col][row][0];
		}
		std::cout << " |" << std::endl;

		row = (row + 1) % NUM_ROWS;
	} while(row != mFirstRow);
	std::cout << "++++++++++++++++++++   END SCREEN DUMP  ++++++++++++++++++++" << std::endl;
}

void Screen::Put16(unsigned int data) {
	char chr = data & 0xFF;
	unsigned char colour = (data & 0xFF00) >> 0x08;

	if(chr == 0x08) { //Backspace
		mCurX = mCurX == 0 ? 0 : mCurX - 1;
	} else if(chr == 0x09) { //Tab
		for(unsigned int i = 0; i < 4; i++) {
			_PutChar(' ');
			_PutColour(0);
		}
	} else if(chr == 0x0A) { //Line feed
		mCurY = (mCurY + 1) % NUM_ROWS;
		if(mCurY == mFirstRow) {
			mFirstRow = (mFirstRow + 1) % NUM_ROWS;
		}
		for(unsigned int i = 0; i < NUM_ROWS; i++) {
			mScreen[i][mCurY][0] = ' ';
			mScreen[i][mCurY][1] = 0;
		}
	} else if(chr == 0x0D) {
		mCurX = 0;
	} else if(chr >= 32 && chr < 127) {
		_PutColour(colour);
		_PutChar(chr);
	}
}

void Screen::Put8(unsigned int data) {
	Put16(data & 0xFF);
}

unsigned int Screen::Get8() {
	return mScreen[mCurX][mCurY][0];
}

unsigned int Screen::Get16() {
	return mScreen[mCurX][mCurY][0] | (mScreen[mCurX][mCurY][1] << 8);
}

void Screen::_PutChar(char chr) {
	mScreen[mCurX][mCurY][0] = chr;

	if(mCurX + 1 == NUM_COLS) {
		mCurY = (mCurY + 1) % NUM_ROWS;
		if(mCurY == mFirstRow) {
			mFirstRow = (mFirstRow + 1) % NUM_ROWS;
		}
		for(unsigned int i = 0; i < NUM_ROWS; i++) {
			mScreen[i][mCurY][0] = ' ';
			mScreen[i][mCurY][1] = 0;
		}
		mCurX = 0;
	} else {
		mCurX++;
	}
}

void Screen::_PutColour(unsigned char colour) {
	mScreen[mCurX][mCurY][1] = colour;
}

//Each call returns the next port number, or 0xDEADBEEF
unsigned int Screen::GetVirgoPort() {
	if(VirgoPorts[mLastVirgoPort] != 0xDEADBEEF) {
		return VirgoPorts[++mLastVirgoPort];
	}
	mLastVirgoPort = 0;
	return VirgoPorts[mLastVirgoPort];
}	

unsigned int Screen::GetX86Port() {
	if(x86Ports[mLastx86Port] != 0xDEADBEEF) {
		return x86Ports[++mLastx86Port];
	}
	return x86Ports[mLastx86Port = 0];
}
