// main.cpp
#include "gui1.h"
#include "webpagedisplay.h"
#include "errorwindow.h"
#include "querywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI1 mainWindow;
    ErrorWindow ew;

    QObject::connect(&mainWindow,&GUI1::searchInpError,&ew,&ErrorWindow::searchError);
    QObject::connect(&mainWindow,&GUI1::showError,&ew,&ErrorWindow::show);

    mainWindow.show();
    return a.exec();
}
