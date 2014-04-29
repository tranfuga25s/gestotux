TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = cuotas

DESTDIR = ../../../bin/plugins/informes

INCLUDEPATH += ../../../reporte \
               ../../../utiles \
               ../../../src \
               ../.

LIBS += ../../../bin/libutiles.a \
        ../../../bin/libreporte.a

PRE_TARGETDEPS += ../../../bin/libreporte.a \
                  ../../../bin/libutiles.a

HEADERS += \
    reportescuotas.h

SOURCES += \
    reportescuotas.cpp

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}