TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

HEADERS += unidadesproductosplugin.h

SOURCES += unidadesproductosplugin.cpp

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