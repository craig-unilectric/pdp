#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"



//---------------------------------------------------
//
// macros
//

#define PROCESS_WIN_EVENTS()  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents)

// Log window
#define Log(x)  mainWin->log(x)

// number conversions for log window
#define IntToStr(n)                     QString::number(n)
#define IntToHex(n)                     QString::number(n, 16)

//---------------------------------------------------
//
//
//

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    void log(QString text);
    void setProgress(int percentage);
    void idleProgress(void);
    QProgressBar *statusBarProgress;


private:


    QPlainTextEdit* plainTextEdit_log_window;


    //QTabWidget* tabWidget;

    //QWidget *tab[GRAPH_TABCNT];

    //QGridLayout *gridLayout[GRAPH_TABCNT];




    void addStatusBarProgress(int statusBarWidth, int statusBarHeight);


};

#endif // MAINWINDOW_H
