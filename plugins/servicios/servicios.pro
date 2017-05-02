TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

TARGET = servicios

DESTDIR = ../../bin/plugins

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
           vservicios.h \
           formasociarserviciocliente.h \
           formservicio.h \
           edrecargos.h \
           formrecargos.h \
           FormClientesAdheridos.h \
           FormFacturarServicio.h \
           MTempClientesFacturarServicio.h \
           mcobroservicioclienteperiodo.h \
           FormVerificarRecargos.h \
           eserviciosdelegate.h \
           vlistaperiodos.h \
           formfacturacionemitida.h \
           mclientesservicios.h \
           ../../src/mclientes.h

SOURCES =  servicios.cpp \
           vservicios.cpp \
           formasociarserviciocliente.cpp \
           formservicio.cpp \
           edrecargos.cpp \
           formrecargos.cpp \
           FormClientesAdheridos.cpp \
           FormFacturarServicio.cpp \
           MTempClientesFacturarServicio.cpp \
           mcobroservicioclienteperiodo.cpp \
           FormVerificarRecargos.cpp \
           eserviciosdelegate.cpp \
           vlistaperiodos.cpp \
           formfacturacionemitida.cpp \
           mclientesservicios.cpp \
           ../../src/mclientes.cpp

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
        -llibservicios \
        -lutiles \
        -lreporte \
        -llibctacte \
        -lpagos \
        -llibventas \
        -llibdescuentos \
        -llibcaja \
        -llibproductos \
        -llibservicios \
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
