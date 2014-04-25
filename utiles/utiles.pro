TEMPLATE = lib

HEADERS += eactcerrar.h \
           eactimprimir.h \
           eventana.h \
           evlista.h \
           preferencias.h \
           evisorinformes.h \
           formprefhijo.h \
           eayuda.h \
           eactguardar.h \
           emcliente.h \
           eeditor.h \
           eregistroplugins.h \
           eactpdf.h \
           mproductostotales.h \
           dproductostotales.h \
           edsbprecio.h \
           dsino.h \
           EReporte.h \
           ../plugins/productos/mproductos.h \
           NumeroComprobante.h \
           ELECuitCuil.h \
           ecbproductos.h \
           ecbclientes.h \
           ebusqueda.h \
           etextbrowser.h \
           ecbpaises.h \
           ecbprovincias.h \
           ecbproveedor.h \
           ecbtabla.h \
           eddescuento.h \
    util.h
SOURCES += eactcerrar.cpp \
           eactimprimir.cpp \
           eventana.cpp \
           evlista.cpp \
           preferencias.cpp \
           evisorinformes.cpp \
           eayuda.cpp \
           eactguardar.cpp \
           emcliente.cpp \
           eeditor.cpp \
           eregistroplugins.cpp \
           eactpdf.cpp \
           mproductostotales.cpp \
           dproductostotales.cpp \
           edsbprecio.cpp \
           dsino.cpp \
           ../plugins/productos/mproductos.cpp \
           NumeroComprobante.cpp \
           EReporte.cpp \
           ELECuitCuil.cpp \
           ecbproductos.cpp \
           ecbclientes.cpp \
           ebusqueda.cpp \
           etextbrowser.cpp \
           ecbpaises.cpp \
           ecbprovincias.cpp \
           ecbproveedor.cpp \
           ecbtabla.cpp \
           eddescuento.cpp \
    util.cpp

QT += sql \
      xml

CONFIG +=   staticlib \
            help \
            exceptions

RESOURCES += utiles.qrc

DESTDIR = ../bin

FORMS += EAyudaBase.ui \
    EBusquedaBase.ui \
    eddescuentobase.ui

TRANSLATIONS += utiles.ts

INCLUDEPATH += ../src \
               ../reporte
