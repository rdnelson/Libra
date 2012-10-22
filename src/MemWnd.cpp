#include "MemWnd.hpp"
#include "ui_MemWnd.h"
#include "QFileDialog.h"
#include "QScrollBar"
#include "QFontMetrics"
#include "QStringList"
#include "QStringListModel"
#include "VMWorker.hpp"
#include "QThread"

#include <iostream>

MemWnd::MemWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MemWnd)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint);
    QString s = "                                        \n";
    QString str = "";
    for(int i = 0; i < 20; i++) {
        str.append(s);
    }

    QFont fnt = ui->textBrowser->font();
    fnt.setPixelSize(15);
    ui->textBrowser->setFont(fnt);
    ui->textBrowser->setPlainText(str);

    QSize sz = ui->textBrowser->document()->size().toSize();
    ui->textBrowser->resize(sz.width() + 5, sz.height() * (ui->textBrowser->font().pixelSize() + 2.5));
    ui->textBrowser->setFixedSize(ui->textBrowser->size());
    std::cout << sz.width() << ", " << sz.height() * (ui->textBrowser->font().pixelSize() + 2.5) << std::endl;

    this->connect(this->ui->action_Load_Object_File, SIGNAL(triggered()),this, SLOT(loadObjFile()));
    this->connect(this->ui->actionRun, SIGNAL(triggered()), this, SLOT(runVM()));

    QStandardItemModel *model = new QStandardItemModel(0x1000, 0x10, this);


    for(int i = 0; i < 0x10; i++) {
        model->setHorizontalHeaderItem(i, new QStandardItem(QString("%1").arg(i, 0, 16).toUpper()));
    }

    for(int i = 0; i < 0x1000; i++) {

        model->setVerticalHeaderItem(i,new QStandardItem("0x" + QString("%1").arg(i * 0x10, 4, 16, QChar('0')).toUpper()));
        for(int j = 0; j < 0x10; j++) {
            model->setItem(i,j,new QStandardItem("FF"));
        }
    }

    int width = ui->tableView->fontMetrics().width('F') * 4;
    ui->tableView->setModel(model);
    for(int i = 0; i < 0x10; i++) {
        ui->tableView->setColumnWidth(i, width);
    }
    resize(ui->hrzAll->sizeHint().width() + 20,
           ui->hrzAll->sizeHint().height() + ui->menuBar->sizeHint().height() +
           ui->statusBar->sizeHint().height() + ui->mainToolBar->sizeHint().height() + 20);
    /*
    int width = 0;
    for(int i = 0; i < 0x10; i++) {
        width += ui->tableWidget->columnWidth(0);
    }
    width += 3 * ui->tableWidget->columnWidth(0);
    width += ui->tableWidget->verticalScrollBar()->sizeHint().width();
    ui->tableWidget->resize(width, ui->tableWidget->height());
*/
}

MemWnd::~MemWnd()
{
    delete ui;
}

void MemWnd::loadObjFile() {

    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Flat Binary (*.*);;Virgo Object (*.obj)"));
    mVM.LoadObjectFile(file.toStdString().c_str());
    if(mVM.isLoaded()) {
        ui->actionRun->setEnabled(true);
        mVM.Disassemble();
        QStringListModel *qListModel = new QStringListModel();
        qListModel->insertRows(0,mVM.GetNumInstructions());
        QStringList qList;
        for(int i = 0; i < mVM.GetNumInstructions(); i++) {
            qList << mVM.GetInstructionStr(i).c_str();
        }

        qListModel->setStringList(qList);
        ui->listView->setModel(qListModel);
    }
}

void MemWnd::runVM() {
    if(mVM.isLoaded()){
        this->ui->actionRun->setEnabled(false);
        QThread* thread = new QThread();
        VMWorker* worker = new VMWorker(&mVM);
        worker->moveToThread(thread);
        connect(worker, SIGNAL(breakpoint()), this, SLOT(vmBreakpoint()));
        connect(thread, SIGNAL(started()), worker, SLOT(run()));
        connect(worker, SIGNAL(breakpoint()), this, SLOT(vmBreakpoint()));
        connect(worker, SIGNAL(runDone()), this, SLOT(vmDone()));
        connect(this, SIGNAL(vmResume()), worker, SLOT(run()));
        connect(this, SIGNAL(vmStep()), worker, SLOT(step()));
        connect(worker, SIGNAL(stepDone()), this, SLOT(stepDone()));
        connect(worker, SIGNAL(exception()), thread, SLOT(quit()));
        connect(worker, SIGNAL(exception()), worker, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}

void MemWnd::vmBreakpoint() {

}

void MemWnd::vmDone() {
    this->setWindowTitle("VM Stopped");
}

void MemWnd::stepDone() {
    unsigned int numDevices = mVM.GetDevices().size();
    for(unsigned int i = 0; i < numDevices; i++) {
        //found a screen
        if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_SCREEN) {
            ui->textBrowser->setPlainText(mVM.GetDevices().at(i)->GetStr().c_str());
            ui->txtAX->setText(mVM.GetProc().GetRegisterHex(REG_AX));
            ui->txtBX->setText(mVM.GetProc().GetRegisterHex(REG_BX));
            ui->txtCX->setText(mVM.GetProc().GetRegisterHex(REG_CX));
            ui->txtDX->setText(mVM.GetProc().GetRegisterHex(REG_DX));
            ui->txtSI->setText(mVM.GetProc().GetRegisterHex(REG_SI));
            ui->txtDI->setText(mVM.GetProc().GetRegisterHex(REG_DI));
            ui->txtBP->setText(mVM.GetProc().GetRegisterHex(REG_BP));
            ui->txtSP->setText(mVM.GetProc().GetRegisterHex(REG_SP));
            ui->chkAdjust->setChecked(mVM.GetProc().GetFlag(FLAGS_AF));
            ui->chkOverflow->setChecked(mVM.GetProc().GetFlag(FLAGS_OF));
            ui->chkCarry->setChecked(mVM.GetProc().GetFlag(FLAGS_CF));
            ui->chkParity->setChecked(mVM.GetProc().GetFlag(FLAGS_PF));
            ui->chkZero->setChecked(mVM.GetProc().GetFlag(FLAGS_ZF));
            ui->chkSign->setChecked(mVM.GetProc().GetFlag(FLAGS_SF));
        }
    }

}
