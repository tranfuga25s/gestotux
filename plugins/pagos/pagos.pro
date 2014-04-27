TEMPLATE = lib

CONFIG += dll \
plugin \
help \
rtti \
exceptions

QT += sql \
      xml

TARGET = pagos

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

HEADERS += pagosplugin.h \
           mpagos.h \
           formagregarrecibo.h \
           vpagos.h \
           recibo.h \
           FormPrefRecibos.h \
           mvpagos.h \
           formrecibo.h \
           ../../src/mclientes.h \
           ../CtaCte/mcuentacorriente.h \
           ../CtaCte/mitemcuentacorriente.h

SOURCES += pagosplugin.cpp \
           mpagos.cpp \
           formagregarrecibo.cpp \
           vpagos.cpp \
           recibo.cpp \
           FormPrefRecibos.cpp \
           mvpagos.cpp \
           formrecibo.cpp \
           ../../src/mclientes.cpp \
           ../CtaCte/mcuentacorriente.cpp \
           ../CtaCte/mitemcuentacorriente.cpp

FORMS += FormReciboBase.ui \
         FormPrefRecibos.ui

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles \
               ../caja \
               ../CtaCte

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

TRANSLATIONS += pagos.ts

RESOURCES = pagos.qrc

OTHER_FILES += pagos.QMYSQL.sql \
               pagos.QSQLITE.sql
win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}
QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -llibctacte \
        -lcaja \
        -llibventas

DISTFILES += ../../bin/reportes/Recibo.xml

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}