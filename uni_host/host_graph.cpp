//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: host_graph.cpp
Date: 2015
User: Craig Sullender

Class name: host
Env. var.:
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


#include <QPainter>
#include <QtCore/qmath.h>
#include <QLineF>
#include <QTime>


#include "host.h"
#include "ui_mainwindow.h"
#include "mainwindow.h" // for Log() and file and display functions
extern MainWindow *mainWin; // for Log(), ProgressBar(), StatusBar()
#include <windows.h> // for Sleep()
#include <math.h> // for fmod(), sqrt(), abs(), log2(), pow()

//#include "usb_i2c.h"
#include "usb_vcp.h"

#include "pdp_command_c.h" // for data types and structs
//#include "fc_command_c.h" // for data types and structs
//extern FeatureList fl;
//extern RegionList rl;
//extern ColorWindowLUT cwlut[]; // in fc_command_c.h
#include "fc_comport_c.h" // for Host to device serial port calls and TX_PACKET_SIZE_HOST


#include "pdp_command_c.h" // for data types and structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;




//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
// Graph
//
//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


#define GRAPH_NAME_AC   0
#define GRAPH_NAME_RMS  1
#define GRAPH_NAME_PK   2
#define GRAPH_NAME_DC   3
#define GRAPH_NAME_MAX  2
#define GRAPH_NAME_MIN  4


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//
//
//

void Host::clear_metrics(int channel)
{

    // change graph title
    pdpGraphTitle->setText("PDP Channel " + QString::number(channel));

    uint16 val16;
    QPen pen2;
    pen2.setWidth(2);
    pen2.setCosmetic(false);

    int size = 100;// dc values, graphing just one value
    int xrange = 100;   // seconds
    int yrange = 4100;  // adc

    QVector<double> x(1001), y(1001);

    int graphName;

    pdpGraph->xAxis2->setRange(0, xrange);
    pdpGraph->yAxis2->setRange(0, yrange);
    pdpGraph->xAxis2->setVisible(true);
    pdpGraph->yAxis2->setVisible(true);

    //--------------------------------------
    //--------------------------------------
    graphName = GRAPH_NAME_DC;
    //pen2.setColor(Qt::magenta);
    //pdpGraph->graph(graphName)->setPen(pen2);
    for(int i = 0;i < size;i++){
        val16 = (uint16)0;
        x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;
    }
    pdpGraph->graph(graphName)->setData(x, y);
    //--------------------------------------
    //--------------------------------------
    graphName = GRAPH_NAME_RMS;
    //pen2.setColor(Qt::green);
    //pdpGraph->graph(graphName)->setPen(pen2);
    for(int i = 0;i < size;i++){
        val16 = (uint16)0;
        //x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;
    }
    pdpGraph->graph(graphName)->setData(x, y);
    //--------------------------------------
    //--------------------------------------
    graphName = GRAPH_NAME_PK;
    //pen2.setColor(Qt::red);
    //pdpGraph->graph(graphName)->setPen(pen2);
    for(int i = 0;i < size;i++){
        val16 = (uint16)0;
        //x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;
    }
    pdpGraph->graph(graphName)->setData(x, y);
    //--------------------------------------
    //--------------------------------------


    pdpGraph->replot();


}


