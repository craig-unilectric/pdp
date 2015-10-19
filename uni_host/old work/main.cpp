#include "mainwindow.h"
#include <QApplication>


//
// Globals for *_c.cpp (C files) access
//
// Global for Log(), ProgressBar(), StatusBar()
MainWindow *mainWin; // for Log(), ProgressBar(), StatusBar()
// Global for USB-VCP
//USB_VCP *usbVCP;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainWin = new MainWindow;





    mainWin->show();

    return a.exec();
}
