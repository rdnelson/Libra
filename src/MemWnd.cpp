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
#include "QDesktopServices"
#include "QUrl"
#include "Breakpoint.hpp"
#include "peripherals/Screen.hpp"
#include "peripherals/Keyboard.hpp"
#include "peripherals/Timer.hpp"
#include "QKbdFilter.hpp"
#include "Processor.hpp"

#include "opcodes/In.hpp"
#include "opcodes/Out.hpp"
#include "Version.h"

#include <QTimer>
#include <iostream>

#define TABLE(a) (new QTableWidgetItem(a))

MemWnd::MemWnd(const char* const file, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MemWnd),
	mFile(""),
	mCurDir(""),
	mSettings("SCE", "Libra"),
	COL_LABEL(0),
	COL_LST(1),
	COL_INST(2),
	ipWarned(false)
{
	//Setup the ui object (Qt Mandatory)
	ui->setupUi(this);
	mCurDir = mSettings.value("FileOpen/CurrentDirectory").toString();
	if(!QDir(mCurDir).exists()) {
		mCurDir = "";
		mSettings.setValue("FileOpen/CurrentDirectory", mCurDir);
	}

	//Initialize the keyboard filter
	QKbdFilter* kbdFilter = new QKbdFilter();
	this->connect(kbdFilter, SIGNAL(KeyEvent(QKeyEvent*)), this, SLOT(KeyEvent(QKeyEvent*)));
	this->installEventFilter(kbdFilter);
	this->setWindowTitle("Libra - 8086 Emulator");

	ui->lstInstructions->setColumnCount(3);
	ui->lstInstructions->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui->lstInstructions->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui->lstInstructions->setStyleSheet("QTableWidget::item::selected {background-color: #3399FF; color: white;}");
	ui->lstInstructions->setContextMenuPolicy(Qt::CustomContextMenu);

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
	this->connect(this->ui->actionEnable_Memory_Logging, SIGNAL(triggered()), this, SLOT(enableMemoryLogging_Clicked()));
	this->connect(this->ui->lstInstructions, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(lstInstructions_RightClicked(const QPoint&)));
	this->connect(this->ui->actionHelp, SIGNAL(triggered()), this, SLOT(openHelp()));
    this->connect(this->ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAbout()));

	//Create the memory view model
	QMemModel* memModel = new QMemModel(this);
	memModel->setDataPtr(mVM.GetMemPtr());

	//Setup the memory view control
	int width = ui->tableView->fontMetrics().width(" FF ");// * 3;
	ui->tableView->setModel(memModel);
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui->tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	for(int i = 0; i < 0x10; i++) {
		ui->tableView->setColumnWidth(i, width);
	}

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
	connect(mVMWorker, SIGNAL(devError(IPeripheral*, unsigned int)), this, SLOT(deviceError(IPeripheral*, unsigned int)));
	connect(this, SIGNAL(vmResume()), mVMWorker, SLOT(run()));
	connect(this, SIGNAL(vmPause()), mVMWorker, SLOT(pause()));

	//Connect the flag controls
	connect(this->ui->chkAdjust, SIGNAL(stateChanged(int)), this, SLOT(adjustFlagChanged(int)));
	connect(this->ui->chkCarry, SIGNAL(stateChanged(int)), this, SLOT(carryFlagChanged(int)));
	connect(this->ui->chkDirection, SIGNAL(stateChanged(int)), this, SLOT(directionFlagChanged(int)));
	connect(this->ui->chkInterrupt, SIGNAL(stateChanged(int)), this, SLOT(interruptFlagChanged(int)));
	connect(this->ui->chkOverflow, SIGNAL(stateChanged(int)), this, SLOT(overflowFlagChanged(int)));
	connect(this->ui->chkParity, SIGNAL(stateChanged(int)), this, SLOT(parityFlagChanged(int)));
	connect(this->ui->chkSign, SIGNAL(stateChanged(int)), this, SLOT(signFlagChanged(int)));
	connect(this->ui->chkTrap, SIGNAL(stateChanged(int)), this, SLOT(trapFlagChanged(int)));
	connect(this->ui->chkZero, SIGNAL(stateChanged(int)), this, SLOT(zeroFlagChanged(int)));

	//Connect the register controls
	connect(this->ui->txtAX, SIGNAL(editingFinished()), this, SLOT(axChanged()));
	connect(this->ui->txtBX, SIGNAL(editingFinished()), this, SLOT(bxChanged()));
	connect(this->ui->txtCX, SIGNAL(editingFinished()), this, SLOT(cxChanged()));
	connect(this->ui->txtDX, SIGNAL(editingFinished()), this, SLOT(dxChanged()));
	connect(this->ui->txtSP, SIGNAL(editingFinished()), this, SLOT(spChanged()));
	connect(this->ui->txtBP, SIGNAL(editingFinished()), this, SLOT(bpChanged()));
	connect(this->ui->txtSI, SIGNAL(editingFinished()), this, SLOT(siChanged()));
	connect(this->ui->txtDI, SIGNAL(editingFinished()), this, SLOT(diChanged()));
	connect(this->ui->txtFLAGS, SIGNAL(editingFinished()), this, SLOT(flChanged()));
	connect(this->ui->txtIP, SIGNAL(editingFinished()), this, SLOT(ipChanged()));

	//Connect the memory control
	connect(this->ui->tableView, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(memChanged(const QModelIndex&, const QModelIndex&)));

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
		SetMemoryEditState(false);

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

		ClearRegisterHighlighting();

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
		} else if(err == Processor::PROC_HALT) {
			QMessageBox::information(this, "Halt Encountered", "HLT was encountered, execution is terminated.");
		} else if (err == Instruction::PERIPH_WRITE) {
			unsigned int devErr = 0;
			for(int i = 0; i < mVM.GetDevices().size(); i++) {
				if(devErr = mVM.GetDevices()[i]->GetError()) {
					deviceError(mVM.GetDevices()[i], devErr);
				}
			}
		}

		//Enable stop (if it isn't already) because the program state can be reinitialized now
		ui->actionStop->setEnabled(true);

		//Update the entire gui
		UpdateGui();

		//Notify all Memory Logging functions to update their logs
		UpdateMemCallbacks();
	}
}

