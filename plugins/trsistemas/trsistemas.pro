HEADERS += trsistemas.h

SOURCES += trsistemas.cpp

TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

TARGET = trsistemas

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles \
               ../caja \
               ../CtaCte \
               ../pagos

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -lpagos

RESOURCES = trsistemas.qrc

