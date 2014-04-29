HEADERS += jhau-pora.h

SOURCES += jhau-pora.cpp

TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

TARGET = ajhau-pora

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
        -lreporte

RESOURCES = \
    jhaupora.qrc

OTHER_FILES += \
    jhau-pora.rc