void MemWnd::stepOutVM_Clicked() {
	//Ensure a file is loaded
	if(mVM.isLoaded()) {
		//Update the memory control, and clear highlighting
		UpdateMemView();

		//Disable focus (helps with keyboard input)
		SetMemoryEditState(false);

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

		ClearRegisterHighlighting();

		//Disable all the run actions
		DisableRun(0);

		//start the vmworker thread to run the program
		mVMWorker->StepOut();

		//Enable the stop button
		ui->actionStop->setEnabled(true);

		//Notify all Memory Logging functions to update their logs
		UpdateMemCallbacks();
	}
}

void MemWnd::stepOverVM_Clicked() {
	//Ensure a file is loaded
	if(mVM.isLoaded()) {
		//Update the memory control, and clear highlighting
		UpdateMemView();

		//Disable focus (helps with keyboard input)
		SetMemoryEditState(false);

		//Clear all highlighting except breakpoints
		HighlightInstructions();

		ClearRegisterHighlighting();

		//Disable all the run actions
		DisableRun(0);

		//start the vmworker thread to run the program
		mVMWorker->StepOver();

		//Enable the stop button
		ui->actionStop->setEnabled(true);

		//Notify all Memory Logging functions to update their logs
		UpdateMemCallbacks();
	}
}

void MemWnd::UpdateMemCallbacks() {
	if(this->ui->actionEnable_Memory_Logging->isChecked()) {
		mVM.notifyReadCallbacks();
		mVM.notifyWriteCallbacks();
	} else {
		mVM.notifyReadCallbacks(0);
		mVM.notifyWriteCallbacks(0);
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
		HighlightInstructions();
		return;
	}
	//Create a new unconditional breakpoint
	bp = new Breakpoint(mVM.GetInstructionAddr(ui->lstInstructions->currentRow()));
	mVM.AddBreakpoint(bp);
	//Highlight the new breakpoint
	HighlightInstructions();
}

void MemWnd::enableListings_Clicked() {
	UpdateInstructions();
}

void MemWnd::enableMemoryLogging_Clicked() {
	if(this->ui->actionEnable_Memory_Logging->isChecked())
		mVM.EnableMemoryLogging();
	else
		mVM.DisableMemoryLogging();
}

void MemWnd::lstInstructions_RightClicked(const QPoint& pt) {
	int row = ui->lstInstructions->currentRow();
	if(row != -1) {
		unsigned int addr = ui->lstInstructions->item(row,0)->data(32).toInt();
		bool isBreakpoint = mVM.FindBreakpoint(addr) != 0;
		QPoint globalPos = ui->lstInstructions->mapToGlobal(pt);
		QMenu context;
		context.addAction(isBreakpoint ? "Remove Breakpoint" : "Add Breakpoint");

		QAction* selectedItem = context.exec(globalPos);
		if(selectedItem) {
			if(isBreakpoint) {
				mVM.RemoveBreakpoint(addr);
			} else {
				mVM.AddBreakpoint(new Breakpoint(addr));
			}
			HighlightInstructions();
		}
	}

}
/*
 * File loading functions
 */

