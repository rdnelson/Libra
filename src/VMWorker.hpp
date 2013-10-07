#ifndef VMWORKER_H
#define VMWORKER_H

#include "QObject"
#include "QThread"
#include "VM.hpp"

class VMWorker : public QThread
{
    Q_OBJECT
public:
    VMWorker(VM* vm);
    void StepN(unsigned int n);
    void StepOut();
    void StepOver();

    static const int VM_NOT_LOADED = 0xFF;

public slots:
    void run();
    void pause();

signals:
    void quit();
    void error(int);
    void procReturn(int);
    void breakpoint();
    void stepDone();
    void runDone();
    void paused();
    void stopped();
    void devError(IPeripheral*, unsigned int);

private:
    VM* mVM;
    unsigned int mSteps;
    unsigned int mRunConditions;

    enum {
        NOT_RUNNING,
        FULL_RUN,
        STEP_OUT,
        STEP_OVER,
        STEP_N,
    };
};

#endif // VMTHREAD_H
