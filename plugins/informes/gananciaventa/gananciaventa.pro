TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti

QT += sql \
      xml

TARGET = gananciaventa

DESTDIR = ../../../bin/plugins/informes

INCLUDEPATH += ../../../reporte \
               ../../../utiles \
               ../../../src \
               ../. \

LIBS += ../../../bin/libutiles.a \
        ../../../bin/libreporte.a

LIBS += -lcompras \
        -L../../../bin \
        -L../../../bin/plugins

QMAKE_LFLAGS += -Wl,-rpath,./../plugins

PRE_TARGETDEPS += ../../../bin/libreporte.a \
                  ../../../bin/libutiles.a

HEADERS += gananciaventa.h \
           formrangofechas.h

SOURCES += gananciaventa.cpp \
           formrangofechas.cpp

FORMS += FormRangoFechasBase.ui
