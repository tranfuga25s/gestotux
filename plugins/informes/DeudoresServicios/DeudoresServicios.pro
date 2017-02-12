TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = cuotas

DESTDIR = ../../../bin/plugins/informes

INCLUDEPATH += ../../../reporte \
               ../../../utiles \
               ../../../src \
               ../. \
               ../../../plugins/servicios

LIBS += -L../../../bin/ \
        -L../../../bin/plugins \
        -llibservicios \
        -lutiles \
        -lreporte


PRE_TARGETDEPS += ../../../bin/libreporte.a \
                  ../../../bin/libutiles.a

HEADERS += DeudoresServicios.h \
    dialogoresumenservicioshistorico.h \
    dialogoresumenporservicio.h \
    dialogodeudahistoricaservicioporcliente.h

SOURCES += DeudoresServicios.cpp \
    dialogoresumenservicioshistorico.cpp \
    dialogoresumenporservicio.cpp \
    dialogodeudahistoricaservicioporcliente.cpp

exists( ../../../travis.pri ) {
    include( ../../../travis.pri )
}

FORMS += dialogoresumenservicioshistorico.ui \
         dialogoresumenporservicio.ui \
         dialogodeudahistoricaservicioporcliente.ui

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}
QMAKE_LFLAGS += -Wl,-rpath,./plugins
