#ifndef RELAYWINDOW_H
#define RELAYWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"

//static const int GRAPH_CNTN_PER_TAB = 4;
//static const int n = ceil(sqrt(N));
//gridLayout[i]->addWidget(b[j], i/n, i%n, 1, 1);
static const int GRAPH_X_CNT = 2;
static const int GRAPH_Y_CNT = 2;
static const int GRAPH_TABCNT = 4;
static const int GRAPH_TOTAL = GRAPH_X_CNT * GRAPH_Y_CNT * GRAPH_TABCNT;

//---------------------------------------------------
//
// macros
//





namespace Ui {
class RelayWindow;
}

class RelayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RelayWindow(QWidget *parent = 0);
    ~RelayWindow();

    void log(QString text);




private:
    Ui::RelayWindow *ui;

    QPlainTextEdit* plainTextEdit_log_window;


    QTabWidget* tabWidget;

    QWidget *tab[GRAPH_TABCNT];

    QGridLayout *gridLayout[GRAPH_TABCNT];

    QCustomPlot * graph[GRAPH_TOTAL + 1];


    //ClockView *clockViews[N];
    //QLayout *layouts[N];


};

#endif // RELAYWINDOW_H
