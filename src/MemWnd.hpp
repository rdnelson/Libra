#ifndef MEMWND_HPP
#define MEMWND_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QAbstractTextDocumentLayout>
#include "VM.hpp"

namespace Ui {
class MemWnd;
}

class MemWnd : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MemWnd(QWidget *parent = 0);
    ~MemWnd();

public slots:
    void loadObjFile();
    void runVM();
    void vmBreakpoint();
    void vmDone();
    void stepDone();

signals:
    void vmResume();
    void vmStep();

private:
    Ui::MemWnd *ui;

    VM mVM;

};

#endif // MEMWND_HPP
