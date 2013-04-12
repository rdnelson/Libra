#include "VMWorker.hpp"
#include "peripherals/Timer.hpp"
#include <fstream>
#define EVER ;;

VMWorker::VMWorker(VM* vm) : mVM(vm), mSteps(0), mRunConditions(NOT_RUNNING)
{

}

void VMWorker::StepN(unsigned int n) {
	if(mRunConditions == NOT_RUNNING) {
		mRunConditions = STEP_N;
		mSteps = n;
		start();
	}
}

void VMWorker::StepOver() {
	if(mRunConditions == NOT_RUNNING) {
		mRunConditions = STEP_OVER;
		start();
	}
}

void VMWorker::StepOut() {
	if(mRunConditions == NOT_RUNNING) {
		mRunConditions = STEP_OUT;
		start();
	}
}

void VMWorker::run() {
	int err;
	if(mRunConditions == NOT_RUNNING) {
		mRunConditions = FULL_RUN;
	}
	if(mVM && mVM->isLoaded()) {
		unsigned int start = Timer::GetSystemTime();
		unsigned int end = start;
		unsigned int instCount = 0;
		unsigned int callDepth = mVM->GetCallDepth();
		for(EVER) {
			instCount++;
			if((err = mVM->Step()) < 0) {
				break;
			} else if (err == VM::VM_BREAKPOINT) {
				emit procReturn(err);
				emit breakpoint();
				mRunConditions = NOT_RUNNING;
				return;
			} else if (err == Processor::PROC_HALT) {
				if(!mVM->GetProc().InterruptsEnabled()) {
					end = Timer::GetSystemTime();
					std::ofstream fout("time.log");
					fout << "Instructions: " << instCount << std::endl;
					fout << "Time (msec): " << end - start << std::endl;
					fout << "KIPS: " << (end - start == 0 ? 0 : instCount / (end - start)) << std::endl;
					fout.close();
					emit stopped();
					mRunConditions = NOT_RUNNING;
					return;
				}
			}
			switch(mRunConditions) {
			case FULL_RUN:
				break;
			case NOT_RUNNING:
				emit paused();
				return;
			case STEP_OVER:
				if(callDepth == mVM->GetCallDepth()) {
					emit procReturn(err);
					emit breakpoint();
					mRunConditions = NOT_RUNNING;
					return;
				}
				break;
			case STEP_OUT:
				if(callDepth == 0 || callDepth > mVM->GetCallDepth()) {
					emit procReturn(err);
					emit breakpoint();
					mRunConditions = NOT_RUNNING;
					return;
				}
				break;
			case STEP_N:
				if(instCount == mSteps) {
					emit procReturn(err);
					emit breakpoint();
					mRunConditions = NOT_RUNNING;
					return;
				}
				break;
			}
		}
		emit error(err);
	} else {
		emit quit();
	}
}

void VMWorker::pause() {
		mRunConditions = NOT_RUNNING;
}
