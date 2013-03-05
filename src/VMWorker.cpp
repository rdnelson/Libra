#include "VMWorker.hpp"
#define EVER ;;

VMWorker::VMWorker(VM* vm) : mVM(vm), mPaused(false)
{

}

void VMWorker::run() {
	int err;
	int updateCtr = 0;
	mPaused = false;
	if(mVM && mVM->isLoaded()) {
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
			} else if(err > 0) {
			//	emit procReturn(err);
			}
			mVM->notifyReadCallbacks();
			mVM->notifyWriteCallbacks();
			//Only update the GUI every 300 instructions (~0.005 seconds)
			updateCtr = (updateCtr + 1) % 300;
			if(updateCtr == 0)
				emit stepDone();
			usleep(50);
		}
		emit error(err);
	} else {
		emit quit();
	}
}

void VMWorker::step() {
	if(mVM && mVM->isLoaded()) {
		//mVM->Step();
		emit stepDone();
	} else {
		emit quit();
	}
}

void VMWorker::pause() {
	if(mVM && mVM->isLoaded()) {
		mPaused = true;
	}
}
