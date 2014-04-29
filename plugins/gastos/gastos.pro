TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

SOURCES +=  dgastos.cpp \
            mgasto.cpp \
            vgastos.cpp \
            gastos.cpp \
            formagregargasto.cpp \
            vcategoriasgastos.cpp \
            mcategoriasgastos.cpp

HEADERS +=  dgastos.h \
            mgasto.h \
            vgastos.h \
            gastos.h \
            formagregargasto.h \
            vcategoriasgastos.h \
            mcategoriasgastos.h

QT += sql \
      xml

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

FORMS += FormAgregarGastoBase.ui

RESOURCES += gastos.qrc

TRANSLATIONS += gastos.ts

TARGET = gastos

OTHER_FILES += gastos.QSQLITE.sql \
               gastos.QMYSQL.sql
win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -lcaja \

DISTFILES += ../../bin/reportes/ListadoGastos.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}