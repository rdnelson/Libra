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
#include "peripherals/Timer.hpp"
#include "QKbdFilter.hpp"

#include <QTimer>
#include <iostream>

#define TABLE(a) (new QTableWidgetItem(a))

MemWnd::MemWnd(const char* const file, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MemWnd),
	mFile(""),
	COL_LABEL(0),
	COL_LST(1),
	COL_INST(2)
{
	//Setup the ui object (Qt Mandatory)
	ui->setupUi(this);

	//Initialize the keyboard filter
	QKbdFilter* kbdFilter = new QKbdFilter();
	this->connect(kbdFilter, SIGNAL(KeyEvent(QKeyEvent*)), this, SLOT(KeyEvent(QKeyEvent*)));
	this->installEventFilter(kbdFilter);
	this->setWindowTitle("Libra - 8086 Emulator");

	ui->lstInstructions->setColumnCount(3);
	ui->lstInstructions->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui->lstInstructions->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

	//Create a temporary screen, because the processor isn't initialized until a file is loaded
	Screen scr;

	//Initialize font size, and contents of screen
	std::string strScreen = scr.GetStr();
	QFont fnt = ui->textBrowser->font();
	fnt.setPixelSize(15);
	ui->textBrowser->setFont(fnt);
	ui->textBrowser->setPlainText(QString::fromAscii(strScreen.data(), strScreen.size()));

	//Calculate font metrics for setting screen size
	ui->textBrowser->resize(ui->textBrowser->fontMetrics().width(' ') * scr.GetWidth() + 10,
							ui->textBrowser->fontMetrics().height() * scr.GetHeight() + 10);
	ui->textBrowser->setFixedSize(ui->textBrowser->size());

	//connect menubar actions
	this->connect(this->ui->action_Open, SIGNAL(triggered()),this, SLOT(loadObjFile_Clicked()));
	this->connect(this->ui->actionRun, SIGNAL(triggered()), this, SLOT(runVM_Clicked()));
	this->connect(this->ui->actionPause, SIGNAL(triggered()), this, SLOT(pauseVM_Clicked()));
	this->connect(this->ui->actionStop, SIGNAL(triggered()), this, SLOT(stopVM_Clicked()));
	this->connect(this->ui->actionStep_Into, SIGNAL(triggered()), this, SLOT(stepInVM_Clicked()));
	this->connect(this->ui->actionStep_Over, SIGNAL(triggered()), this, SLOT(stepOverVM_Clicked()));
	this->connect(this->ui->actionStep_Out, SIGNAL(triggered()), this, SLOT(stepOutVM_Clicked()));
	this->connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(appQuit_Clicked()));
	this->connect(this->ui->actionReload_File, SIGNAL(triggered()), this, SLOT(reloadObjFile_Clicked()));
	this->connect(this->ui->actionBreakpoint, SIGNAL(triggered()), this, SLOT(toggleBreakpoint_Clicked()));
	this->connect(this->ui->actionEnable_Listings, SIGNAL(triggered()), this, SLOT(enableListings_Clicked()));

	//Create the memory view model
	QMemModel* memModel = new QMemModel(this);
	memModel->setDataPtr(mVM.GetMemPtr());

	//Setup the memory view control
	int width = ui->tableView->fontMetrics().width(" FF ");// * 3;
	ui->tableView->setModel(memModel);
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	ui->tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	for(int i = 0; i < 0x10; i++) {
		ui->tableView->setColumnWidth(i, width);
	}
	ui->tableView->setFixedWidth(width/3 * (3 * 17 + 5));

	//Create the VMWorker and hook it up to the slots on the GUI.
	mVMWorker = new VMWorker(&mVM);
	connect(mVMWorker, SIGNAL(breakpoint()), this, SLOT(workerBreakpoint()));
	connect(mVMWorker, SIGNAL(runDone()), this, SLOT(workerRunDone()));
	connect(mVMWorker, SIGNAL(paused()), this, SLOT(workerPaused()));
	connect(mVMWorker, SIGNAL(stepDone()), this, SLOT(workerStepDone()));
	connect(mVMWorker, SIGNAL(error(int)), this, SLOT(workerRunError(int)));
	connect(mVMWorker, SIGNAL(quit()), mVMWorker, SLOT(deleteLater()));
	connect(mVMWorker, SIGNAL(procReturn(int)), this, SLOT(workerProcReturn(int)));
	connect(mVMWorker, SIGNAL(stopped()), this, SLOT(workerStopped()));
	connect(this, SIGNAL(vmResume()), mVMWorker, SLOT(run()));
	connect(this, SIGNAL(vmPause()), mVMWorker, SLOT(pause()));

	//Initialize the timer's QTimer object
	QTimer* baseTimer = new QTimer();
	connect(baseTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
	mVM.SetTimer(baseTimer);
	baseTimer->moveToThread(mVMWorker);

	QTimer* guiTimer = new QTimer();
	guiTimer->setInterval(30); //~30th of a second
	guiTimer->setSingleShot(false);
	connect(guiTimer, SIGNAL(timeout()), this, SLOT(UpdateScreenTick()));
	guiTimer->start();

	//Now that everything is initialized, see if we can load the provided file
	if(file != 0) {
		mFile = QString::fromAscii(file);
		loadFile(false);
		if(!mVM.isLoaded()) {
			mFile = "";
		}
	}
}

