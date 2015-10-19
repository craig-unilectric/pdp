#include "mainwindow.h"
#include "ui_mainwindow.h"






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    // init main window
    //
    this->setWindowTitle("Unilectric PDP");
    // Set fixed window size
    QSize windowsize = this->size();
    this->setFixedSize(windowsize);
    this->setFixedSize(windowsize.width(), windowsize.height());
    // Set window position
    QDesktopWidget *dw = qApp->desktop(); // info on all screens
    QRect screen  = dw->availableGeometry(this); // size of screen containing this widget
    int winx = screen.width() - windowsize.width() - 20;
    int winy = 2;
    this->move(winx,winy);


    //
    // init status bar
    //
    int statusBarWidth = windowsize.width();
    int statusBarHeight = 20;
    addStatusBarProgress(statusBarWidth, statusBarHeight);

    //
    // init log window
    //
    plainTextEdit_log_window = ui->plainTextEdit;
    plainTextEdit_log_window->setReadOnly(true);
    // visible cursor
    plainTextEdit_log_window->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    // log app name and date
    this->log("Unilectirc PDP");
    this->log("");
    this->log((QString)QFileInfo(qApp->applicationFilePath()).absoluteFilePath());
    this->log(QDateTime::currentDateTime().toString());
    // log system byte order
    //if(QSysInfo::ByteOrder == QSysInfo::BigEndian) this->log(tr("System byte order - big endian"));
    //else this->log(tr("System byte order - little endian"));
    this->log("");
    this->log("");
    qApp->setCursorFlashTime(5);
    plainTextEdit_log_window->setCursorWidth(4);


    // init icon display
    ui->frame_icon->setFrameStyle(QFrame::StyledPanel);
    ui->frame_icon->setStyleSheet("background-image: url(C:/Unilectric/processor/uni_host/Uni_mini_131x131.jpg)");



    //---------------------------------------------------------------------





}



MainWindow::~MainWindow()
{
    delete ui;
}


//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
void MainWindow::log(QString text)
{
    // add text to log window
    this->plainTextEdit_log_window->appendPlainText(text);
    //
    // move cursor to last line
    this->plainTextEdit_log_window->textCursor().movePosition(QTextCursor::End);
    // slider to maximum
    this->plainTextEdit_log_window->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
    //
    // Allow Windows to show the message immediately.
    PROCESS_WIN_EVENTS();
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
void MainWindow::addStatusBarProgress(int statusBarWidth, int statusBarHeight)
{
    this->statusBar()->setFixedHeight(statusBarHeight);
    this->statusBar()->setSizeGripEnabled(false);
    this->statusBar()->showMessage("Status");
    //
    statusBarProgress = new QProgressBar(statusBar());
    statusBarProgress->setFixedHeight(statusBarHeight - 7);
    statusBarProgress->setFixedWidth(statusBarWidth/4);
    //
    statusBarProgress->setTextVisible(true);
    statusBar()->addPermanentWidget(statusBarProgress, 0);
    statusBarProgress->setRange(0,100);
    statusBarProgress->setValue(100);
    // idle
    statusBarProgress->setRange(0,0);
    statusBarProgress->setValue(0);
}
//
void MainWindow::setProgress(int percentage)
{
    statusBarProgress->setRange(0,100);
    if(percentage < 0) percentage = 0;
    if(percentage > 100) percentage = 100;
    statusBarProgress->setValue(percentage);
}
//
void MainWindow::idleProgress(void)
{
    statusBarProgress->setRange(0,0);
    statusBarProgress->setValue(0);
}
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//




//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

