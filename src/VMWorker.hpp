#ifndef VMWORKER_H
#define VMWORKER_H

#include "QObject"
#include "VM.hpp"

class VMWorker : public QObject
{
    Q_OBJECT
public:
    VMWorker(VM* vm);

    static const int VM_NOT_LOADED = 0xFF;

public slots:
    void step();
    void run();

signals:
    void quit();
    void error(int);
    void procReturn(int);
    void breakpoint();
    void stepDone();
    void runDone();

private:
    VM* mVM;
};

#endif // VMTHREAD_H
