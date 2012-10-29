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
	//action slots
	void loadObjFile();
	void reloadObjFile();
	void stepInVM();
	void stepOutVM();
	void stepOverVM();
	void runVM();

	//VMWorker Slots
	//trigger on breakpoint
	void vmBreakpoint();
	//trigger on run finished
	void vmDone();
	//trigger on step completion
	void stepDone();
	//trigger on run error
	void vmRunError(int err);

signals:
	void vmResume();
	void vmStep();

private:
	Ui::MemWnd *ui;

	void UpdateGui();
	void UpdateMemView();
	void DisableRun(int err);
	void EnableRun();

	VM mVM;
	QString mFile;

};

#endif // MEMWND_HPP
