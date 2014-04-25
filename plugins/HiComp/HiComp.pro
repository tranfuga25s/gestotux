TEMPLATE = lib

CONFIG += plugin \
          dll \
          help

TARGET = hicomp

QT += xml

HEADERS += hicomp.h \
           DPagarRecibo.h\
           ../pagos/mpagos.h \
           ../CtaCte/mitemcuentacorriente.h \
           ../CtaCte/mcuentacorriente.h \
           ../caja/mcajas.h \
           ../caja/mmovimientoscaja.h \
           ../servicios/mcobroservicioclienteperiodo.h \
           ../servicios/mrecargoshechos.h \
           ../servicios/mrecargos.h \
           ../servicios/mservicios.h \
           ../servicios/mperiodoservicio.h \
           ../ventas/MFactura.h \
           ../ventas/mitemfactura.h \
           ../productos/mproductos.h \
           ../../reporte/common/parameter.h \
           ../descuentos/mdescuentos.h \
           ../cuotas/mitemplancuota.h \
           ../../src/mclientes.h \
    vrecibosimpagos.h \
    mrecibosimpagos.h

SOURCES += hicomp.cpp \
           DPagarRecibo.cpp \
           ../pagos/mpagos.cpp \
           ../CtaCte/mitemcuentacorriente.cpp \
           ../CtaCte/mcuentacorriente.cpp \
           ../caja/mcajas.cpp \
           ../caja/mmovimientoscaja.cpp \
           ../servicios/mcobroservicioclienteperiodo.cpp \
           ../servicios/mrecargoshechos.cpp \
           ../servicios/mrecargos.cpp \
           ../servicios/mservicios.cpp \
           ../servicios/mperiodoservicio.cpp \
           ../ventas/MFactura.cpp \
           ../ventas/mitemfactura.cpp \
           ../productos/mproductos.cpp \
           ../../reporte/common/parameter.cpp \
           ../descuentos/mdescuentos.cpp \
           ../cuotas/mitemplancuota.cpp \
           ../../src/mclientes.cpp \
    vrecibosimpagos.cpp \
    mrecibosimpagos.cpp

DESTDIR = ../../bin/plugins

RESOURCES += hicomp.qrc

INCLUDEPATH += ../../utiles \
               ../pagos \
               ../../src \
               ../CtaCte \
               ../caja \
               ../servicios \
               ../productos \
               ../descuentos \
               ../cuotas \
               ../../reporte

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

TRANSLATIONS += hicomp.ts

OTHER_FILES += hicomp.QSQLITE.sql \
               hicomp.QMYSQL.sql

FORMS = DPagarReciboBase.ui
