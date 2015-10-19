/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QFrame *frame_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_rms;
    QLabel *label_min;
    QLabel *label_max;
    QLabel *label_peak;
    QGroupBox *groupBox;
    QRadioButton *radioButton_relayOn;
    QRadioButton *radioButton_relayOff;
    QPushButton *pushButton_read_relay;
    QPushButton *pushButton_read_AC;
    QLabel *label_ctdc;
    QLabel *label_5;
    QPushButton *pushButton_read_DMA;
    QLabel *label_channel;
    QPushButton *pushButton_read_AC_filtered;
    QPlainTextEdit *plainTextEdit;
    QFrame *frame_2;
    QListWidget *listWidget_comms_list;
    QPushButton *pushButton_comms_test;
    QPushButton *pushButton_comms_open;
    QFrame *frame_icon;
    QPushButton *pushButton_write_command;
    QPushButton *pushButton_read_status;
    QGroupBox *groupBox_relaySelect;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1274, 920);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QLatin1String("\n"
"QMainWindow,\n"
"QMenuBar,\n"
"QMenuBar::item,\n"
"QStatusBar,\n"
"QProgressBar,\n"
"QGroupBox,\n"
"/*QFrame,*/\n"
"QSpinBox,\n"
"QLineEdit,\n"
"QCheckBox,\n"
"QMessageBox,\n"
"QLineEdit,\n"
"QRadioButton,\n"
"QPushButton,\n"
"QCheckBox,\n"
"QLabel,\n"
"/*QWidget,*/\n"
"QTabWidget,\n"
"QListWidget,\n"
"QTabBar::tab,\n"
"QPlainTextEdit\n"
"{\n"
"    color: white;\n"
"	font-family: Courier New;\n"
"	font-size: 10pt;\n"
"	font-weight: bold;\n"
"	background-color: black;\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"/* QTab Widget */\n"
"\n"
"QTabWidget::pane { /* The tab widget frame */\n"
"    border: 1px solid;\n"
"    border-color: #aaaaaa;\n"
"}\n"
"QTabBar::tab {\n"
"    border: 1px solid;\n"
"    border-color: #aaaaaa;\n"
"    background: solid;\n"
"    background-color: #444444;\n"
"   /* border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;*/\n"
"    min-width: 30ex;\n"
"    padding-left: 16px;\n"
"    padding-right: 16px;\n"
"\n"
"}\n"
"QTabBar::tab:hover {\n"
"    border: 1px solid;\n"
"    borde"
                        "r-color: #ffffff;\n"
"    background-color: black;\n"
"    padding-left: 0px;\n"
"    padding-right: 0px;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    font-size: 12pt;\n"
"    border: 1px solid;\n"
"    border-color: #ffffff;\n"
"    background-color: black;\n"
"    padding-left: 0px;\n"
"    padding-right: 0px;\n"
"}\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 4px; /* make non-selected tabs look smaller */\n"
"}\n"
"\n"
"/* QGroupBox */\n"
"\n"
"\n"
"/* QRadioButton */\n"
"\n"
"QRadioButton::!checked{\n"
"    background:transparent;\n"
"}\n"
"QRadioButton::hover{\n"
"    background:#444444;\n"
"}\n"
"QRadioButton::checked{\n"
"    background:#444444;\n"
"}\n"
"\n"
" \n"
"QLabel {\n"
"	\n"
"	font-size: 10pt;\n"
"\n"
"}\n"
"\n"
"QSpinBox {\n"
"    border: 1px solid;\n"
"    border-color: white;\n"
"	font-size: 12pt;\n"
"}\n"
"QSpinBox::hover{\n"
"    background-color: #444444;\n"
"}\n"
"\n"
"\n"
"\n"
"QLineEdit {\n"
"    border: 1px solid;\n"
"    border-color: white;\n"
"    background-color: black;\n"
"}\n"
""
                        "QLineEdit::hover{\n"
"    background-color: #444444;\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"QPushButton { \n"
"	border: 1px solid;\n"
"    /*border-radius: 2px;*/\n"
"	border-color: #aaaaaa;\n"
"    background: solid;\n"
"    background-color: black;\n"
"     min-height: 20px;\n"
"}\n"
"QPushButton:!pressed {\n"
"    background-color: #444444;\n"
"}\n"
"QPushButton:hover {\n"
"    background: transparent;\n"
"}\n"
"QPushButton:disabled {\n"
"    background: transparent;\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #aa0000;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #888888;\n"
"}\n"
"\n"
"\n"
"\n"
"QStatusBar \n"
"{\n"
"	font-size: 10pt;\n"
"	background-color: #444444;\n"
"}\n"
"        \n"
"QProgressBar \n"
"{\n"
"	color: #444444; /* if same as bkg color, hides text */\n"
"        font-size: 10pt;\n"
"}\n"
"\n"
"\n"
"QMenuBar \n"
"{\n"
"    font-size: 10pt;\n"
"    background-color: #444444;\n"
"}\n"
"\n"
"QMenuBar::item \n"
"{\n"
"    spacing: 10px; /* spacing between menu bar items */\n"
""
                        "    padding: 1px 20px; /* height width */\n"
"    background: transparent;\n"
"    border-radius: 1px;\n"
"}\n"
"\n"
"QMenuBar::item:selected \n"
"{ \n"
"}\n"
"\n"
"QMenuBar::item:pressed \n"
"{\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 170, 951, 711));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Sunken);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(10, 160, 771, 541));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(800, 170, 91, 16));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(800, 230, 91, 16));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(800, 350, 91, 16));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(800, 410, 91, 16));
        label_rms = new QLabel(frame);
        label_rms->setObjectName(QStringLiteral("label_rms"));
        label_rms->setGeometry(QRect(800, 190, 91, 16));
        label_min = new QLabel(frame);
        label_min->setObjectName(QStringLiteral("label_min"));
        label_min->setGeometry(QRect(800, 430, 91, 16));
        label_max = new QLabel(frame);
        label_max->setObjectName(QStringLiteral("label_max"));
        label_max->setGeometry(QRect(800, 370, 91, 16));
        label_peak = new QLabel(frame);
        label_peak->setObjectName(QStringLiteral("label_peak"));
        label_peak->setGeometry(QRect(800, 250, 91, 16));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 30, 171, 121));
        radioButton_relayOn = new QRadioButton(groupBox);
        radioButton_relayOn->setObjectName(QStringLiteral("radioButton_relayOn"));
        radioButton_relayOn->setGeometry(QRect(40, 60, 82, 17));
        radioButton_relayOff = new QRadioButton(groupBox);
        radioButton_relayOff->setObjectName(QStringLiteral("radioButton_relayOff"));
        radioButton_relayOff->setGeometry(QRect(40, 80, 82, 17));
        radioButton_relayOff->setChecked(true);
        pushButton_read_relay = new QPushButton(groupBox);
        pushButton_read_relay->setObjectName(QStringLiteral("pushButton_read_relay"));
        pushButton_read_relay->setGeometry(QRect(40, 30, 91, 22));
        pushButton_read_AC = new QPushButton(frame);
        pushButton_read_AC->setObjectName(QStringLiteral("pushButton_read_AC"));
        pushButton_read_AC->setGeometry(QRect(800, 60, 91, 30));
        label_ctdc = new QLabel(frame);
        label_ctdc->setObjectName(QStringLiteral("label_ctdc"));
        label_ctdc->setGeometry(QRect(800, 310, 91, 16));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(800, 290, 91, 16));
        pushButton_read_DMA = new QPushButton(frame);
        pushButton_read_DMA->setObjectName(QStringLiteral("pushButton_read_DMA"));
        pushButton_read_DMA->setEnabled(true);
        pushButton_read_DMA->setGeometry(QRect(800, 20, 91, 30));
        label_channel = new QLabel(frame);
        label_channel->setObjectName(QStringLiteral("label_channel"));
        label_channel->setGeometry(QRect(240, 40, 91, 16));
        pushButton_read_AC_filtered = new QPushButton(frame);
        pushButton_read_AC_filtered->setObjectName(QStringLiteral("pushButton_read_AC_filtered"));
        pushButton_read_AC_filtered->setGeometry(QRect(240, 70, 91, 30));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(970, 10, 301, 871));
        plainTextEdit->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(10, 10, 951, 151));
        frame_2->setFrameShape(QFrame::Panel);
        frame_2->setFrameShadow(QFrame::Raised);
        listWidget_comms_list = new QListWidget(frame_2);
        listWidget_comms_list->setObjectName(QStringLiteral("listWidget_comms_list"));
        listWidget_comms_list->setEnabled(true);
        listWidget_comms_list->setGeometry(QRect(20, 20, 181, 71));
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        listWidget_comms_list->setFont(font);
        listWidget_comms_list->setSortingEnabled(false);
        pushButton_comms_test = new QPushButton(frame_2);
        pushButton_comms_test->setObjectName(QStringLiteral("pushButton_comms_test"));
        pushButton_comms_test->setGeometry(QRect(120, 100, 81, 30));
        pushButton_comms_open = new QPushButton(frame_2);
        pushButton_comms_open->setObjectName(QStringLiteral("pushButton_comms_open"));
        pushButton_comms_open->setGeometry(QRect(20, 100, 81, 30));
        frame_icon = new QFrame(frame_2);
        frame_icon->setObjectName(QStringLiteral("frame_icon"));
        frame_icon->setGeometry(QRect(810, 10, 131, 131));
        frame_icon->setFrameShape(QFrame::StyledPanel);
        frame_icon->setFrameShadow(QFrame::Raised);
        frame_icon->setLineWidth(0);
        pushButton_write_command = new QPushButton(frame_2);
        pushButton_write_command->setObjectName(QStringLiteral("pushButton_write_command"));
        pushButton_write_command->setGeometry(QRect(260, 20, 141, 30));
        pushButton_read_status = new QPushButton(frame_2);
        pushButton_read_status->setObjectName(QStringLiteral("pushButton_read_status"));
        pushButton_read_status->setGeometry(QRect(260, 60, 141, 30));
        groupBox_relaySelect = new QGroupBox(frame_2);
        groupBox_relaySelect->setObjectName(QStringLiteral("groupBox_relaySelect"));
        groupBox_relaySelect->setGeometry(QRect(460, 5, 211, 141));
        groupBox_relaySelect->setCheckable(false);
        groupBox_relaySelect->setChecked(false);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Unilectric PDP", 0));
        label->setText(QApplication::translate("MainWindow", "RMS", 0));
        label_2->setText(QApplication::translate("MainWindow", "Peak", 0));
        label_3->setText(QApplication::translate("MainWindow", "Max", 0));
        label_4->setText(QApplication::translate("MainWindow", "Min", 0));
        label_rms->setText(QApplication::translate("MainWindow", "0", 0));
        label_min->setText(QApplication::translate("MainWindow", "0", 0));
        label_max->setText(QApplication::translate("MainWindow", "0", 0));
        label_peak->setText(QApplication::translate("MainWindow", "0", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", " Relay ", 0));
        radioButton_relayOn->setText(QApplication::translate("MainWindow", "On", 0));
        radioButton_relayOff->setText(QApplication::translate("MainWindow", "Off", 0));
        pushButton_read_relay->setText(QApplication::translate("MainWindow", "Read", 0));
        pushButton_read_AC->setText(QApplication::translate("MainWindow", "Read AC", 0));
        label_ctdc->setText(QApplication::translate("MainWindow", "0", 0));
        label_5->setText(QApplication::translate("MainWindow", "CT-DC", 0));
        pushButton_read_DMA->setText(QApplication::translate("MainWindow", "Read DMA", 0));
        label_channel->setText(QApplication::translate("MainWindow", "Channel 0", 0));
        pushButton_read_AC_filtered->setText(QApplication::translate("MainWindow", "Read AC", 0));
        plainTextEdit->setPlainText(QString());
        pushButton_comms_test->setText(QApplication::translate("MainWindow", "Test", 0));
        pushButton_comms_open->setText(QApplication::translate("MainWindow", "Open", 0));
        pushButton_write_command->setText(QApplication::translate("MainWindow", "Write Command", 0));
        pushButton_read_status->setText(QApplication::translate("MainWindow", "Read Status", 0));
        groupBox_relaySelect->setTitle(QApplication::translate("MainWindow", " Relay ", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
