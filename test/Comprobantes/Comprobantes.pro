#-------------------------------------------------
# Test agregaro para el nuevo sistema de comprobantes
#-------------------------------------------------
QT       += sql svg xml testlib
QT       -= gui

TARGET = tst_comprobantestest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_comprobantestest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../comprobantes/release/ -lcomprobantes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../comprobantes/debug/ -lcomprobantes
else:symbian: LIBS += -lcomprobantes
else:unix: LIBS += -L$$OUT_PWD/../../comprobantes/ -lcomprobantes

INCLUDEPATH += $$PWD/../../comprobantes
DEPENDPATH += $$PWD/../../comprobantes
