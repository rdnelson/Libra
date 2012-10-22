#include "VMWorker.hpp"
#define EVER ;;

VMWorker::VMWorker(VM* vm) : mVM(vm)
{

}

void VMWorker::run() {
    if(mVM && mVM->isLoaded()) {
        for(EVER) {
            if(mVM->Step()) {
                break;
            }
            emit stepDone();
        }
        emit runDone();
    } else {
        emit exception();
    }
}

void VMWorker::step() {
    if(mVM && mVM->isLoaded()) {
        //mVM->Step();
        emit stepDone();
    } else {
        emit exception();
    }
}
