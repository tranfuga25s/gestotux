TEMPLATE = lib

CONFIG += dll \
plugin

TARGET = autodestruct

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src


HEADERS += autodestruc.h

SOURCES += autodestruc.cpp

TRANSLATIONS += autodestruc.ts

QT += sql

