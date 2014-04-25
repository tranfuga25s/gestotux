TEMPLATE = lib

CONFIG += dll \
plugin \
debug \
 help

QT += sql \
xml

win32 {
    CONFIG += release
    CONFIG -= debug
}
linux-g++ {
    UI_DIR = ui
    MOC_DIR = moc
    OBJECTS_DIR = obj
}

TARGET = movimientos

DESTDIR = ../../../../bin/plugins/informes

HEADERS += informesmovimientos.h \
 formfiltro.h \
 erenderizadorinforme.h \
 ../../mcategoria.h \
 ../../mestablecimiento.h \
 ../../einformeimpresora.h

SOURCES += informesmovimientos.cpp \
 formfiltro.cpp \
 erenderizadorinforme.cpp \
 ../../mcategoria.cpp \
 ../../mestablecimiento.cpp \
 ../../einformeimpresora.cpp


FORMS += FormFiltrosBase.ui

QMAKE_CXXFLAGS_DEBUG += -ggdb \
-g3 \
-O0

DISTFILES += estiloinforme.css

RESOURCES += informe.qrc

CONFIG -= release




LIBS += ../../../../bin/libutiles.a

TARGETDEPS += ../../../../bin/libutiles.a


INCLUDEPATH += ../.. \
  ../../../../utiles

