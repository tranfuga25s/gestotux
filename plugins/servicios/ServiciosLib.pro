TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libservicios

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

QT += sql

HEADERS =  mservicios.h \
           mrecargos.h \
           mrecargoshechos.h \
           mperiodoservicio.h \
           mcobroservicioclienteperiodo.h

SOURCES =  mservicios.cpp \
           mrecargos.cpp \
           mrecargoshechos.cpp \
           MClientesServicios.cpp \
           MTempClientesFacturarServicio.cpp \
           mperiodoservicio.cpp \
           mcobroservicioclienteperiodo.cpp \
           mfacturacionemitida.cpp \

DEFINES += GESTOTUX_HICOMP

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}