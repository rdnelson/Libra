#include "VMWorker.hpp"
#include "peripherals/Timer.hpp"
#include <fstream>
#define EVER ;;

VMWorker::VMWorker(VM* vm) : mVM(vm), mPaused(false)
{

}

void VMWorker::run() {
	int err;
	mPaused = false;
	if(mVM && mVM->isLoaded()) {
		unsigned int start = Timer::GetSystemTime();
		unsigned int end = start;
		unsigned int instCount = 0;
		for(EVER) {
			//Set by a different thread
			if(mPaused) {
				emit paused();
				return;
			}
			instCount++;
			if((err = mVM->Step()) < 0) {
				break;
			} else if (err == VM::VM_BREAKPOINT) {
				emit procReturn(err);
				emit breakpoint();
				return;
			} else if (err == Processor::PROC_HALT) {
				if(!mVM->GetProc().GetFlag(FLAGS_IF)) {
					end = Timer::GetSystemTime();
					std::ofstream fout("time.log");
					fout << "Instructions: " << instCount << std::endl;
					fout << "Time (msec): " << end - start << std::endl;
					fout << "KIPS: " << instCount / (end - start) << std::endl;
					fout.close();
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
