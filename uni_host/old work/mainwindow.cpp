#include "mainwindow.h"
#include "ui_mainwindow.h"






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //mMyNewWindow = new NewWindow(); // Be sure to destroy you window somewhere
    //   mMyNewWindow->show();







    // init window
    this->setWindowTitle("Unilectric PDP");


    // Set window size and position
    QDesktopWidget *dw = qApp->desktop(); // info on all screens
    QRect screen  = dw->availableGeometry(this); // size of screen containing this widget
    int height = 3 * screen.height()/4;
    int width = 1.5 * height;
    int top = 30;
    int left = screen.width() - width - 10;
    this->setGeometry(left, top, width, height);


    // main widget and layout
    ui->centralWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QGridLayout* centralWidgetLayout = new QGridLayout(ui->centralWidget);
    ui->centralWidget->setLayout(centralWidgetLayout);


    // add control (buttons) window
    /*
    QPlainTextEdit* plainTextEdit3 = new QPlainTextEdit(ui->centralWidget);
    centralWidgetLayout->addWidget(plainTextEdit3, 0, 0, 2, -1);
    plainTextEdit3->appendPlainText("window 3");
    */

/*
    QGroupBox* groupbox_control = new QGroupBox(ui->centralWidget);
    centralWidgetLayout->addWidget(groupbox_control, 0, 0, 2, -1);
    QGridLayout* controlWidgetLayout = new QGridLayout(groupbox_control);
    groupbox_control->setLayout(controlWidgetLayout);
    QPushButton* button_connect = new QPushButton(groupbox_control);
    controlWidgetLayout->addWidget(button_connect, 0, 0, 1, 1);



    // add tab (graphs) window
    tabWidget = new QTabWidget(ui->centralWidget);
    centralWidgetLayout->addWidget(tabWidget, 2, 0, -1, 1);


    // add log (text) window
    plainTextEdit_log_window = new QPlainTextEdit(ui->centralWidget);
    centralWidgetLayout->addWidget(plainTextEdit_log_window, 2, 2, -1, 2);
    plainTextEdit_log_window->appendPlainText("window 2");


    centralWidgetLayout->setRowStretch(0, 0);
    centralWidgetLayout->setRowStretch(1, 0);
    centralWidgetLayout->setRowStretch(2, 0);



    */



    /*
    QWidget* rightCentralWidget = new QWidget(ui->centralWidget);
    //rightCentralWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    centralWidgetLayout->addWidget(rightCentralWidget);

    QVBoxLayout* rightCentralWidgetLayout = new QVBoxLayout(rightCentralWidget);
    rightCentralWidget->setLayout(rightCentralWidgetLayout);

    QPushButton* b = new QPushButton(rightCentralWidget);
    //b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rightCentralWidgetLayout->addWidget(b);
    QPushButton* b2 = new QPushButton(rightCentralWidget);
    //b2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    rightCentralWidgetLayout->addWidget(b2);
    */




    /*

    // init log window
    plainTextEdit_log_window = new QPlainTextEdit(ui->centralWidget);

    //log_window->resize(100, 400);
    //log_window->setGeometry(0, 0, 20, 100);

    //QSizePolicy spRight(QSizePolicy::Fixed, QSizePolicy::Preferred);
    //QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //spRight.setHorizontalStretch(1);
    plainTextEdit_log_window->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    centralWidgetLayout->addWidget(plainTextEdit_log_window);

    // Init Log Window
    plainTextEdit_log_window->setReadOnly(true);
    // visible cursor
    plainTextEdit_log_window->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    // log app name and date
    this->log("IoTcam Host");
    this->log("");
    this->log((QString)QFileInfo(qApp->applicationFilePath()).absoluteFilePath());
    this->log(QDateTime::currentDateTime().toString());
    // log system byte order
    //if(QSysInfo::ByteOrder == QSysInfo::BigEndian) this->log(tr("System byte order - big endian"));
    //else this->log(tr("System byte order - little endian"));
    this->log("");
*/

