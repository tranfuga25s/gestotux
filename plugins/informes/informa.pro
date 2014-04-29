TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

TARGET = informes

DESTDIR = ../../bin/plugins/

LIBS += ../../bin/libutiles.a

PRE_TARGETDEPS += ../../bin/libutiles.a

SOURCES += informesplugin.cpp

HEADERS += informesplugin.h \
    einformeinterface.h

INCLUDEPATH += ../../src \
               ../../utiles

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}