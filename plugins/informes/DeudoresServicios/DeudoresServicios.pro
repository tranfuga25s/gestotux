TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = deudoresservicios

DESTDIR = ../../../bin/plugins/informes

INCLUDEPATH += ../../../reporte \
               ../../../utiles \
               ../../../src \
               ../. \
               ../../../plugins/servicios

QMAKE_LFLAGS += -Wl,-rpath,./../../bin/plugins

LIBS += -L../../../bin/ \
        -L../../../bin/plugins \
        -lutiles \
        -lreporte


PRE_TARGETDEPS += ../../../bin/libreporte.a \
                  ../../../bin/libutiles.a

HEADERS = DeudoresServicios.h \
          dialogoresumenservicioshistorico.h \
          dialogoresumenporservicio.h \
          dialogodeudahistoricaservicioporcliente.h \
          ../../../src/mclientes.h \
          ../../../utiles/eregistroplugins.h \
          utilreportedeudoresservicios.h

SOURCES = DeudoresServicios.cpp \
          dialogoresumenservicioshistorico.cpp \
          dialogoresumenporservicio.cpp \
          dialogodeudahistoricaservicioporcliente.cpp \
          ../../../src/mclientes.cpp \
          ../../../utiles/eregistroplugins.cpp \
          utilreportedeudoresservicios.cpp

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}

FORMS += dialogoresumenservicioshistorico.ui \
         dialogoresumenporservicio.ui \
         dialogodeudahistoricaservicioporcliente.ui

win32 {
    #QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}
