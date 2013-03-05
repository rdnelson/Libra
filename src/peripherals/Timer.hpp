/*-------------------------------------*\
|
|  File Name: Timer.hpp
|
|  Creation Date: 18-01-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../IPeripheral.hpp"
#include "../Processor.hpp"

#define TIMER_CTRL_ENABLE	0x06

#define TIMER_DATA_PORT		0x40
#define TIMER_CTRL_PORT		0x43

#define TIMER_IRQ		0x08
#define TIMER_FREQ		1193182

class QTimer;

class Timer : public IPeripheral {
	public:
		Timer(Processor* proc, QTimer* timer);

		bool Put8(unsigned int port, unsigned int data);
		bool Put16(unsigned int port, unsigned int data);

		unsigned int Get8(unsigned int port);
		unsigned int Get16(unsigned int port);

		virtual void Dump();

		unsigned int GetType() { return PERIPH_TIMER; }

		void Update();

	protected:

		unsigned int  dataBuffer;
		unsigned char ctrlBuffer;

		Processor* mProc;

		QTimer* mTimer;
};
