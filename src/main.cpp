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

int main(int argc, char* argv[]) {

    QApplication a(argc, argv);
    MemWnd w;
    w.show();

    return a.exec();
}
