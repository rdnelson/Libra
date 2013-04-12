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
#include "MainWnd8086.hpp"

#ifdef QICO_NEEDED
#include <QtPlugin>
Q_IMPORT_PLUGIN(qico)
#endif

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);
	MainWnd8086* w = 0;
	if(argc > 1) {
		w = new MainWnd8086(argv[argc - 1]);
	} else {
		w = new MainWnd8086();
	}
	w->show();

	return a.exec();
}