/*

    //---------------------------------------------------------------------
    //
    // Graphs in tabs
    //
    // generate some data
    QVector<double> xx(101), yy(101), zz(101), max(101), min(101);
    for (int i=0; i<101; ++i)
    {
      xx[i] = i/50.0 - 1; // x goes from -1 to 1
      yy[i] = xx[i]*xx[i]; // let's plot a quadratic function
      zz[i] = yy[i]/2;
      max[i] = 0.7;
      min[i] = 0.1;
    }

    // put graphs in tabs
    // tabs are numbered: 1, 2, ...
    for (int i = 0; i < GRAPH_TABCNT; i++) {
        tab[i + 1] = new QWidget();
        tabWidget->addTab(tab[i + 1], QString::number(i + 1));

        gridLayout[i] = new QGridLayout(tab[i + 1]);
        tab[i + 1]->setLayout(gridLayout[i]);

        // add graphs to each tab
        // graphs are numbered: 1, 2, ... to match relay numbering
        for (int y = 0; y < GRAPH_Y_CNT; y++) {
            for (int x = 0; x < GRAPH_X_CNT; x++) {
                int gnum = x + (GRAPH_X_CNT * y) + (GRAPH_X_CNT * GRAPH_Y_CNT * i);
                int gname = gnum + 1;
                graph[gname] = new QCustomPlot();
                // add title
                graph[gname]->plotLayout()->insertRow(0);
                graph[gname]->plotLayout()->addElement(0, 0, new QCPPlotTitle(graph[gname], "PDP Channel " + QString::number(gname)));

                // add legend
                // period as decimal separator and comma as thousand separator
                graph[gname]->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
                graph[gname]->legend->setVisible(true);
                QFont legendFont = font();  // start out with MainWindow's font..
                legendFont.setPointSize(9); // and make a bit smaller for legend
                //legendFont.setPointSize(legendFont.pointSize() + 1);
                graph[gname]->legend->setFont(legendFont);
                graph[gname]->legend->setBrush(QBrush(QColor(255,255,255,230)));
                // the legend is in the main axis rect.
                graph[gname]->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
                graph[gname]->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
                // the legend is outside the main axis rect.
                //graph[gname]->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
                //graph[gname]->axisRect()->insetLayout()->setInsetRect(0, QRectF(0.6, 0.1, 0.35, 0.35));


                // give the axes some labels
                graph[gname]->xAxis->setLabel("Time mSec");
                graph[gname]->yAxis->setLabel("Amps AC");
                graph[gname]->xAxis2->setLabel("Time Sec");
                graph[gname]->yAxis2->setLabel("Amps RMS");
                // set axes ranges, so we see all data
                graph[gname]->xAxis->setRange(-1, 1);
                graph[gname]->yAxis->setRange(0, 1);
                graph[gname]->xAxis2->setRange(-1, 1);
                graph[gname]->yAxis2->setRange(0, 1);


                graph[gname]->xAxis2->setVisible(true);
                graph[gname]->yAxis2->setVisible(true);


                QPen pen2;
                pen2.setColor(Qt::blue);
                pen2.setWidth(2);
                pen2.setCosmetic(false);


                // add data
                //graph[gname]->addGraph();
                graph[gname]->addGraph(graph[gname]->xAxis, graph[gname]->yAxis);
                //graph[gname]->graph(0)->setPen(QPen(Qt::blue));
                //graph[gname]->graph(0)->setPen(QPen(QColor(255, 255, 0, 255)));
                pen2.setColor(Qt::blue);
                graph[gname]->graph(0)->setPen(pen2);
                graph[gname]->graph(0)->setName("AC");
                graph[gname]->graph(0)->setData(xx, yy);


                // add data
                //graph[gname]->addGraph();
                graph[gname]->addGraph(graph[gname]->xAxis2, graph[gname]->yAxis2);
                //graph[gname]->graph(1)->setPen(QPen(Qt::green));
                pen2.setColor(Qt::green);
                graph[gname]->graph(1)->setPen(pen2);
                graph[gname]->graph(1)->setName("RMS");
                graph[gname]->graph(1)->setData(xx, zz);


                // add data
                //graph[gname]->addGraph();
                graph[gname]->addGraph(graph[gname]->xAxis2, graph[gname]->yAxis2);
                //graph[gname]->graph(2)->setPen(QPen(Qt::red));
                pen2.setColor(Qt::red);
                graph[gname]->graph(2)->setPen(pen2);
                graph[gname]->graph(2)->setName("Max");
                graph[gname]->graph(2)->setData(xx, max);


                // add data
                //graph[gname]->addGraph();
                graph[gname]->addGraph(graph[gname]->xAxis2, graph[gname]->yAxis2);
                //graph[gname]->graph(3)->setPen(QPen(Qt::magenta));
                pen2.setColor(Qt::magenta);
                graph[gname]->graph(3)->setPen(pen2);
                graph[gname]->graph(3)->setName("Min");
                graph[gname]->graph(3)->setData(xx, min);



                gridLayout[i]->addWidget(graph[gname], y, x, 1, 1);
            }
        }
    }

*/

    //---------------------------------------------------------------------
    //
    //
    //
    //




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