void MemWnd::loadFile(bool newFile) {

	//Open a file dialog if this is loading a new object file
	if(newFile) {
		QString file = QFileDialog::getOpenFileName(this, tr("Open File"), mCurDir, tr("Virgo Object (*.obj);;Flat Binary (*.*)"));
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
		mCurDir = QFileInfo(mFile).absoluteDir().absolutePath();
		mSettings.setValue("FileOpen/CurrentDirectory", mCurDir);
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

			SetMemoryEditState(true);
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

	SetMemoryEditState(true);
	EnableRun();
	UpdateGui();

}
//Program execution complete
void MemWnd::workerRunDone() {
	this->setWindowTitle("VM Stopped");
	SetMemoryEditState(true);
	UpdateGui();

	//Notify all Memory Logging functions to update their logs
	UpdateMemCallbacks();
}
//Program execution error
void MemWnd::workerRunError(int err) {
	SetMemoryEditState(true);
	UpdateGui();
	DisableRun(err);

	//Notify all Memory Logging functions to update their logs
	UpdateMemCallbacks();
}
//Program paused
void MemWnd::workerPaused() {
	UpdateGui();
	SetMemoryEditState(true);
	EnableRun();

	//Notify all Memory Logging functions to update their logs
	UpdateMemCallbacks();
}
//Program step complete
void MemWnd::workerStepDone() {
	SetMemoryEditState(true);
	UpdateScreen();
	UpdateMemCallbacks();
}
//Program's processor returned an info code
void MemWnd::workerProcReturn(int err) {
	if(err == Instruction::PERIPH_WRITE) {
		UpdateScreen();
	}
}
//Program's processor halted
void MemWnd::workerStopped() {
	SetMemoryEditState(true);
	QMessageBox::information(this, "Halt Encountered", "HLT was encountered, execution is terminated.");
	workerPaused();

	//Notify all Memory Logging functions to update their logs
	UpdateMemCallbacks();
}

/*
 * GUI Update Function Section
 */

void MemWnd::UpdateFlags() {
	ui->chkAdjust->setChecked(mVM.GetProc().GetFlag(FLAGS_AF));
	ui->chkOverflow->setChecked(mVM.GetProc().GetFlag(FLAGS_OF));
	ui->chkCarry->setChecked(mVM.GetProc().GetFlag(FLAGS_CF));
	ui->chkParity->setChecked(mVM.GetProc().GetFlag(FLAGS_PF));
	ui->chkZero->setChecked(mVM.GetProc().GetFlag(FLAGS_ZF));
	ui->chkSign->setChecked(mVM.GetProc().GetFlag(FLAGS_SF));
	ui->chkInterrupt->setChecked(mVM.GetProc().GetFlag(FLAGS_IF));
	ui->chkDirection->setChecked(mVM.GetProc().GetFlag(FLAGS_DF));
	ui->chkTrap->setChecked(mVM.GetProc().GetFlag(FLAGS_TF));
}

void MemWnd::UpdateRegisters() {

	//Update all of the register and flag boxes
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

}

void MemWnd::UpdateGui() {

	UpdateScreen();
	UpdateFlags();
	UpdateRegisters();

	ClearRegisterHighlighting();

	UpdateMemView();

	//Update the instruction listing highlight state
	HighlightInstructions();

	unsigned int ip = mVM.GetProc().GetRegister(REG_IP);
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

		if (i == 1 &&
			(inst->GetOpcode() == In::IN_AL_DX ||
			 inst->GetOpcode() == In::IN_AL_IMM8 ||
			 inst->GetOpcode() == In::IN_AX_DX ||
			 inst->GetOpcode() == In::IN_AX_IMM8)) {

			 ui->txtIN->setStyleSheet("QLineEdit {background: " + color.name() + "}");
			 char hex[5];
			 sprintf(hex, "%04X", op->GetValue());
			 ui->txtIN->setText(hex);
			 continue;
		} else if (i == 1) {
			ui->txtIN->setText("N/A");
		} else if (i == 0 &&
			(inst->GetOpcode() == Out::OUT_DX_AL ||
			 inst->GetOpcode() == Out::OUT_DX_AX ||
			 inst->GetOpcode() == Out::OUT_IMM8_AL ||
			 inst->GetOpcode() == Out::OUT_IMM8_AX)) {
			 ui->txtOUT->setStyleSheet("QLineEdit {background: " + color.name() + "}");
			 char hex[5];
			 sprintf(hex, "%04X", op->GetValue());
			 ui->txtOUT->setText(hex);
			continue;
		} else if (i == 0) {
			ui->txtOUT->setText("N/A");
		}

		if(op) {
			int val = op->GetUnresolvedValue();
			if(val >= 0 && val < 0x10000) {
				//Address, highlight it in mem view
				QMemModel* qMemModel = (QMemModel*)ui->tableView->model();
				if(qMemModel) {
					qMemModel->Highlight(val, op->GetBitmask() == 0xFF ? 1 : 2, color);
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
	ui->lstInstructions->clear();
	ui->lstInstructions->setRowCount(0);
	for(unsigned int i = 0; i < mVM.GetNumInstructions(); i++) {
		//add a new row
		ui->lstInstructions->setRowCount(ui->lstInstructions->rowCount() + 1);
		for(int j = 0; j <  ui->lstInstructions->columnCount(); j++) {

			//If labels are enabled
			if(j == COL_LABEL) {
				if (mVM.GetInstructionLabel(i).empty() && ui->actionEnable_Listings->isChecked()) {
					char* buf = new char[20];
					sprintf(buf, "%0.4X", mVM.GetInstructionAddr(i));
					ui->lstInstructions->setItem(ui->lstInstructions->rowCount() - 1, j, TABLE("    " + QString(buf) + "    "));
					delete buf;
				}
				else
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
			ui->lstInstructions->item(ui->lstInstructions->rowCount() - 1, j)->setData(32, QVariant(mVM.GetInstructionAddr(i)));
		}
	}
	QHeaderView* horz = ui->lstInstructions->horizontalHeader();
	if(horz) {
		horz->setResizeMode(0, QHeaderView::ResizeToContents);
		horz->setResizeMode(1, QHeaderView::Fixed);
		horz->setResizeMode(2, QHeaderView::ResizeToContents);
		if(ui->actionEnable_Listings->isChecked()) {
			horz->resizeSection(1, 75);
		} else {
			horz->resizeSection(1, 0);
		}
	}
	QHeaderView* vert = ui->lstInstructions->verticalHeader();
	if(vert) {
		vert->setDefaultSectionSize(15);
		vert->setResizeMode(QHeaderView::Fixed);
		vert->resizeSections(QHeaderView::Fixed);
	}
}

void MemWnd::ClearRegisterHighlighting() {
	ui->txtAX->setStyleSheet("");
	ui->txtBX->setStyleSheet("");
	ui->txtCX->setStyleSheet("");
	ui->txtDX->setStyleSheet("");
	ui->txtSI->setStyleSheet("");
	ui->txtDI->setStyleSheet("");
	ui->txtBP->setStyleSheet("");
	ui->txtSP->setStyleSheet("");
	ui->txtIP->setStyleSheet("");
	ui->txtFLAGS->setStyleSheet("");
	ui->txtOUT->setStyleSheet("");
	ui->txtIN->setStyleSheet("");
}

void MemWnd::HighlightInstructions() {
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

///Checkbox Flag update Functions

void MemWnd::adjustFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_AF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::carryFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_CF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::directionFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_DF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::interruptFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_IF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::overflowFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_OF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::parityFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_PF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::signFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_SF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::trapFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_TF, state == Qt::Checked); UpdateRegisters(); }
void MemWnd::zeroFlagChanged(int state) { mVM.GetProc().SetFlag(FLAGS_ZF, state == Qt::Checked); UpdateRegisters(); }

///End Checkbox Flag update Functions


///Register update functions

void MemWnd::axChanged() { if (QMemModel::_validHexText(this->ui->txtAX->text(), 4)) { mVM.GetProc().SetRegister(REG_AX, QMemModel::_htoi(this->ui->txtAX->text())); } ui->txtAX->setText(mVM.GetProc().GetRegisterHex(REG_AX)); }
void MemWnd::bxChanged() { if (QMemModel::_validHexText(this->ui->txtBX->text(), 4)) { mVM.GetProc().SetRegister(REG_BX, QMemModel::_htoi(this->ui->txtBX->text())); } ui->txtBX->setText(mVM.GetProc().GetRegisterHex(REG_BX)); }
void MemWnd::cxChanged() { if (QMemModel::_validHexText(this->ui->txtCX->text(), 4)) { mVM.GetProc().SetRegister(REG_CX, QMemModel::_htoi(this->ui->txtCX->text())); } ui->txtCX->setText(mVM.GetProc().GetRegisterHex(REG_CX)); }
void MemWnd::dxChanged() { if (QMemModel::_validHexText(this->ui->txtDX->text(), 4)) { mVM.GetProc().SetRegister(REG_DX, QMemModel::_htoi(this->ui->txtDX->text())); } ui->txtDX->setText(mVM.GetProc().GetRegisterHex(REG_DX)); }
void MemWnd::spChanged() { if (QMemModel::_validHexText(this->ui->txtSP->text(), 4)) { mVM.GetProc().SetRegister(REG_SP, QMemModel::_htoi(this->ui->txtSP->text())); } ui->txtSP->setText(mVM.GetProc().GetRegisterHex(REG_SP)); }
void MemWnd::bpChanged() { if (QMemModel::_validHexText(this->ui->txtBP->text(), 4)) { mVM.GetProc().SetRegister(REG_BP, QMemModel::_htoi(this->ui->txtBP->text())); } ui->txtBP->setText(mVM.GetProc().GetRegisterHex(REG_BP)); }
void MemWnd::siChanged() { if (QMemModel::_validHexText(this->ui->txtSI->text(), 4)) { mVM.GetProc().SetRegister(REG_SI, QMemModel::_htoi(this->ui->txtSI->text())); } ui->txtSI->setText(mVM.GetProc().GetRegisterHex(REG_SI)); }
void MemWnd::diChanged() { if (QMemModel::_validHexText(this->ui->txtDI->text(), 4)) { mVM.GetProc().SetRegister(REG_DI, QMemModel::_htoi(this->ui->txtDI->text())); } ui->txtDI->setText(mVM.GetProc().GetRegisterHex(REG_DI)); }
void MemWnd::flChanged() { if (QMemModel::_validHexText(this->ui->txtFLAGS->text(), 4)) { mVM.GetProc().SetRegister(REG_FLAGS, QMemModel::_htoi(this->ui->txtFLAGS->text())); } ui->txtFLAGS->setText(mVM.GetProc().GetRegisterHex(REG_FLAGS)); UpdateFlags(); }

void MemWnd::ipChanged() {
	if (!this->ui->txtIP->text().compare(mVM.GetProc().GetRegisterHex(REG_IP)))
		return;

	disconnect(this->ui->txtIP, SIGNAL(editingFinished()), this, SLOT(ipChanged()));

	int reply = QMessageBox::Yes;

	if (!this->ipWarned) {
		reply = QMessageBox::warning(this, "IP Changing", "Changing the value of IP will likely cause instruction highlighting to misbehave.\n\t\tDo you want to continue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (reply == QMessageBox::Yes)
			ipWarned = true;
	}


	if (reply == QMessageBox::Yes && QMemModel::_validHexText(this->ui->txtIP->text(), 4)) {
		if (!mVM.GetProc().ForceReloadInstruction(QMemModel::_htoi(this->ui->txtIP->text()))) {
			QMessageBox::critical(this, "Invalid Instruction at new IP", "The instruction at the new IP is not valid.\nReverting IP to previous value.");
			ui->txtIP->setText(mVM.GetProc().GetRegisterHex(REG_IP));
		} else {
			this->UpdateGui();
		}
	} else {
		ui->txtIP->setText(mVM.GetProc().GetRegisterHex(REG_IP));
	}

	connect(this->ui->txtIP, SIGNAL(editingFinished()), this, SLOT(ipChanged()));
}

///End register update functions

void MemWnd::memChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
	QMemModel* model = (QMemModel*)this->ui->tableView->model();

	if(model->isDirty() && topLeft == bottomRight) {
		mVM.GetProc().SetMemory(topLeft.column() + topLeft.row() *  0x10, 1, (unsigned int)model->data(topLeft, Qt::UserRole).toChar().toAscii());
	}
}

void MemWnd::SetMemoryEditState(bool editable) {
	if(editable) {
		ui->tableView->setFocusPolicy(Qt::WheelFocus);
	} else {
		ui->tableView->setFocusPolicy(Qt::NoFocus);
	}
	((QMemModel*)ui->tableView->model())->setEditable(editable);
}

void MemWnd::openHelp() {
    QDesktopServices::openUrl(QUrl("Docs\\index.html"));
}

void MemWnd::deviceError(IPeripheral* dev, unsigned int err) {
	switch(err) {
	case IPeripheral::SCREEN_UNPRINTABLE:
		{
			QMessageBox::warning(this, "Printing Unprintable Character", "You are attempting to print a character that has no visible representation.\nThis is likely caused by forgetting to convert a numeric value to it's ASCII equivalent.");
		}
	}
}

void MemWnd::openAbout() {
    QMessageBox::information(this, "Placeholder About Dialog", VERSION "\n" HOSTNAME );
}
