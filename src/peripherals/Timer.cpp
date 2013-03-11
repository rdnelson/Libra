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
#include <QObject>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif __APPLE__
#include <mach/mach_time.h>
#else
//linux
#include <time.h>
#endif

//initialize keyboard to polling mode
Timer::Timer(Processor* proc) : dataBuffer('\0'), ctrlBuffer(TIMER_CTRL_ENABLE), mProc(proc), mTimeStart(0) {

}

bool Timer::Put8(unsigned int port, unsigned int data) {
	if(port == TIMER_CTRL_PORT) {
		//Can't write to the status port, so this is the command port
		//No commands yet though because this is supposed to be simplified
		ctrlBuffer = data & 0xFF;
		if((ctrlBuffer & TIMER_CTRL_ENABLE) == TIMER_CTRL_ENABLE) {
			timerBuffer=GetSystemTime()+dataBuffer;
			mTimeStart=GetSystemTime();
			qDebug("Enabling Timer (8). CurrentTime: %lu, targetTime: %lu", mTimeStart, timerBuffer);
		} else {
			//disable the timer
			timerBuffer = 0;
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
			timerBuffer=GetSystemTime()+dataBuffer;
			mTimeStart=GetSystemTime();
		} else {
			//disable the timer
			timerBuffer = 0;
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
	std::cout << "Timer Enabled:  " << ((mTimeStart < timerBuffer) ? "True" : "False") << std::endl;
}

void Timer::Update() {
	//qDebug("Updating Timer. CurrentTime: %lu, targetTime: %lu", GetSystemTime(), timerBuffer);
	//check if timer is enabled
	if(mTimeStart >= timerBuffer)
		return;
	if(GetSystemTime() > timerBuffer) {
		//Timer has ticked, reset it
		mProc->SetInterrupt(TIMER_IRQ);
		timerBuffer = GetSystemTime() + dataBuffer;
		mTimeStart = GetSystemTime();
	}
}

unsigned long Timer::GetSystemTime() {
#ifdef _WIN32
	return GetTickCount();
#elif __APPLE__
	unsigned long absTime = mach_absolute_time();
	Nanoseconds ns = AbsoluteToNanoseconds(*(AbsoluteTime*)&absTime);
	return *(unsigned long *)(&ns) / 1000000;
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_nsec / 1000000 + ts.tv_sec * 1000; //convert ns to ms
#endif
}
