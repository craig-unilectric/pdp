//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
File name: host.h
Date: 3/9/2014
User: Craig Sullender

Class name: host
Env. var.:
*/
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#ifndef HOST_H
#define HOST_H
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


#include <QWidget>
#include <QMainWindow>


#include "qcustomplot.h"


#include "mainwindow.h" // UI
#include "usb_vcp.h"


#include "pdp_command_c.h" // for data types



//static const int GRAPH_CNTN_PER_TAB = 4;
//static const int n = ceil(sqrt(N));
//gridLayout[i]->addWidget(b[j], i/n, i%n, 1, 1);
static const int GRAPH_X_CNT = 2;
static const int GRAPH_Y_CNT = 2;
static const int GRAPH_TABCNT = 4;
static const int GRAPH_TOTAL = GRAPH_X_CNT * GRAPH_Y_CNT * GRAPH_TABCNT;

static const int RELAY_COUNT = 16;


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class Host : public QWidget
{
    Q_OBJECT

public:
    explicit Host(QWidget *MainWin, USB_VCP *usbObj);
    ~Host();








signals:



public slots:



private slots:

    // Comms tab, host_comms.cpp
    void comms_open_selected(QListWidgetItem * item);
    bool comms_open(void);
    void comms_test(void);


    // host.cpp
    //void newTabSelected(int tabIndex = 0);


    // host_pdp.cpp
    void read_DMA(void);
    void read_channelDMA(void);
    void read_channelDMA_filtered(void);
    void write_command(void);
    void read_status(void);
    void read_status_channel(uint8 channel);


    // host_relay.cpp
    void relay_selected(void);
    void relay_read(void);
    void relay_on(void);
    void relay_off(void);







protected:




private:


    // Comms tab, host_comms.cpp
    void read_chipID(void);
    bool txrx_test(void);
    bool write_file_to_device(uint8 command, uint32 size, uint8* buf);



    // host.cpp
    MainWindow *mainWin;
    Ui::MainWindow *ui;
    USB_VCP *usb_vcp;

    QRadioButton* relaySelectButton[RELAY_COUNT];
    int relayNumberSelected;


    QCustomPlot* pdpGraph;
    //QCustomPlot* graph[GRAPH_TOTAL + 1];
    QCPPlotTitle* pdpGraphTitle;


    // host_graph.cpp
    void graph_init(void);
    void graph_ac_with_offset(int channel, uint16 *buf, uint32 size);
    void graph_ac(int channel, int16 *buf, uint32 size);
    void graph_dc(int channel, uint16 *buf, uint32 size);
    void show_metrics(int channel);
    void clear_metrics(int channel);


    // host_relay.cpp
    int get_relay_selected(void);




};

#endif // HOST_H
