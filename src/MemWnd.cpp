#include "MemWnd.hpp"
#include "ui_MemWnd.h"
#include "QFileDialog"
#include "QScrollBar"
#include "QFontMetrics"
#include "QStringList"
#include "QStringListModel"
#include "QListWidgetItem"
#include "QMessageBox"
#include "VMWorker.hpp"
#include "QThread"
#include "Instruction.hpp"
#include "QMemModel.hpp"
#include "Breakpoint.hpp"

#include <iostream>

MemWnd::MemWnd(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MemWnd),
	mFile("")
{
	ui->setupUi(this);
	//setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint);
	QString s = "                                        \n";
	QString str = "";
	for(int i = 0; i < 20; i++) {
		str.append(s);
	}

	//setup screen
	QFont fnt = ui->textBrowser->font();
	fnt.setPixelSize(15);
	ui->textBrowser->setFont(fnt);
	ui->textBrowser->setPlainText(str);

	QSize sz = ui->textBrowser->document()->size().toSize();
	ui->textBrowser->resize(sz.width() + 5, sz.height() * (ui->textBrowser->font().pixelSize() + 2.5));
	ui->textBrowser->setFixedSize(ui->textBrowser->size());


	//connect menubar actions
	this->connect(this->ui->action_Open, SIGNAL(triggered()),this, SLOT(loadObjFile()));
	this->connect(this->ui->actionRun, SIGNAL(triggered()), this, SLOT(runVM()));
	this->connect(this->ui->actionStep_Into, SIGNAL(triggered()), this, SLOT(stepInVM()));
	this->connect(this->ui->actionStep_Over, SIGNAL(triggered()), this, SLOT(stepOverVM()));
	this->connect(this->ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));


	//setup memory view
	/*QStandardItemModel *model = new QStandardItemModel(0x1000, 0x10, this);
	for(int i = 0; i < 0x10; i++) {
		model->setHorizontalHeaderItem(i, new QStandardItem(QString("%1").arg(i, 0, 16).toUpper()));
	}*/

	/*for(int i = 0; i < 0x1000; i++) {

		model->setVerticalHeaderItem(i,new QStandardItem("0x" + QString("%1").arg(i * 0x10, 4, 16, QChar('0')).toUpper()));
		for(int j = 0; j < 0x10; j++) {
			model->setItem(i,j,new QStandardItem("FF"));
		}
	}*/
	QMemModel* model = new QMemModel(this);
	model->copyData(mVM.GetMemPtr());

	int width = ui->tableView->fontMetrics().width('F') * 4;
	ui->tableView->setModel(model);
	for(int i = 0; i < 0x10; i++) {
		ui->tableView->setColumnWidth(i, width);
	}
	ui->tableView->setMaximumWidth(width * 0x13);

}

MemWnd::~MemWnd()
{
	delete ui;
}

void MemWnd::loadObjFile() {

	QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Flat Binary (*.*);;Virgo Object (*.obj)"));
	mFile = file;
	mVM.LoadVirgoFile(file.toStdString().c_str());
	if(mVM.isLoaded()) {
		ui->actionRun->setEnabled(true);
		ui->actionStep_Into->setEnabled(true);
		ui->actionStep_Over->setEnabled(true);
		ui->actionStep_Out->setEnabled(true);

		UpdateMemView();

		ui->lstInstructions->clear();
		mVM.Disassemble();
		for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
			ui->lstInstructions->addItem(mVM.GetInstructionStr(i).c_str());
			ui->lstInstructions->item(i)->setFlags(ui->lstInstructions->item(i)->flags() | Qt::ItemIsUserCheckable);
			ui->lstInstructions->item(i)->setCheckState(Qt::Unchecked);
		}
		UpdateGui();
	}
}

void MemWnd::reloadObjFile() {
	if(mFile != "") {
		mVM.LoadFlatFile(mFile.toStdString().c_str());
		if(mVM.isLoaded()) {
			ui->actionRun->setEnabled(true);
			ui->actionStep_Into->setEnabled(true);
			ui->actionStep_Over->setEnabled(true);
			ui->actionStep_Out->setEnabled(true);

			ui->lstInstructions->clear();
			mVM.Disassemble();
			for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
				ui->lstInstructions->addItem(mVM.GetInstructionStr(i).c_str());
			}
		}
	}
}

