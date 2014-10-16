#-------------------------------------------------
# Project created by QtCreator 2014-10-15T19:40:19
#-------------------------------------------------
QT       += sql svg xml testlib

TARGET = tst_comprobantetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_comprobantetest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../comprobantes/release/ -lcomprobantes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../comprobantes/debug/ -lcomprobantes
else:symbian: LIBS += -lcomprobantes
else:unix: LIBS += -L$$OUT_PWD/../../comprobantes/ -lcomprobantes

INCLUDEPATH += $$PWD/../../comprobantes
DEPENDPATH += $$PWD/../../comprobantes