TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

TARGET = libdescuentos

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

QT += sql

HEADERS += mdescuentos.h \
           mdescuentostipo.h
SOURCES += mdescuentos.cpp \
           mdescuentostipo.cpp


INCLUDEPATH += ../../utiles \
               ../../src


unix {
    LIBS += -gcov
}
