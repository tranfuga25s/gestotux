TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = compras

DESTDIR = ../../../bin/plugins/informes

INCLUDEPATH += ../../../reporte \
               ../../../utiles \
               ../../../src \
               ../../../plugins/compras \
               ../.

LIBS += ../../../bin/libutiles.a \
        ../../../bin/libreporte.a

PRE_TARGETDEPS += ../../../bin/libreporte.a \
                  ../../../bin/libutiles.a

HEADERS = reportecompras.h \
    formresumencomprastotales.h \
    resumencomprastotales.h

SOURCES = reportecompras.cpp \
    formresumencomprastotales.cpp \
    resumencomprastotales.cpp

FORMS += formresumencomprastotales.ui

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}