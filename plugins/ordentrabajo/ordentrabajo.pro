TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      xml

TARGET = ordentrabajo

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte \
               ../garantias

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

SOURCES += OrdenTrabajoPlugin.cpp \
    formordentrabajo.cpp \
    vtecnicos.cpp \
    mtecnicos.cpp \
    vtipooperacionordentrabajo.cpp \
    mtipooperacionordentrabajo.cpp \
    mequipamiento.cpp \
    ecbtecnicos.cpp \
    vequipamiento.cpp \
    ecbmodeloequipamiento.cpp \
    formequipamiento.cpp \
    mordentrabajo.cpp \
    vordentrabajo.cpp \
    mhistorialordentrabajo.cpp \
    dhistorialordentrabajo.cpp \
    ecbtipooperacionordentrabajo.cpp \
    ordentrabajowizard.cpp \
    wizard/paginacliente.cpp \
    wizard/paginaclientenuevo.cpp \
    wizard/paginaequipamiento.cpp \
    wizard/paginaequipamientonuevo.cpp \
    wizard/paginafinal.cpp \
    wizard/paginaequipamientoexistente.cpp \
    ../../src/mestadofiscal.cpp \
    formprefordentrabajo.cpp \
    ecbequipamiento.cpp \
    ../../src/mclientes.cpp \
    mvequipamiento.cpp \
    ../garantias/mgarantias.cpp


HEADERS += OrdenTrabajoPlugin.h \
    formordentrabajo.h \
    vtecnicos.h \
    mtecnicos.h \
    vtipooperacionordentrabajo.h \
    mtipooperacionordentrabajo.h \
    mequipamiento.h \
    ecbtecnicos.h \
    vequipamiento.h \
    ecbmodeloequipamiento.h \
    formequipamiento.h \
    mordentrabajo.h \
    vordentrabajo.h \
    mhistorialordentrabajo.h \
    dhistorialordentrabajo.h \
    ecbtipooperacionordentrabajo.h \
    ordentrabajowizard.h \
    wizard/paginacliente.h \
    wizard/paginaclientenuevo.h \
    wizard/paginaequipamiento.h \
    wizard/paginaequipamientonuevo.h \
    wizard/paginafinal.h \
    wizard/paginaequipamientoexistente.h \
    ../../src/mestadofiscal.h \
    formprefordentrabajo.h \
    ecbequipamiento.h \
    ../../src/mclientes.h \
    mvequipamiento.h  \
    ../garantias/mgarantias.h

RESOURCES += ordentrabajo.qrc

OTHER_FILES += ordentrabajo.QMYSQL.sql \
               ordentrabajo.QSQLITE.sql

FORMS += formordentrabajobase.ui \
    formequipamientobase.ui \
    dhistorialordentrabajo.ui \
    wizard/paginaclientenuevo.ui \
    wizard/paginaequipamiento.ui \
    wizard/paginaequipamientonuevo.ui \
    wizard/paginafinal.ui \
    wizard/paginaequipamientoexistente.ui \
    FormPrefOrdenTrabajo.ui

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}
