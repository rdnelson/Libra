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

#ifdef __linux
#include <signal.h>
#endif

#ifdef QICO_NEEDED
#include <QtPlugin>
Q_IMPORT_PLUGIN(qico)
#endif

void sigterm(int) {
    QApplication::exit(0);
}

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);

    // Handle SIGTERM on linux (mostly for coverage reports)
#ifdef __linux
    struct sigaction sa;
    sa.sa_handler = sigterm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);
#endif

	MemWnd* w = 0;
	if(argc > 1) {
		w = new MemWnd(argv[argc - 1]);
	} else {
		w = new MemWnd();
	}
	w->show();

	return a.exec();
}
