TEMPLATE = lib
HEADERS += floatstyle.h \
           helpers.h
SOURCES += floatstyle.cpp \
           drawcontrol.cpp \
           helpers.cpp \
           sizes.cpp \
           drawcomplexcontrol.cpp
TARGET = floatstyle
CONFIG += plugin \
          dll
RESOURCES = images.qrc
DESTDIR = ../../../bin/styles
exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}
