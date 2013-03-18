#include "VMWorker.hpp"
#include "peripherals\Timer.hpp"
#define EVER ;;

VMWorker::VMWorker(VM* vm) : mVM(vm), mPaused(false)
{

}

void VMWorker::run() {
	int err;
	mPaused = false;
	if(mVM && mVM->isLoaded()) {
		unsigned int guiTimeout = 0;
		for(EVER) {
			//Set by a different thread
			if(mPaused) {
				emit paused();
				return;
			}
			if((err = mVM->Step()) < 0) {
				break;
			} else if (err == VM::VM_BREAKPOINT) {
				emit procReturn(err);
				emit breakpoint();
				return;
			} else if(err == Instruction::PERIPH_WRITE) {
				if(guiTimeout++ % 600 == 0)
					emit procReturn(err);
			} else if (err == Processor::PROC_HALT) {
				if(!mVM->GetProc().GetFlag(FLAGS_IF)) {
					emit paused();
					return;
				}
			}
		}
		emit error(err);
	} else {
		emit quit();
	}
}

void VMWorker::pause() {
		mPaused = true;
}
