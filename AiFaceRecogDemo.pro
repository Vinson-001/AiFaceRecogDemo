#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T10:12:24
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AiFaceRecogDemo
TEMPLATE = app

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj

SOURCES += main.cpp\
        logindlg.cpp \
    menu/menuwindow.cpp \
    systemset/syssetwindow.cpp \
    faceRecog/facerecogwindow.cpp \
    uilib/iconhelper.cpp \
    toolsclass/http/chttpclient.cpp \
    toolsclass/udpthread/devdiscoverythread.cpp \
    toolsclass/xml/domxmlanalysisbase.cpp \
    toolsclass/xml/domxmlanalysisforudp.cpp \
    toolsclass/tabelview/mytableview.cpp \
    toolsclass/tabelview/mytableviewdelegate.cpp \
    deviceManger/devinfosys.cpp \
    deviceManger/deviceMangerwindow.cpp \
    toolsclass/cmd/httpcmd.cpp

HEADERS  += logindlg.h \
    menu/menuwindow.h \
    systemset/syssetwindow.h \
    faceRecog/facerecogwindow.h \
    uilib/iconhelper.h \
    toolsclass/http/chttpclient.h \
    toolsclass/udpthread/devdiscoverythread.h \
    toolsclass/xml/domxmlanalysisbase.h \
    toolsclass/xml/domxmlanalysisforudp.h \
    toolsclass/tabelview/mytableview.h \
    toolsclass/tabelview/mytableviewdelegate.h \
    deviceManger/devinfosys.h \
    deviceManger/deviceMangerwindow.h \
    toolsclass/cmd/httpcmd.h \
    toolsclass/cmd/cmd.h

FORMS    += logindlg.ui \
    menu/menuwindow.ui \
    systemset/syssetwindow.ui \
    deviceManger/deviceMangerwindow.ui \
    faceRecog/facerecogwindow.ui \
    deviceManger/devinfosys.ui

RC_ICONS = ./qrc/image/main.ico

RESOURCES += \
    qrc.qrc
