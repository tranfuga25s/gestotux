TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

SOURCES +=  MFactura.cpp \
            mitemfactura.cpp

HEADERS += MFactura.h \
           mitemfactura.h

QT += sql

TARGET = libventas

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../utiles \
               ../productos \
               ../../src \
               ../CtaCte \
               ../caja \
               ../descuentos

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}