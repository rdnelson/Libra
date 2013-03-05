/*-------------------------------------*\
|
|  File Name: Keyboard.hpp
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

#define KBD_CTRL_INTERRUPT	0x01
#define KBD_CTRL_ENABLE		0x10

#define KBD_STAT_DATA_AVAIL	0x01

#define KBD_DATA_PORT		0x60
#define KBD_STAT_PORT		0x64

#define KBD_IRQ			0x09

class Keyboard : public IPeripheral {
	public:
		Keyboard(Processor* proc);

		bool Put8(unsigned int port, unsigned int data);
		bool Put16(unsigned int port, unsigned int data);

		unsigned int Get8(unsigned int port);
		unsigned int Get16(unsigned int port);

		virtual void Dump();

		unsigned int GetType() { return PERIPH_KEYBOARD; }

		void Update(char key, bool make);

	protected:

		unsigned char dataBuffer;
		unsigned char statBuffer;
		unsigned char ctrlBuffer;

		Processor* mProc;
};
