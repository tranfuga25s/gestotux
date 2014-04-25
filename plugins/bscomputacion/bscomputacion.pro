HEADERS += bscomputacion.h

SOURCES += bscomputacion.cpp

TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      webkit \
      xml

TARGET = bscomputacion

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles \
               ../caja \
               ../CtaCte

LIBS += ../../bin/libreporte.a \
        ../../bin/libutiles.a

RESOURCES = bscomputacion.qrc