MemWnd::~MemWnd()
{
	mVMWorker->terminate();
	delete mVMWorker;
	delete ui;
}

/*
 * GUI Action Handler Section
 */

void MemWnd::appQuit_Clicked() {

	emit vmPause();
	//Give the VM 1 second to shut down, if it fails, quit anyways.
	mVMWorker->wait(1000);
	qApp->exit();
}

void MemWnd::loadObjFile_Clicked() {
	loadFile(true);
}

void MemWnd::reloadObjFile_Clicked() {
	loadFile(false);
}

void MemWnd::runVM_Clicked() {
	//Ensure a file is loaded
	if(mVM.isLoaded()){
		//Update the memory control, and clear highlighting
		UpdateMemView();

		//Disable focus (helps with keyboard input)
		ui->tableView->setFocusPolicy(Qt::NoFocus);

		//Clear all highlighting except breakpoints
		for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
			if(mVM.FindBreakpoint(mVM.GetInstructionAddr(i))) {
				for(int j = 0; j < ui->lstInstructions->columnCount(); j++) {
					ui->lstInstructions->item(i, j)->setBackgroundColor(Qt::red);
				}
			} else {
				for(int j = 0; j < ui->lstInstructions->columnCount(); j++) {
					ui->lstInstructions->item(i, j)->setBackgroundColor(Qt::white);
				}
			}
		}

		//Disable all the run actions
		DisableRun(0);

		//start the vmworker thread to run the program
		mVMWorker->start();

		//Enable the stop button
		ui->actionStop->setEnabled(true);
	}
}

void MemWnd::stepInVM_Clicked() {
	//Ensure program is loaded
	if(mVM.isLoaded()) {
		//Step once, and check for errors
		int err = mVM.Step();
		if(err < 0) {
			//Something bad happened, program can't go any further
			DisableRun(err);
		}

		//Enable stop (if it isn't already) because the program state can be reinitialized now
		ui->actionStop->setEnabled(true);

		//Update the entire gui
		UpdateGui();

		//Notify all Memory Logging functions to update their logs
		mVM.notifyReadCallbacks();
		mVM.notifyWriteCallbacks();
	}
}

void MemWnd::stepOutVM_Clicked() {
	//Ensure a file is loaded
	if(mVM.isLoaded()) {
		//Execute a step and check for errors
		int err = mVM.Step();
		//Loop until a RET is executed
		while(err != Instruction::RET_CALLED) {
			//Even if RET hasn't been hit, the program crashed, time to stop
			if(err < 0) {
				DisableRun(err);
				break;
			}
			//Execute another instruction in the search for a RET
			err = mVM.Step();
		}
		//Enable Stop, cause the program crashed.
		ui->actionStop->setEnabled(true);

		//Update the entire gui
		UpdateGui();
	}
}

void MemWnd::stepOverVM_Clicked() {
	//Ensure a file is loaded
	if(mVM.isLoaded()) {
		//Execute one step and check for errors
		int err = mVM.Step();
		//check if a CALL was executed
		if(err == Instruction::CALL_CALLED) {
			//One CALL was executed
			unsigned int numCall = 1;
			//Until all calls have been returned from
			while(numCall) {
				//Execute another instruction in the search for enough RET
				err = mVM.Step();
				//This CALL has to be returned from as well
				if(err == Instruction::CALL_CALLED) {
					numCall++;
				} else if(err == Instruction::RET_CALLED) {
					numCall--;
				} else if(err < 0) {
					//Errors still cause problems
					DisableRun(err);
					break;
				}
			}
		} else if(err < 0) {
			//This is incase an error happened without a CALL happening
			DisableRun(err);
		}

		ui->actionStop->setEnabled(true);
		UpdateGui();
	}
}

