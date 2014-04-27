TEMPLATE = lib
TARGET = l4ustyle2
CONFIG += plugin \
          dll
Qt += gui \
      core
INCLUDEPATH += .
VERSION = 0.1

HEADERS += l4ustyle2.h misc.h
SOURCES += l4ustyle2.cpp misc.cpp

DESTDIR = ../../../bin/styles

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}