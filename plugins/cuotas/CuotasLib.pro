TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libcuotas

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

QT += sql xml

HEADERS += mplancuota.h \
           mitemplancuota.h
SOURCES += mplancuota.cpp \
           mitemplancuota.cpp

INCLUDEPATH += ../../utiles \
               ../../src \
               ../pagos


unix {
    LIBS += -gcov
}