void MemWnd::pauseVM_Clicked() {
	if(mVM.isLoaded()) {
		emit vmPause();
		UpdateGui();
	}
}

void MemWnd::stopVM_Clicked() {
	if(mVM.isLoaded()) {
		emit vmPause();
		mVMWorker->wait(200);
		mVM.Stop();
		UpdateGui();
	}
	ui->actionStop->setEnabled(false);
}

void MemWnd::toggleBreakpoint_Clicked() {
	//Check if a breakpoint already exists on this line
	Breakpoint* bp = mVM.FindBreakpoint(mVM.GetInstructionAddr(ui->lstInstructions->currentRow()));
	if(bp != 0) {
		//Remove any existing breakpoints
		mVM.RemoveBreakpoint(bp->GetIP());
		return;
	}
	//Create a new unconditional breakpoint
	bp = new Breakpoint(mVM.GetInstructionAddr(ui->lstInstructions->currentRow()));
	mVM.AddBreakpoint(bp);
	//Highlight the new breakpoint
	ui->lstInstructions->currentItem()->setBackgroundColor(Qt::red);
}

void MemWnd::enableListings_Clicked() {
	UpdateInstructions();
}
/*
 * File loading functions
 */

void MemWnd::loadFile(bool newFile) {

	//Open a file dialog if this is loading a new object file
	if(newFile) {
		QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Flat Binary (*.*);;Virgo Object (*.obj)"));
		mFile = file;
	}

	//Stop the VMWorker if the current program is running
	if(mVM.isLoaded()) {
		emit vmPause();
		mVMWorker->wait(200);
		mVM.Stop();
	}

	//Sanity check for mFile
	if(mFile != "") {
		int err = mVM.LoadVirgoFile(mFile.toStdString().c_str());

		//Ensure the loading succeeded
		if(mVM.isLoaded()) {

			//Enable the proper buttons
			ui->actionReload_File->setEnabled(true);
			ui->actionRun->setEnabled(true);
			ui->actionStop->setEnabled(false);
			ui->actionStep_Into->setEnabled(true);
			ui->actionStep_Over->setEnabled(true);
			ui->actionStep_Out->setEnabled(true);

			//Initialize the instruction listing
			mVM.Disassemble();
			ui->lstInstructions->clear();
			UpdateInstructions();

			//Update all the other controls
			UpdateGui();
		} else {
			QMessageBox::critical(this, "File loading failed", "Loading the file \"" + mFile + "\" Failed.\n" + mVM.GetErrStr(err));
		}
	}
}



/*
 * VMWorker Signal Handler Section
 */

//Breakpoint Hit
void MemWnd::workerBreakpoint() {

	EnableRun();
	UpdateGui();

}
//Program execution complete
void MemWnd::workerRunDone() {
	this->setWindowTitle("VM Stopped");
	ui->tableView->setFocusPolicy(Qt::WheelFocus);
	UpdateGui();
}
//Program execution error
void MemWnd::workerRunError(int err) {
	UpdateGui();
	DisableRun(err);
}
//Program paused
void MemWnd::workerPaused() {
	UpdateGui();
	ui->tableView->setFocusPolicy(Qt::WheelFocus);
	EnableRun();
}
//Program step complete
void MemWnd::workerStepDone() {
	UpdateScreen();
	mVM.notifyReadCallbacks();
	mVM.notifyWriteCallbacks();
}
//Program's processor returned an info code
void MemWnd::workerProcReturn(int err) {
	if(err == Instruction::PERIPH_WRITE) {
		UpdateScreen();
	}
}
//Program's processor halted
void MemWnd::workerStopped() {
	QMessageBox::information(this, "Halt Encountered", "HLT was encountered, execution is terminated.");
}

/*
 * GUI Update Function Section
 */

