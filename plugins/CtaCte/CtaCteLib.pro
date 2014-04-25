TEMPLATE = lib

CONFIG += staticlib \
          help \
          exceptions

QT += sql \
      xml

TARGET = libctacte

DESTDIR = ../../bin/plugins/

HEADERS += mcuentacorriente.h \
           mitemcuentacorriente.cpp

SOURCES += mcuentacorriente.cpp \
           mitemcuentacorriente.cpp

INCLUDEPATH += ../../utiles


unix {
    LIBS += -gcov
}
