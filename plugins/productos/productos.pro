TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = productos

DESTDIR = ../../bin/plugins

SOURCES += vcategorias.cpp \
           vproductos.cpp \
           productos.cpp \
           dproductos.cpp \
           formprefproductos.cpp \
           formagregarproducto.cpp \
           formmodificarproducto.cpp \
           dremarcadormasivo.cpp \
           mtempproductoremarcar.cpp
HEADERS += vcategorias.h \
           vproductos.h \
           productos.h \
           dproductos.h \
           formprefproductos.h \
           formagregarproducto.h \
           formmodificarproducto.h \
           ecbcategoriasproductos.h \
           dremarcadormasivo.h \
           mtempproductoremarcar.h

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte \

DISTFILES += productos.SQLITE.sql \
             productos.QMYSQL.sql

TRANSLATIONS += productos.ts
RESOURCES += productos.qrc

FORMS += FormPrefProductosBase.ui \
         formproductobase.ui \
         dremarcadormasivobase.ui

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -llibproductos

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

DISTFILES += ../../bin/reportes/ListadoProductosPrecio.xml \
             ../../bin/reportes/ListadoProductosStock.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}