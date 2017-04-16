TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

SOURCES +=  ventas.cpp \
            vventas.cpp \
            formagregarventa.cpp \
            formprefventa.cpp \
            MFactura.cpp \
            mitemfactura.cpp \
           ../../src/mclientes.cpp \
           MVFacturas.cpp \
           vitemfactura.cpp \
           ../CtaCte/mitemcuentacorriente.cpp

HEADERS += ventas.h \
           vventas.h \
           formagregarventa.h \
           formprefventa.h \
           MFactura.h \
           mitemfactura.h \
           ../../src/mclientes.h \
           MVFacturas.h \
           vitemfactura.h \
           ../CtaCte/mitemcuentacorriente.h

TRANSLATIONS = ventas.ts

QT += sql \
      xml

TARGET = ventas

DESTDIR = ../../bin/plugins/

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -llibctacte \
        -llibcaja \
        -llibcuotas \
        -llibproductos \
        -llibdescuentos

PRE_TARGETDEPS += ../../bin/libutiles.a \
                   ../../bin/libreporte.a

FORMS += FormAgregarVentaBase.ui \
         FormPrefVentaBase.ui

INCLUDEPATH +=  ../../src \
                ../../utiles \
                ../../reporte \
                ../productos \
                ../CtaCte \
                ../caja \
                ../descuentos

RESOURCES += ventas.qrc

OTHER_FILES += ventas.ts \
               ventas.QSQLITE.sql \
               ventas.QMYSQL.sql
win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

DISTFILES += ../../bin/reportes/AnulacionFactura.xml \
             ../../bin/reportes/Factura.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
