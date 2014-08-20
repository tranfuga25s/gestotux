TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

HEADERS += unidadesproductosplugin.h \
    formprefunidadesproductos.h \
    munidadesproductos.h \
    mproductosunidades.h \
    dunidadproducto.h \
    ecbunidadesproductos.h

SOURCES += unidadesproductosplugin.cpp \
    formprefunidadesproductos.cpp \
    munidadesproductos.cpp \
    mproductosunidades.cpp \
    dunidadproducto.cpp \
    ecbunidadesproductos.cpp

TARGET = unidades_productos

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles \
               ../productos

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lproductos

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}

RESOURCES += unidades_productos.qrc

OTHER_FILES += \
    unidades_productos.QSQLITE.sql \
    unidades_productos.QMYSQL.sql

FORMS += \
    formprefunidadesproductos.ui \
    dunidadproducto.ui