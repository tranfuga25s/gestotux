TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = resumencompras

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
    resumencomprastotales.h \
    resumencomprasproveedor.h \
    TiposPeriodos.h \
    formresumencomprasproveedor.h

SOURCES = reportecompras.cpp \
    formresumencomprastotales.cpp \
    resumencomprastotales.cpp \
    resumencomprasproveedor.cpp \
    formresumencomprasproveedor.cpp

FORMS += formresumencomprastotales.ui \
    formresumencomprasproveedor.ui

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}

RESOURCES += \
    resumencompras.qrc