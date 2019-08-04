#-------------------------------------------------
#
# Project created by QtCreator 2019-07-22T18:40:42
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
        clogindlg.cpp \
    uilib/iconhelper.cpp \
    menuwindow.cpp \
    bkgroupwindow.cpp \
    syssetwindow.cpp \
    facerecogwindow.cpp \
    toolsclass/chttpclient.cpp \
    toolsclass/canalysisxml.cpp \
    deviceMangerwindow.cpp \
    toolsclass/devsendthread.cpp \
    toolsclass/cdomxmlanalysis.cpp

HEADERS  += clogindlg.h \
    uilib/iconhelper.h \
    menuwindow.h \
    bkgroupwindow.h \
    syssetwindow.h \
    facerecogwindow.h \
    toolsclass/readonlydelegatefortableview.h \
    toolsclass/vipmodel.h \
    toolsclass/canalysisxml.h \
    toolsclass/chttpclient.h \
    deviceMangerwindow.h \
    toolsclass/devsendthread.h \
    toolsclass/cdomxmlanalysis.h

FORMS    += clogindlg.ui \
    menuwindow.ui \
    bkgroupwindow.ui \
    syssetwindow.ui \
    facerecogwindow.ui \
    deviceMangerwindow.ui

RC_ICONS = ./qrc/image/main.ico

RESOURCES += \
    aifacerecog.qrc

DISTFILES +=
