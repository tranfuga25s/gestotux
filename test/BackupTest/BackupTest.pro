#-------------------------------------------------
#
#-------------------------------------------------
QT       += sql testlib
QT       -= gui
TARGET    = tst_backuptest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../.

SOURCES += tst_backuptest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
