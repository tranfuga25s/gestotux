#-------------------------------------------------
# Test para ver que funcione correctamente el Backup Local y Remoto
#-------------------------------------------------
QT       += sql testlib
TARGET    = tst_backuptest
CONFIG   += console help
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../. \
               ../../src \
               ../../utiles

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += ../../bin/libutiles.a

FORMS += ../../src/formBackup.ui

SOURCES += tst_backuptest.cpp \
           ../../src/ebackup.cpp

HEADERS += ../../src/ebackup.h

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}