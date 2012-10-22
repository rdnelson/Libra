#ifndef VMWORKER_H
#define VMWORKER_H

#include "QObject"
#include "VM.hpp"

class VMWorker : public QObject
{
    Q_OBJECT
public:
    VMWorker(VM* vm);

public slots:
    void step();
    void run();

signals:
    void exception();
    void breakpoint();
    void stepDone();
    void runDone();

private:
    VM* mVM;
};

#endif // VMTHREAD_H
