/*-------------------------------------*\
|
|  File Name: Keyboard.cpp
|
|  Creation Date: 07-02-2013
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "../IPeripheral.hpp"
#include "Keyboard.hpp"

//initialize keyboard to polling mode
Keyboard::Keyboard(Processor* proc) : dataBuffer('\0'), statBuffer('\0'), ctrlBuffer(KBD_CTRL_ENABLE), mProc(proc) {

}

bool Keyboard::Put8(unsigned int port, unsigned int data) {
	if(port == KBD_STAT_PORT) {
		//Can't write to the status port, so this is the command port
		//No commands yet though because this is supposed to be simplified
		return true;
	} else if (port == KBD_DATA_PORT) {
		//Writing to the control port by default because no commands are implemented
		ctrlBuffer = data & 0xFF;
		return true;
	}
	return false;
}

bool Keyboard::Put16(unsigned int port, unsigned int data) {
	//split the write among the two ports
	if(port == KBD_DATA_PORT) {
		Put8(port, data & 0xFF);
		Put8(port + 4, (data & 0xFF00) >> 8);
		return true;
	}
	return false;
}

unsigned int Keyboard::Get8(unsigned int port) {
	switch(port) {

	case KBD_STAT_PORT:
		return statBuffer;
		break;
	case KBD_DATA_PORT:
		if(statBuffer & KBD_STAT_DATA_AVAIL)
			statBuffer &= ~KBD_STAT_DATA_AVAIL;
		return dataBuffer;
		break;
	}
	return 0xFFFFFFFF;
}

unsigned int Keyboard::Get16(unsigned int port) {
	return Get8(port);
}

void Keyboard::Dump() {
/*	const char* const topline	= "| >`< | >1< | >2< | >3< | >4< | >5< | >6< | >7< | >8< | >9< | >0< | >-< | >=< |";
	const char* const midtopline	= "         | >q< | >w< | >e< | >r< | >t< | >y< | >u< | >i< | >o< | >p< | >[< | >]< | >\\< |";
	const char* const midbottomline	= "           | >a< | >s< | >d< | >f< | >g< | >h< | >j< | >k< | >l< | >;< | >'< |";
	const char* const bottomline	= "              | >z< | >x< | >c< | >v< | >b< | >n< | >m< | >,< | >.< | >/< |";*/

	//TODO: write a console representation of the keyboard.

}

#define FIRST_PRINTABLE_CHAR	' '
#define LAST_PRINTABLE_CHAR	'~'
#define ENTER_KEY 0x0A

void Keyboard::Update(char keyPress, bool) {
	//Only update if the keyboard is enabled
	if(!(ctrlBuffer & KBD_CTRL_ENABLE)) {
		return;
	}

	//Only update if there's no data waiting
	if(!(statBuffer & KBD_STAT_DATA_AVAIL)) {

		//Allow all visible keys and control keys
		if((keyPress >= FIRST_PRINTABLE_CHAR && keyPress <= LAST_PRINTABLE_CHAR) || keyPress == ENTER_KEY) {
			dataBuffer = (unsigned char)keyPress;
			statBuffer |= KBD_STAT_DATA_AVAIL;
		}

	}
	//Check whether to send an interrupt or not
	if(ctrlBuffer & KBD_CTRL_INTERRUPT) {
		mProc->SetInterrupt(KBD_IRQ);
	}
}
