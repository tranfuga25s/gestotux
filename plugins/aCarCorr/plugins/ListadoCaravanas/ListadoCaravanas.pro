TEMPLATE = lib

CONFIG += dll \
plugin \
debug \
exceptions \
help
CONFIG -= release

QT += sql

QMAKE_CXXFLAGS_DEBUG += -ggdb \
  -g3

LIBS += ../../../../bin/libutiles.a

TARGETDEPS += ../../../../bin/libutiles.a

TARGET = listadocaravanas

DESTDIR = /home/Esteban/Programas/gestotux/bin/plugins/informes

HEADERS += listadocaravanas.h \
 ../../mestablecimiento.h \
 irenderizador.h \
 ../../einformeimpresora.h

SOURCES += listadocaravanas.cpp \
 ../../mestablecimiento.cpp \
 irenderizador.cpp \
 ../../einformeimpresora.cpp

INCLUDEPATH += ../.. \
  ../../../../src \
  ../../../../utiles