void MemWnd::runVM() {
	if(mVM.isLoaded()){
		DisableRun(0);
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
		connect(worker, SIGNAL(quit()), thread, SLOT(quit()));
		connect(worker, SIGNAL(quit()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(worker, SIGNAL(error(int)), this, SLOT(vmRunError(int)));
		thread->start();
	}
}

void MemWnd::vmBreakpoint() {

	EnableRun();
	UpdateGui();

}

void MemWnd::vmDone() {
	this->setWindowTitle("VM Stopped");
	UpdateGui();

}

void MemWnd::vmRunError(int err) {
	UpdateGui();
	DisableRun(err);
}

void MemWnd::stepDone() {
	UpdateGui();
}

void MemWnd::UpdateGui() {
	unsigned int numDevices = mVM.GetDevices().size();
	for(unsigned int i = 0; i < numDevices; i++) {
		//found a screen
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_SCREEN) {
			ui->textBrowser->setPlainText(mVM.GetDevices().at(i)->GetStr().c_str());
		}
	}
	ui->txtAX->setText(mVM.GetProc().GetRegisterHex(REG_AX));
	ui->txtBX->setText(mVM.GetProc().GetRegisterHex(REG_BX));
	ui->txtCX->setText(mVM.GetProc().GetRegisterHex(REG_CX));
	ui->txtDX->setText(mVM.GetProc().GetRegisterHex(REG_DX));
	ui->txtSI->setText(mVM.GetProc().GetRegisterHex(REG_SI));
	ui->txtDI->setText(mVM.GetProc().GetRegisterHex(REG_DI));
	ui->txtBP->setText(mVM.GetProc().GetRegisterHex(REG_BP));
	ui->txtSP->setText(mVM.GetProc().GetRegisterHex(REG_SP));
	ui->txtIP->setText(mVM.GetProc().GetRegisterHex(REG_IP));
	ui->txtFLAGS->setText(mVM.GetProc().GetRegisterHex(REG_FLAGS));
	ui->chkAdjust->setChecked(mVM.GetProc().GetFlag(FLAGS_AF));
	ui->chkOverflow->setChecked(mVM.GetProc().GetFlag(FLAGS_OF));
	ui->chkCarry->setChecked(mVM.GetProc().GetFlag(FLAGS_CF));
	ui->chkParity->setChecked(mVM.GetProc().GetFlag(FLAGS_PF));
	ui->chkZero->setChecked(mVM.GetProc().GetFlag(FLAGS_ZF));
	ui->chkSign->setChecked(mVM.GetProc().GetFlag(FLAGS_SF));
	UpdateMemView();

	unsigned int ip = mVM.GetProc().GetRegister(REG_IP);
	for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
		if(mVM.GetInstructionAddr(i) == ip) {
			ui->lstInstructions->item(i)->setBackgroundColor(Qt::red);
			ui->lstInstructions->scrollToItem(ui->lstInstructions->item(i));
		} else {
			ui->lstInstructions->item(i)->setBackgroundColor(Qt::white);
		}
	}
	QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
	qMemModel->ClearHighlights();
	qMemModel->Highlight(ip,mVM.CalcInstructionLen(),Qt::yellow);
	qMemModel->update();

}

void MemWnd::UpdateMemView() {
	QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
	qMemModel->copyData(mVM.GetMemPtr());
	qMemModel->ClearHighlights();
	qMemModel->update();
}

void MemWnd::stepInVM() {
	if(mVM.isLoaded()) {
		int err = mVM.Step();
		if(err < 0) {
			DisableRun(err);
		}
		UpdateGui();
		mVM.notifyReadCallbacks();
		mVM.notifyWriteCallbacks();
	}
}

void MemWnd::stepOutVM() {


}

void MemWnd::stepOverVM() {
	if(mVM.isLoaded()) {
		int err = mVM.Step();
		if(err == Instruction::CALL_CALLED) {
			unsigned int numCall = 1;
			while(numCall) {
				err = mVM.Step();
				if(err == Instruction::CALL_CALLED) {
					numCall++;
				} else if(err == Instruction::RET_CALLED) {
					numCall--;
				} else if(err < 0) {
					DisableRun(err);
					break;
				}
			}
		} else if(err < 0) {
			DisableRun(err);
		}
		UpdateGui();
	}
}

void MemWnd::DisableRun(int err) {
	switch(err) {
	case Processor::PROC_ERR_INV_INST:
		QMessageBox::information(this, tr("Execution Halted"), tr("Encountered an unknown instruction, execution is stopping."));
		break;
	case Processor::PROC_ERR_INST:
		QMessageBox::information(this, tr("Execution Halted"), tr("Encountered a malformed instruction, execution is stopping."));
		break;
	}

	ui->actionRun->setEnabled(false);
	ui->actionStep_Into->setEnabled(false);
	ui->actionStep_Out->setEnabled(false);
	ui->actionStep_Over->setEnabled(false);
}

void MemWnd::EnableRun() {
	ui->actionRun->setEnabled(true);
	ui->actionStep_Into->setEnabled(true);
	ui->actionStep_Out->setEnabled(true);
	ui->actionStep_Over->setEnabled(true);
}
