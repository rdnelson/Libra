#ifndef MEMWND_HPP
#define MEMWND_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QAbstractTextDocumentLayout>
#include <QKeyEvent>
#include "VM.hpp"
#include "VMWorker.hpp"

namespace Ui {
class MemWnd;
}

class MemWnd : public QMainWindow
{
	Q_OBJECT

public:
	explicit MemWnd(const char* const file = 0, QWidget *parent = 0);

	~MemWnd();

public slots:

	//GUI action slots
	void loadObjFile_Clicked();
	void reloadObjFile_Clicked();
	void stepInVM_Clicked();
	void stepOutVM_Clicked();
	void stepOverVM_Clicked();
	void runVM_Clicked();
	void appQuit_Clicked();
	void pauseVM_Clicked();
	void stopVM_Clicked();
	void toggleBreakpoint_Clicked();

	//VMWorker Slots
	//trigger on breakpoint
	void workerBreakpoint();
	//trigger on run finished
	void workerRunDone();
	//trigger on step completion
	void workerStepDone();
	//trigger on run error
	void workerRunError(int err);
	//trigger on run paused
	void workerPaused();
	//trigger on proc return positive
	void workerProcReturn(int err);

	void KeyEvent(QKeyEvent*);
	void TimerEvent();

signals:
	void vmResume();
	void vmStep();
	void vmPause();

private:
	Ui::MemWnd *ui;
	VMWorker* mVMWorker;

	void loadFile(bool newFile = false);
	void UpdateScreen();
	void UpdateGui();
	void UpdateMemView(unsigned int ip = 0xFFFFFFFF, unsigned int len = 0);
	void UpdateInstHighlight();
	void DisableRun(int err);
	void EnableRun();

	VM mVM;
	QString mFile;

};

#endif // MEMWND_HPP
