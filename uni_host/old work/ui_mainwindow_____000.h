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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QWidget *tab_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_2;
    QPlainTextEdit *plainTextEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1125, 767);
        QIcon icon;
        icon.addFile(QStringLiteral("Smiley.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
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
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(6, -1, 811, 731));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayoutWidget = new QWidget(tab);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(190, 190, 301, 181));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(gridLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayoutWidget_2 = new QWidget(tab_2);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(250, 220, 160, 80));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(gridLayoutWidget_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(823, 20, 291, 711));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Unilectric PDP", 0));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
