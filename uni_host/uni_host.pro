#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T07:15:32
#
#-------------------------------------------------


#QT += serialport must be in 1st or 2nd line
QT += core gui
QT += serialport
#QT += network

#QT       += core websockets # comment out on desktop, uncomment for laptop
#QT       -= gui
#QT += network widgets



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = uni_host
TEMPLATE = app


release: DESTDIR = ./release
debug:   DESTDIR = ./debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc

RC_FILE += icon.rc




SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    host.cpp \
    host_comms.cpp \
    usb_vcp.cpp \
    host_graph.cpp \
    pdp_command_c.cpp \
    fc_comport_c.cpp \
    host_pdp.cpp \
    host_relay.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    host.h \
    usb_vcp.h \
    fc_comport_c.h \
    fc_host_commands.h \
    pdp_command_c.h \
    uni_host_structs.h

FORMS    += mainwindow.ui

