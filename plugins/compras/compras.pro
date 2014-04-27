TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql

TARGET = compras

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../productos \
               ../caja

LIBS += ../../bin/libutiles.a

PRE_TARGETDEPS += ../../bin/libutiles.a

TRANSLATIONS += compras.ts

FORMS += FormAgregarCompraBase.ui \
         FormActualizarPrecios.ui \
         ../productos/formproductobase.ui

RESOURCES += compras.qrc

SOURCES += mcompra.cpp \
           vcompras.cpp \
           formagregarcompra.cpp \
           compras.cpp \
           mcompraproducto.cpp \
           FormActualizarPrecios.cpp \
           vlistacompra.cpp

HEADERS += formagregarcompra.h \
           mcompra.h \
           vcompras.h \
           compras.h \
           mcompraproducto.h \
           FormActualizarPrecios.h \
           vlistacompra.h \

OTHER_FILES += compras.QMYSQL.sql \
               compras.QSQLITE.sql

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}
QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -lproductos \
        -llibcaja

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}