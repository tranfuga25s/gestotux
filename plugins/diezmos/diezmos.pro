TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          excpetions

QT += sql \
      xml

TARGET = diezmo

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a


RESOURCES = diezmos.qrc

OTHER_FILES += diezmos.QMYSQL.sql \
               diezmos.QSQLITE.sql

HEADERS += mdiezmos.h \
           vdiezmos.h \
    diezmosplugin.h
SOURCES += mdiezmos.cpp \
           vdiezmos.cpp \
    diezmosplugin.cpp

FORMS += ddardiezmo.ui

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}