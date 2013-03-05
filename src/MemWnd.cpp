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
#include "peripherals/Screen.hpp"
#include "peripherals/Keyboard.hpp"
#include "QKbdFilter.hpp"

#include <iostream>

MemWnd::MemWnd(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MemWnd),
	mFile("")
{
	ui->setupUi(this);
	QKbdFilter* kbdFilter = new QKbdFilter();
	this->connect(kbdFilter, SIGNAL(KeyEvent(QKeyEvent*)), this, SLOT(KeyEvent(QKeyEvent*)));
	this->installEventFilter(kbdFilter);
	//setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint);

	Screen* scr = 0;
	unsigned int numDevices = mVM.GetDevices().size();
	for(unsigned int i = 0; i < numDevices; i++) {
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_SCREEN) {
			scr = (Screen*)mVM.GetDevices().at(i);
		}
	}

	QString s = "";
	for(unsigned int i = 0; i < scr->GetWidth(); i++)
		s.append(" ");
	s.append('\n');
	QString str = "";
	for(unsigned int i = 0; i < scr->GetHeight(); i++) {
		str.append(s);
	}

	//setup screen
	QFont fnt = ui->textBrowser->font();
	fnt.setPixelSize(15);
	ui->textBrowser->setFont(fnt);
	ui->textBrowser->setPlainText(str);

	ui->textBrowser->resize(ui->textBrowser->fontMetrics().width(s) + 10, ui->textBrowser->fontMetrics().height() * scr->GetHeight() + 10);
	ui->textBrowser->setFixedSize(ui->textBrowser->size());


	//connect menubar actions
	this->connect(this->ui->action_Open, SIGNAL(triggered()),this, SLOT(loadObjFile()));
	this->connect(this->ui->actionRun, SIGNAL(triggered()), this, SLOT(runVM()));
	this->connect(this->ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseVM_Clicked()));
	this->connect(this->ui->actionStop, SIGNAL(triggered()), this, SLOT(stopVM_Clicked()));
	this->connect(this->ui->actionStep_Into, SIGNAL(triggered()), this, SLOT(stepInVM()));
	this->connect(this->ui->actionStep_Over, SIGNAL(triggered()), this, SLOT(stepOverVM()));
	this->connect(this->ui->actionStep_Out, SIGNAL(triggered()), this, SLOT(stepOutVM()));
	this->connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(appQuit()));
	this->connect(this->ui->actionReload_File, SIGNAL(triggered()), this, SLOT(reloadObjFile()));

	QMemModel* model = new QMemModel(this);
	model->copyData(mVM.GetMemPtr());

	int width = ui->tableView->fontMetrics().width('F') * 4;
	ui->tableView->setModel(model);
	for(int i = 0; i < 0x10; i++) {
		ui->tableView->setColumnWidth(i, width);
	}
	ui->tableView->setMaximumWidth(width * 0x13);

	mVMWorker = new VMWorker(&mVM);
	connect(mVMWorker, SIGNAL(breakpoint()), this, SLOT(vmBreakpoint()));
	connect(mVMWorker, SIGNAL(breakpoint()), this, SLOT(vmBreakpoint()));
	connect(mVMWorker, SIGNAL(runDone()), this, SLOT(vmDone()));
	connect(mVMWorker, SIGNAL(paused()), this, SLOT(vmPaused()));
	connect(this, SIGNAL(vmResume()), mVMWorker, SLOT(run()));
	connect(this, SIGNAL(vmStep()), mVMWorker, SLOT(step()));
	connect(this, SIGNAL(vmPause()), mVMWorker, SLOT(pause()));
	connect(mVMWorker, SIGNAL(stepDone()), this, SLOT(stepDone()));
	connect(mVMWorker, SIGNAL(quit()), mVMWorker, SLOT(deleteLater()));
	connect(mVMWorker, SIGNAL(error(int)), this, SLOT(vmRunError(int)));


}

MemWnd::~MemWnd()
{
	delete mVMWorker;
	delete ui;
}

