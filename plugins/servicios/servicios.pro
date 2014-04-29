TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

TARGET = servicios

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
              ../../reporte \
              ../../utiles \
              ../CtaCte \
              ../ventas \
              ../productos \
              ../caja \
              ../descuentos

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libreporte.a \
                   ../../bin/libutiles.a

HEADERS =  servicios.h \
           mservicios.h \
           vservicios.h \
           mrecargos.h \
           mrecargoshechos.h \
           formasociarserviciocliente.h \
           formservicio.h \
           edrecargos.h \
           formrecargos.h \
           FormClientesAdheridos.h \
           MClientesServicios.h \
           FormFacturarServicio.h \
           MTempClientesFacturarServicio.h \
           mperiodoservicio.h \
           mcobroservicioclienteperiodo.h \
           FormVerificarRecargos.h \
           eserviciosdelegate.h \
           vlistaperiodos.h \
           formfacturacionemitida.h \
           mfacturacionemitida.h \
           ../../src/mclientes.h \
           ../productos/mproductos.h


SOURCES =  servicios.cpp \
           mservicios.cpp \
           vservicios.cpp \
           mrecargos.cpp \
           mrecargoshechos.cpp \
           formasociarserviciocliente.cpp \
           formservicio.cpp \
           edrecargos.cpp \
           formrecargos.cpp \
           FormClientesAdheridos.cpp \
           MClientesServicios.cpp \
           FormFacturarServicio.cpp \
           MTempClientesFacturarServicio.cpp \
           mperiodoservicio.cpp \
           mcobroservicioclienteperiodo.cpp \
           FormVerificarRecargos.cpp \
           eserviciosdelegate.cpp \
           vlistaperiodos.cpp \
           formfacturacionemitida.cpp \
           mfacturacionemitida.cpp \
           ../../src/mclientes.cpp \
           ../productos/mproductos.cpp

RESOURCES += servicios.qrc

TRANSLATIONS = servicios.ts

FORMS += FormAsociarBase.ui \
         formServicioBase.ui \
         FormRecargoBase.ui \
         FormClientesAdheridosBase.ui \
         FormFacturarServicioBase.ui \
         FormVerificarRecargos.ui \
         formfacturacionemitida.ui

OTHER_FILES += servicios.QMYSQL.sql \
               servicios.QSQLITE.sql

DEFINES += GESTOTUX_HICOMP

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -llibctacte \
        -lpagos \
        -llibventas \
        -ldescuentos \
        -lcaja \
        -lutiles

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

DISTFILES += ../../bin/reportes/baja-servicio.xml \
             ../../bin/reportes/ListaDeudoresServicio.xml \
             ../../bin/reportes/ListaDeudoresServicioHC.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}