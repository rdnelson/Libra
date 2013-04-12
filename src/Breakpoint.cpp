/*-------------------------------------*\
|
|  File Name: Breakpoint.cpp
|
|  Creation Date: 28-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Breakpoint.hpp"

Breakpoint::Breakpoint() : mIP(0xFFFFFFFF), mUseIP(false) {}

Breakpoint::Breakpoint(unsigned int IP) : mIP(IP & 0xFFFF), mUseIP(true) {}

void Breakpoint::AddRegisterWatch(unsigned int reg, unsigned int val) {

	Condition c;

	c.source.reg = reg;
	c.value.iVal = val;
	c.type = REGISTER_WATCH;
	c.ready = true;

	mConditions.push_back(c);

}

void Breakpoint::AddRegisterChanged(unsigned int reg) {

	Condition c;

	c.source.reg = reg;
	c.type = REGISTER_CHANGE;
	c.ready = false;

	mConditions.push_back(c);
}

void Breakpoint::AddFlagsWatch(unsigned int flag, bool value) {

	Condition c;

	c.source.flag = flag;
	c.type = FLAGS_WATCH;
	c.value.bVal = value;
	c.ready = true;

	mConditions.push_back(c);
}

void Breakpoint::AddFlagsChanged(unsigned int flag) {

	Condition c;

	c.source.flag = flag;
	c.type = FLAGS_CHANGE;
	c.ready = false;

	mConditions.push_back(c);
}

void Breakpoint::AddMemWatch(unsigned int addr, unsigned int val, unsigned int size) {

	Condition c;

	c.source.addr = addr;
	c.type = MEM_WATCH;
	c.value.iVal = val;
	c.size = size;
	c.ready = true;

	mConditions.push_back(c);
}

void Breakpoint::AddMemChanged(unsigned addr, unsigned int size) {

	Condition c;

	c.source.addr = addr;
	c.type = MEM_CHANGE;
	c.size = size;
	c.ready = false;

	mConditions.push_back(c);
}

bool Breakpoint::Evaluate(Processor* proc) {
	bool retVal = false;

	if(mUseIP) {
		//Line must match for any other conditions to matter
		if(proc->GetIP() != mIP) {
			return false;
		}
	}

	if(mConditions.size() == 0)
		return true;

	for(size_t i = 0; i < mConditions.size(); i++) {
		if(mConditions[i].ready == false) {
			switch(mConditions[i].type) {
				case REGISTER_CHANGE:
					mConditions[i].value.iVal = proc->GetRegister(mConditions[i].source.reg);
					break;
				case FLAGS_CHANGE:
					mConditions[i].value.bVal = proc->GetFlag(mConditions[i].source.flag);
					break;
				case MEM_CHANGE:
					mConditions[i].value.iVal = proc->GetMemory(mConditions[i].source.addr, mConditions[i].size);
					break;
				case REGISTER_WATCH:
				case FLAGS_WATCH:
				case MEM_WATCH:
					break;
			}
			mConditions[i].ready = true;
			continue;
		}

		//At this point line is matched, or doesn't matter
		switch(mConditions[i].type) {
			case REGISTER_WATCH:
				if(proc->GetRegister(mConditions[i].source.reg) == mConditions[i].value.iVal)
					retVal = true;
				break;
			case REGISTER_CHANGE:
				if(proc->GetRegister(mConditions[i].source.reg) != mConditions[i].value.iVal)
					retVal = true;
				break;
			case FLAGS_WATCH:
				if(proc->GetFlag(mConditions[i].source.flag) == mConditions[i].value.bVal)
					retVal = true;
				break;
			case FLAGS_CHANGE:
				if(proc->GetFlag(mConditions[i].source.flag) != mConditions[i].value.bVal)
					retVal = true;
				break;
			case MEM_WATCH:
				if(proc->GetMemory(mConditions[i].source.addr,
							mConditions[i].size) ==
							mConditions[i].value.iVal)
					retVal = true;
				break;
			case MEM_CHANGE:
				if(proc->GetMemory(mConditions[i].source.addr,
							mConditions[i].size) !=
							mConditions[i].value.iVal)
					retVal = true;
				break;
		}
		if (retVal)
			return true;
	}

	return false;

}