void Host::show_metrics(int channel)
{

    // change graph title
    pdpGraphTitle->setText("PDP Channel " + QString::number(channel));

    uint16 val16;
    QPen pen2;
    pen2.setWidth(2);
    pen2.setCosmetic(false);

    int size = 100;// dc values, graphing just one value
    int xrange = 100;   // seconds
    int yrange = 4100;  // adc

    QVector<double> x(1001), y(1001);

    int graphName;

    pdpGraph->xAxis2->setRange(0, xrange);
    pdpGraph->yAxis2->setRange(0, yrange);
    pdpGraph->xAxis2->setVisible(true);
    pdpGraph->yAxis2->setVisible(true);

    //--------------------------------------
    //--------------------------------------
    graphName = GRAPH_NAME_DC;
    //pen2.setColor(Qt::magenta);
    //pdpGraph->graph(graphName)->setPen(pen2);
    for(int i = 0;i < size;i++){
        val16 = (uint16)ps.dc[channel];
        x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;
    }
    pdpGraph->graph(graphName)->setData(x, y);
    //--------------------------------------
    //--------------------------------------
    graphName = GRAPH_NAME_RMS;
    //pen2.setColor(Qt::green);
    //pdpGraph->graph(graphName)->setPen(pen2);
    for(int i = 0;i < size;i++){
        val16 = (uint16)ps.rms[channel];
        //x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;
    }
    pdpGraph->graph(graphName)->setData(x, y);
    //--------------------------------------
    //--------------------------------------
    graphName = GRAPH_NAME_PK;
    //pen2.setColor(Qt::red);
    //pdpGraph->graph(graphName)->setPen(pen2);
    for(int i = 0;i < size;i++){
        val16 = (uint16)ps.pk[channel];
        //x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;
    }
    pdpGraph->graph(graphName)->setData(x, y);
    //--------------------------------------
    //--------------------------------------


    pdpGraph->replot();


}



void Host::graph_dc(int channel, uint16 *buf, uint32 size)
{

    // change graph title
    pdpGraphTitle->setText("PDP Channel " + QString::number(channel));

    uint16 val16;
    QPen pen2;
    pen2.setColor(QColor(255, 200, 0, 255));
    pen2.setWidth(2);
    pen2.setCosmetic(false);

    int xrange = 100;   // seconds
    int yrange = 4200;  // adc

    int graphName = GRAPH_NAME_DC;

    pdpGraph->xAxis2->setRange(0, xrange);
    pdpGraph->yAxis2->setRange(0, yrange);
    pdpGraph->xAxis2->setVisible(true);
    pdpGraph->yAxis2->setVisible(true);
    pdpGraph->graph(graphName)->setPen(pen2);

    QVector<double> x(1001), y(1001);

    for(int i = 0;i < size;i++){
        val16 = (uint16)buf[i];
        //Log("    " + IntToStr(val32));

        x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;

    }

    pdpGraph->graph(graphName)->setData(x, y);

    pdpGraph->replot();


}





void Host::graph_ac(int channel, int16 *buf, uint32 size)
{

    // change graph title
    pdpGraphTitle->setText("PDP Channel " + QString::number(channel));

    int16 val16;
    QPen pen2;
    pen2.setColor(Qt::blue);
    pen2.setWidth(2);
    pen2.setCosmetic(false);

    int xrange = 17;//ms
    int yrange = 4200;//adc 12 bit = 4096

    int graphName = GRAPH_NAME_AC;

    pdpGraph->xAxis->setRange(0, xrange);
    pdpGraph->yAxis->setRange(-yrange/2, yrange/2);
    pdpGraph->xAxis->setVisible(true);
    pdpGraph->yAxis->setVisible(true);
    pdpGraph->graph(graphName)->setPen(pen2);


    QVector<double> x(1001), y(1001);

    for(int i = 0;i < size;i++){
        val16 = (int16)buf[i];
        //Log("    " + IntToStr(val16));
        //if(i < 10) Log(IntToStr(i) + "    " + IntToStr(val16));

        x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;

    }

    pdpGraph->graph(graphName)->setData(x, y);

    pdpGraph->replot();

}