void MemWnd::UpdateGui() {

	UpdateScreen();

	//Update all of the register and flag boxes
	ui->txtAX->setText(mVM.GetProc().GetRegisterHex(REG_AX));
	ui->txtAX->setStyleSheet("");
	ui->txtBX->setText(mVM.GetProc().GetRegisterHex(REG_BX));
	ui->txtBX->setStyleSheet("");
	ui->txtCX->setText(mVM.GetProc().GetRegisterHex(REG_CX));
	ui->txtCX->setStyleSheet("");
	ui->txtDX->setText(mVM.GetProc().GetRegisterHex(REG_DX));
	ui->txtDX->setStyleSheet("");
	ui->txtSI->setText(mVM.GetProc().GetRegisterHex(REG_SI));
	ui->txtSI->setStyleSheet("");
	ui->txtDI->setText(mVM.GetProc().GetRegisterHex(REG_DI));
	ui->txtDI->setStyleSheet("");
	ui->txtBP->setText(mVM.GetProc().GetRegisterHex(REG_BP));
	ui->txtBP->setStyleSheet("");
	ui->txtSP->setText(mVM.GetProc().GetRegisterHex(REG_SP));
	ui->txtSP->setStyleSheet("");
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

	//Update the instruction listing highlight state
	unsigned int ip = mVM.GetProc().GetRegister(REG_IP);
	for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
		for(int j = 0; j < ui->lstInstructions->columnCount(); j++) {
			if(mVM.GetInstructionAddr(i) == ip) {
				ui->lstInstructions->item(i, j)->setBackgroundColor(Qt::yellow);
				ui->lstInstructions->scrollToItem(ui->lstInstructions->item(i,0));
			} else if(mVM.FindBreakpoint(mVM.GetInstructionAddr(i))) {
				ui->lstInstructions->item(i,j)->setBackgroundColor(Qt::red);
			} else {
				ui->lstInstructions->item(i,j)->setBackgroundColor(Qt::white);
			}
		}
	}

	UpdateMemView(ip, mVM.CalcInstructionLen());

	UpdateInstHighlight();
}

void MemWnd::UpdateScreen() {
	//Search for the screen
	unsigned int numDevices = mVM.GetDevices().size();
	for(unsigned int i = 0; i < numDevices; i++) {
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_SCREEN) {
			//Found the screen, now update the screen control
			ui->textBrowser->setPlainText(mVM.GetDevices().at(i)->GetStr().c_str());
			break;
		}
	}
}

void MemWnd::UpdateMemView(unsigned int ip, unsigned int len) {
	QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
	if(qMemModel) {
		//Remove execution highlighting
		qMemModel->ClearHighlights();
		if(len != 0) {
			qMemModel->Highlight(ip & 0xFFFF, len, Qt::yellow);
		}
		//Let the GUI know that there are memory changes
		qMemModel->update();
	}
}

void MemWnd::UpdateInstHighlight() {
	const Instruction* inst = mVM.GetProc().GetNextInstruction();
	Operand* op = 0;
	QColor color = Qt::white;
	if(!inst) {
		return;
	}
	for(unsigned int i = 0; i < 4; i++) {
		switch(i) {
			case 0:
				color = Qt::green;
				break;
			case 1:
				color = Qt::red;
				break;
			case 2:
				color = Qt::magenta;
				break;
			case 3:
				color = Qt::cyan;
				break;
			default:
				color = Qt::white;
		}
		op = inst->GetOperand(i);
		if(op) {
			int val = op->GetUnresolvedValue();
			if(val > 0 && val < 0x10000) {
				//Address, highlight it in mem view
				QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
				if(qMemModel) {
					qMemModel->Highlight(val, 2, color);
				}
				qMemModel->update();
			} else if(val < 0) {
				//Immediate, highlight it in mem view
				QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
				if(qMemModel) {
					qMemModel->Highlight(op->GetOffset(),op->GetBytecodeLen(),color);
					qMemModel->update();
				}
			} else if(val >= 0x10000) {
				//Register, highlight the register box
				val -= 0x10000;
				switch(val) {
				case REG_AX:
				case REG_AL:
				case REG_AH:
					ui->txtAX->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_BX:
				case REG_BL:
				case REG_BH:
					ui->txtBX->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_CX:
				case REG_CL:
				case REG_CH:
					ui->txtCX->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_DX:
				case REG_DL:
				case REG_DH:
					ui->txtDX->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_SP:
					ui->txtSP->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_BP:
					ui->txtBP->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_SI:
					ui->txtSI->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				case REG_DI:
					ui->txtDI->setStyleSheet("QLineEdit {background: " + color.name() + "}");
					break;
				}

			}
		}
	}
}