void MemWnd::loadObjFile() {

	QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Flat Binary (*.*);;Virgo Object (*.obj)"));
	mFile = file;
	mVM.LoadVirgoFile(file.toStdString().c_str());
	if(mVM.isLoaded()) {
		ui->actionReload_File->setEnabled(true);
		ui->actionRun->setEnabled(true);
		ui->actionStop->setEnabled(false);
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
	stopVM_Clicked();
	if(mFile != "") {
		mVM.LoadVirgoFile(mFile.toStdString().c_str());
		if(mVM.isLoaded()) {
			ui->actionReload_File->setEnabled(true);
			ui->actionRun->setEnabled(true);
			ui->actionStop->setEnabled(false);
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
}

void MemWnd::runVM() {
	if(mVM.isLoaded()){
		QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
		if(qMemModel) {
			qMemModel->ClearHighlights();
			qMemModel->update();
		}
		ui->tableView->setFocusPolicy(Qt::NoFocus);
		for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
			ui->lstInstructions->item(i)->setBackgroundColor(Qt::white);
		}

		DisableRun(0);
		mVMWorker->start();
	}
	ui->actionStop->setEnabled(true);
}

void MemWnd::vmBreakpoint() {

	EnableRun();
	UpdateGui();

}

void MemWnd::vmDone() {
	this->setWindowTitle("VM Stopped");
	ui->tableView->setFocusPolicy(Qt::WheelFocus);
	UpdateGui();

}

void MemWnd::vmRunError(int err) {
	UpdateGui();
	DisableRun(err);
}

void MemWnd::vmPaused() {
	UpdateGui();
	ui->tableView->setFocusPolicy(Qt::WheelFocus);
	EnableRun();
}

void MemWnd::stepDone() {
	UpdateScreen();
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
	ui->chkInterrupt->setChecked(mVM.GetProc().GetFlag(FLAGS_IF));
	UpdateMemView();

	unsigned int ip = mVM.GetProc().GetRegister(REG_IP);
	for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
		if(mVM.GetInstructionAddr(i) == ip) {
			ui->lstInstructions->item(i)->setBackgroundColor(Qt::yellow);
			ui->lstInstructions->scrollToItem(ui->lstInstructions->item(i));
		} else {
			ui->lstInstructions->item(i)->setBackgroundColor(Qt::white);
		}
	}
	QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
	if(qMemModel) {
		qMemModel->ClearHighlights();
		qMemModel->Highlight(ip,mVM.CalcInstructionLen(),Qt::yellow);
		qMemModel->update();
	}

}

void MemWnd::UpdateScreen() {
	unsigned int numDevices = mVM.GetDevices().size();
	for(unsigned int i = 0; i < numDevices; i++) {
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_SCREEN) {
			ui->textBrowser->setPlainText(mVM.GetDevices().at(i)->GetStr().c_str());
		}
	}
}

void MemWnd::UpdateMemView() {
	QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
	//Remove execution highlighting
	qMemModel->ClearHighlights();
	//Let the GUI know that there are memory changes
	qMemModel->update();
}

void MemWnd::stepInVM() {
	if(mVM.isLoaded()) {
		int err = mVM.Step();
		if(err < 0) {
			DisableRun(err);
		}
		ui->actionStop->setEnabled(true);
		UpdateGui();
		mVM.notifyReadCallbacks();
		mVM.notifyWriteCallbacks();
	}
}

void MemWnd::stepOutVM() {

	if(mVM.isLoaded()) {
		int err = mVM.Step();
		while(err != Instruction::RET_CALLED) {
			if(err < 0) {
				DisableRun(err);
				break;
			}
			err = mVM.Step();
		}
		ui->actionStop->setEnabled(true);
		UpdateGui();
	}

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
		ui->actionStop->setEnabled(true);
		UpdateGui();
	}
}

void MemWnd::pauseVM_Clicked() {
	if(mVM.isLoaded()) {
		emit vmPause();
	}
}

void MemWnd::stopVM_Clicked() {
	if(mVM.isLoaded()) {
		emit vmPause();
		mVMWorker->wait(200);
		mVM.Stop();
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
	ui->actionPause->setEnabled(true);
	ui->actionStep_Into->setEnabled(false);
	ui->actionStep_Out->setEnabled(false);
	ui->actionStep_Over->setEnabled(false);
}

void MemWnd::EnableRun() {
	ui->actionRun->setEnabled(true);
	ui->actionPause->setEnabled(false);
	ui->actionStep_Into->setEnabled(true);
	ui->actionStep_Out->setEnabled(true);
	ui->actionStep_Over->setEnabled(true);
}

void MemWnd::appQuit() {

	emit vmPause();
	//Give the VM 1 second to shut down, if it fails, quit anyways.
	mVMWorker->wait(1000);
	qApp->exit();
}

void MemWnd::KeyEvent(QKeyEvent* evt) {

	unsigned int numDevices = mVM.GetDevices().size();
	for(unsigned int i = 0; i < numDevices; i++) {
		//found a screen
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_KEYBOARD) {
			qDebug("Keypress: %d", evt->key());
			((Keyboard*)mVM.GetDevices().at(i))->Update(evt->key(), evt->type() == QEvent::KeyPress);
		}
	}
}
