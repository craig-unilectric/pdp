#include "mainwindow.h"
#include <QApplication>

#include "host.h"
#include "usb_vcp.h"

#include <windows.h> // for Sleep()


//
// Globals for *_c.cpp (C files) access
//
// Global for Log(), ProgressBar(), StatusBar()
MainWindow *mainWin; // for Log(), ProgressBar(), StatusBar()
// Global for USB-VCP
USB_VCP *usbVCP;




int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Unilectirc Inc.");
    app.setApplicationName("PDP");
    mainWin = new MainWindow;
    //
    // USB comm port driver
    usbVCP = new USB_VCP(mainWin);
    //
    // User controls and processing
    Host *host = new Host(mainWin, usbVCP);
    //
    // Execute app.exec();
    mainWin->show();
    int returnValue = app.exec();
    //
    // Delete globals, but not mainWin
    //delete usbVCP;
    //delete host;
    //
    return returnValue;
}
