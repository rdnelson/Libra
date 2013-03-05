/*-------------------------------------*\
|
|  File Name: Timer.cpp
|
|  Creation Date: 07-02-2013
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "../IPeripheral.hpp"
#include "Timer.hpp"
#include <QTimer>

//initialize keyboard to polling mode
Timer::Timer(Processor* proc, QTimer* timer) : dataBuffer('\0'), ctrlBuffer(TIMER_CTRL_ENABLE), mProc(proc), mTimer(timer) {

}

bool Timer::Put8(unsigned int port, unsigned int data) {
	if(port == TIMER_CTRL_PORT) {
		//Can't write to the status port, so this is the command port
		//No commands yet though because this is supposed to be simplified
		ctrlBuffer = data & 0xFF;
		if((ctrlBuffer & TIMER_CTRL_ENABLE) == TIMER_CTRL_ENABLE) {
			if(mTimer) {
				mTimer->setInterval(TIMER_FREQ/dataBuffer);
				mTimer->start();
			}
		}
		return true;
	} else if (port == TIMER_DATA_PORT) {
		//Writing to the control port by default because no commands are implemented
		dataBuffer = data & 0xFF;
		return true;
	}
	return false;
}

bool Timer::Put16(unsigned int port, unsigned int data) {
	//split the write among the two ports
	if(port == TIMER_DATA_PORT) {
		dataBuffer = data & 0xFFFF;
		return true;
	} else if (port == TIMER_CTRL_PORT) {
		ctrlBuffer = data & 0xFF;
		if((ctrlBuffer & TIMER_CTRL_ENABLE) == TIMER_CTRL_ENABLE) {
			if(mTimer) {
				mTimer->setInterval(TIMER_FREQ/dataBuffer);
				mTimer->start();
			}
		}
	}
	return false;
}

unsigned int Timer::Get8(unsigned int port) {
	switch(port) {

	case TIMER_CTRL_PORT:
		return ctrlBuffer;
		break;
	case TIMER_DATA_PORT:
		return dataBuffer & 0xFF;
		break;
	}
	return 0xFFFFFFFF;
}

unsigned int Timer::Get16(unsigned int port) {
	switch(port) {
		case TIMER_CTRL_PORT:
			return ctrlBuffer;
			break;
		case TIMER_DATA_PORT:
			return dataBuffer & 0xFFFF;
			break;
		}
	return 0xFFFFFFFF;
}

void Timer::Dump() {
	std::cout << "Timer Interval: " << dataBuffer << std::endl;
	std::cout << "Timer Enabled:  " << (mTimer->isActive() ? "True" : "False") << std::endl;
}

void Timer::Update() {
	mProc->SetInterrupt(TIMER_IRQ);
	if(mTimer) {
		mTimer->setInterval(TIMER_FREQ/dataBuffer);
		mTimer->start();
	}
}