void MemWnd::UpdateInstructions() {
	ui->lstInstructions->setRowCount(0);
	for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
		//add a new row
		ui->lstInstructions->setRowCount(ui->lstInstructions->rowCount() + 1);
		for(int j = 0; j <  ui->lstInstructions->columnCount(); j++) {

			//If labels are enabled
			if(j == COL_LABEL) {
				ui->lstInstructions->setItem(ui->lstInstructions->rowCount() - 1, j, TABLE(mVM.GetInstructionLabel(i).c_str()));
			} else if(j == COL_LST) {
				if(ui->actionEnable_Listings->isChecked()) {
					ui->lstInstructions->setItem(ui->lstInstructions->rowCount() - 1, j, TABLE(mVM.GetInstructionText(i).c_str()));
				} else {
					ui->lstInstructions->setItem(ui->lstInstructions->rowCount() - 1, j, TABLE(""));
				}

			} else if(j == COL_INST) {
				ui->lstInstructions->setItem(ui->lstInstructions->rowCount() - 1, j, TABLE(mVM.GetInstructionStr(i).c_str()));
			}
		}
	}
	QHeaderView* horz = ui->lstInstructions->horizontalHeader();
	if(horz) {
		horz->setResizeMode(QHeaderView::ResizeToContents);
	}
	QHeaderView* vert = ui->lstInstructions->verticalHeader();
	if(vert) {
		vert->setDefaultSectionSize(15);
		vert->setResizeMode(QHeaderView::Fixed);
		vert->resizeSections(QHeaderView::Fixed);
	}
}

void MemWnd::DisableRun(int err) {

	//Disable the run actions
	ui->actionRun->setEnabled(false);
	ui->actionStep_Into->setEnabled(false);
	ui->actionStep_Out->setEnabled(false);
	ui->actionStep_Over->setEnabled(false);
	
	//Check for an error
	switch(err) {
	case Processor::PROC_ERR_INV_INST:
		QMessageBox::information(this, tr("Execution Halted"), tr("Encountered an unknown instruction, execution is stopping."));
		return;
	case Processor::PROC_ERR_INST:
		QMessageBox::information(this, tr("Execution Halted"), tr("Encountered a malformed instruction, execution is stopping."));
		return;
	}

	//Only re-enable pause if there's no error
	ui->actionPause->setEnabled(true);
}

void MemWnd::EnableRun() {
	//Enable all the run commands
	ui->actionRun->setEnabled(true);
	ui->actionStep_Into->setEnabled(true);
	ui->actionStep_Out->setEnabled(true);
	ui->actionStep_Over->setEnabled(true);

	//Disable pause. Leave stop enabled, because it's valid to stop execution while paused.
	ui->actionPause->setEnabled(false);
}

/*
 * Peripheral Event Handlers
 */

#define ENTER_KEY_NL 0x0A
#define ENTER_KEY_CR 0x0D

void MemWnd::KeyEvent(QKeyEvent* evt) {
	//Search for a keyboard
	unsigned int numDevices = mVM.GetDevices().size();
	for(unsigned int i = 0; i < numDevices; i++) {
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_KEYBOARD) {
			//found the keyboard, dispatch the keypress event
			if (evt->text().toAscii().size() > 0) {
				char key = evt->text().toAscii().at(0);
				if (key == ENTER_KEY_CR) {
					key = ENTER_KEY_NL; //Convert Enter's Carriage Return to Newline
				}
				((Keyboard*)mVM.GetDevices().at(i))->Update(key, evt->type() == QEvent::KeyPress);
				break;
			}
		}
	}
}

void MemWnd::TimerEvent() {
	//Search for the timer
	size_t numDevices = mVM.GetDevices().size();
	for(size_t i = 0; i < numDevices; i++) {
		if(mVM.GetDevices().at(i)->GetType() == IPeripheral::PERIPH_TIMER) {
			//Found timer, dispatch the elapsed event
			((Timer*)mVM.GetDevices().at(i))->Update();
			break;
		}
	}
}

void MemWnd::UpdateScreenTick() {
	UpdateScreen();
}