void Host::graph_ac_with_offset(int channel, uint16 *buf, uint32 size)
{

    // change graph title
    pdpGraphTitle->setText("PDP Channel " + QString::number(channel));

    uint16 val16;
    QPen pen2;
    pen2.setColor(Qt::blue);
    pen2.setWidth(2);
    pen2.setCosmetic(false);

    int xrange = 17;//ms
    int yrange = 4200;//adc 12 bit = 4096

    int graphName = GRAPH_NAME_AC;

    pdpGraph->xAxis->setRange(0, xrange);
    pdpGraph->yAxis->setRange(0, yrange);
    pdpGraph->xAxis->setVisible(true);
    pdpGraph->yAxis->setVisible(true);
    pdpGraph->graph(graphName)->setPen(pen2);


    QVector<double> x(1001), y(1001);

    for(int i = 0;i < size;i++){
        val16 = (uint16)buf[i];
        //Log("    " + IntToStr(val16));
        //if(i < 10) Log(IntToStr(i) + "    " + IntToStr(val16));

        x[i] = (double)i * ((double)xrange/(double)size);
        y[i] = val16;

    }

    pdpGraph->graph(graphName)->setData(x, y);

    pdpGraph->replot();

}




//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
//


//--------------------------------------------------------------
//--------------------------------------------------------------
//
// init graph
//

