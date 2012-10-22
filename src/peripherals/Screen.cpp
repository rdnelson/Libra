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
#include <sstream>

Screen::Screen() : mCurX(0), mCurY(0), mFirstRow(0){
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

bool Screen::Put16(unsigned int port, unsigned int data) {
	switch(port) {
	case 0x04E8:
		return true;
	case 0x04E9:
	{
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
		return true;
	}
	case 0x4EA:
		mCurX = data & 0xFF;
		return true;
	case 0x4EB:
		mCurY = data & 0xFF;
		return true;
	}
	return false;
}

bool Screen::Put8(unsigned int port, unsigned int data) {
	return Put16(port, data & 0xFF);
}

unsigned int Screen::Get8(unsigned int port) {
	switch(port) {
	case 0x04E8: //Control
		return 0;
	case 0x04E9:
		return mScreen[mCurX][mCurY][0];
	case 0x04EA:
		return mCurX;
	case 0x04EB:
		return mCurY;
	}
	return 0xFFFFFFFF;
}

unsigned int Screen::Get16(unsigned int port) {
	switch(port) {
	case 0x04E8: //Control
		return 0;
	case 0x04E9:
		return mScreen[mCurX][mCurY][0] | (mScreen[mCurX][mCurY][1] << 8);
	case 0x04EA:
		return mCurX;
	case 0x04EB:
		return mCurY;
	}
	return 0xFFFFFFFF;

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

std::string Screen::GetStr() {
    std::stringstream ss;
    unsigned int row = mFirstRow;

    do {
        for(unsigned int col = 0; col < NUM_COLS; col++) {
            ss << mScreen[col][row][0];
        }
        ss << std::endl;
        row = (row + 1) % NUM_ROWS;
    } while(row != mFirstRow);
    return ss.str();
}
