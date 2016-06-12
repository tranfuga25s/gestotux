HEADERS += barbier.h

SOURCES += barbier.cpp

TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = barbier

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a \

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles

LIBS += ../../bin/libreporte.a \
        ../../bin/libutiles.a

RESOURCES = barbier.qrc