void Host::graph_init(void)
{


    ui->label_rms->setText("...");
    ui->label_peak->setText("...");
    ui->label_ctdc->setText("...");
    ui->label_max->setText("...");
    ui->label_min->setText("...");

    ui->label_5->setVisible(false);
    ui->label_ctdc->setVisible(false);

    ui->label_3->setVisible(false);
    ui->label_max->setVisible(false);

    ui->label_4->setVisible(false);
    ui->label_min->setVisible(false);



// generate some data
QVector<double> xx(101), yy(101), zz(101), max(101), min(101), dc(101);
for (int i=0; i<101; ++i)
{
  xx[i] = i/50.0 - 1; // x goes from -1 to 1
  yy[i] = xx[i]*xx[i]; // let's plot a quadratic function
  zz[i] = yy[i]/2;
  max[i] = 0.7;
  min[i] = 0.1;
  dc[i] = 0.2 + i/100;
}

// set graph layout
QHBoxLayout* graphLayout = new QHBoxLayout(ui->frame_3);
ui->frame_3->setLayout(graphLayout);

int channel = 0;

// create graph and add to layout
pdpGraph = new QCustomPlot();
graphLayout->addWidget(pdpGraph);

// add title
pdpGraph->plotLayout()->insertRow(0);
pdpGraphTitle = new QCPPlotTitle(pdpGraph, "PDP Channel " + QString::number(channel));
pdpGraph->plotLayout()->addElement(0, 0, pdpGraphTitle);



// add legend
// period as decimal separator and comma as thousand separator
pdpGraph->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
pdpGraph->legend->setVisible(true);
QFont legendFont = font();  // start out with MainWindow's font..
legendFont.setPointSize(9); // and make a bit smaller for legend
//legendFont.setPointSize(legendFont.pointSize() + 1);
pdpGraph->legend->setFont(legendFont);
pdpGraph->legend->setBrush(QBrush(QColor(255,255,255,230)));
// the legend is in the main axis rect.
pdpGraph->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
pdpGraph->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
// the legend is outside the main axis rect.
//pdpGraph->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
//pdpGraph->axisRect()->insetLayout()->setInsetRect(0, QRectF(0.6, 0.1, 0.35, 0.35));

// give the axes some labels
pdpGraph->xAxis->setLabel("Time mSec");
pdpGraph->yAxis->setLabel("Amps AC");
pdpGraph->xAxis2->setLabel("Time Sec");
pdpGraph->yAxis2->setLabel("Amps RMS");


// set axes ranges, so we see all data
pdpGraph->xAxis->setRange(-1, 1);
pdpGraph->yAxis->setRange(0, 1);
pdpGraph->xAxis2->setRange(-1, 1);
pdpGraph->yAxis2->setRange(0, 1);

pdpGraph->xAxis2->setVisible(true);
pdpGraph->yAxis2->setVisible(true);

QPen pen2;
pen2.setColor(Qt::blue);
pen2.setWidth(2);
pen2.setCosmetic(false);

// add data
//pdpGraph->addGraph();
pdpGraph->addGraph(pdpGraph->xAxis, pdpGraph->yAxis);
//pdpGraph->graph(0)->setPen(QPen(Qt::blue));
//pdpGraph->graph(0)->setPen(QPen(QColor(255, 255, 0, 255)));
pen2.setColor(Qt::blue);
pdpGraph->graph(GRAPH_NAME_AC)->setPen(pen2);
pdpGraph->graph(GRAPH_NAME_AC)->setName("AC");
pdpGraph->graph(GRAPH_NAME_AC)->setData(xx, yy);

// add data
//pdpGraph->addGraph();
pdpGraph->addGraph(pdpGraph->xAxis2, pdpGraph->yAxis2);
//pdpGraph->graph(1)->setPen(QPen(Qt::green));
pen2.setColor(Qt::green);
pdpGraph->graph(GRAPH_NAME_RMS)->setPen(pen2);
pdpGraph->graph(GRAPH_NAME_RMS)->setName("RMS");
pdpGraph->graph(GRAPH_NAME_RMS)->setData(xx, zz);

// add data
//pdpGraph->addGraph();
pdpGraph->addGraph(pdpGraph->xAxis2, pdpGraph->yAxis2);
//pdpGraph->graph(2)->setPen(QPen(Qt::red));
pen2.setColor(Qt::red);
pdpGraph->graph(GRAPH_NAME_PK)->setPen(pen2);
pdpGraph->graph(GRAPH_NAME_PK)->setName("Pk");
pdpGraph->graph(GRAPH_NAME_PK)->setData(xx, max);

// add data
//pdpGraph->addGraph();
pdpGraph->addGraph(pdpGraph->xAxis2, pdpGraph->yAxis2);
pen2.setColor(Qt::magenta);
pdpGraph->graph(GRAPH_NAME_DC)->setPen(pen2);
pdpGraph->graph(GRAPH_NAME_DC)->setName("CT-DC");
pdpGraph->graph(GRAPH_NAME_DC)->setData(xx, dc);

/*
// add data
//pdpGraph->addGraph();
pdpGraph->addGraph(pdpGraph->xAxis2, pdpGraph->yAxis2);
//pdpGraph->graph(2)->setPen(QPen(Qt::red));
pen2.setColor(Qt::red);
pdpGraph->graph(GRAPH_NAME_MAX)->setPen(pen2);
pdpGraph->graph(GRAPH_NAME_MAX)->setName("Max");
pdpGraph->graph(GRAPH_NAME_MAX)->setData(xx, max);

// add data
//pdpGraph->addGraph();
pdpGraph->addGraph(pdpGraph->xAxis2, pdpGraph->yAxis2);
//pdpGraph->graph(3)->setPen(QPen(Qt::magenta));
pen2.setColor(QColor(255, 200, 0, 255));
pdpGraph->graph(GRAPH_NAME_MIN)->setPen(pen2);
pdpGraph->graph(GRAPH_NAME_MIN)->setName("Min");
pdpGraph->graph(GRAPH_NAME_MIN)->setData(xx, min);
*/

//pdpGraph->hide();



//------------------------------------------
/*
const int bufsize = 80000;
uint8 data_in[bufsize];

uint32 *val32_ptr;
uint32 val32;


for(int i = 0;i < 1000;i++){
    val32_ptr = (uint32*)&data_in[i * 64];
    *val32_ptr = 1000;//i * 4;
}



//int channel = 0;
Graph->xAxis->setRange(0, 1000);
Graph->yAxis->setRange(0, 4000);
Graph->xAxis->setVisible(true);
Graph->yAxis->setVisible(true);

QVector<double> x(1001), y(1001);

for(int i = 0;i < 1000;i++){
    val32_ptr = (uint32*)&data_in[i * 64];
    val32 = *val32_ptr;
    //Log("    " + IntToStr(val32));

    x[i] = i;
    y[i] = i * 4;//val32;

}

Graph->addGraph(Graph->xAxis, Graph->yAxis);
Graph->graph(0)->setPen(QPen(QColor(255, 0, 0, 255)));
Graph->graph(0)->setData(x, y);
*/

//------------------------------------------

}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
