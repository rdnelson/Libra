/*-------------------------------------*\
|
|  File Name: main.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Tue, Sep 25, 2012  4:13:11 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include <QApplication>
#include "MemWnd.hpp"

#ifdef QICO_NEEDED
#include <QtPlugin>
Q_IMPORT_PLUGIN(qico)
#endif

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);
	MemWnd* w = 0;
	int remainArgs = argc;
	bool testMode = false;

	for(int i = 1; i < argc; i++) {
		if (strncmp("-t", argv[i], 2) == 0) {
			testMode = true;
			remainArgs--;
		}
	}

	if(remainArgs > 1) {
		w = new MemWnd(argv[argc - 1]);
	} else {
		w = new MemWnd();
	}
	if (testMode)
		w->setTestMode();

	w->show();

	return a.exec();
}
