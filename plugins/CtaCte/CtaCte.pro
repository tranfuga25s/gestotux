TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

TARGET = ctacte

DESTDIR = ../../bin/plugins/

HEADERS += cuentacorrienteplugin.h \
           formprefctacte.h \
           mcuentacorriente.h \
           vcuentacorriente.h \
           formnuevactacte.h \
           mitemcuentacorriente.h \
           formresumenctacte.h \
           ../../src/mclientes.h

SOURCES += cuentacorrienteplugin.cpp \
           formprefctacte.cpp \
           mcuentacorriente.cpp \
           vcuentacorriente.cpp \
           formnuevactacte.cpp \
           mitemcuentacorriente.cpp \
           formresumenctacte.cpp \
           ../../src/mclientes.cpp

RESOURCES += CtaCte.qrc

FORMS += FormPrefCtaCteBase.ui \
         FormCtaCteBase.ui \
         FormResumenCtaCteBase.ui \
         ../pagos/FormReciboBase.ui

DISTFILES += ctacte.QMYSQL.sql \
             ctacte.QSQLITE.sql

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../pagos \
               ../caja \
               ../ventas \
               ../productos \
               ../descuentos \
               ../pagos \
               ../../src \
               ../../reporte \
               ../../utiles

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

TRANSLATIONS += CtaCte.ts

OTHER_FILES +=  CtaCte.ts \
                ctacte.QMYSQL.sql \
                ctacte.QSQLITE.sql
win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}
QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -lcaja

DISTFILES += ../../bin/reportes/ListadoCtaCteSaldo.xml \
             ../../bin/reportes/ResumenCtaCte.xml \
             ../../bin/reportes/ResumenCtaCte-MySQL.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}