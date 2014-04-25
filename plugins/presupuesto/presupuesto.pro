TEMPLATE = lib
CONFIG += plugin \
          dll \
          help

QT += sql \
      xml

HEADERS += presupuesto.h \
           formagregarpresupuesto.h \
           mpresupuesto.h \
           vpresupuesto.h \
           MItemPresupuesto.h \
           ../../src/mclientes.cpp \
           mvpresupuestos.h \
           vlistapresupuesto.h \
           formmodificarpresupuesto.h

SOURCES += presupuesto.cpp \
           formagregarpresupuesto.cpp \
           mpresupuesto.cpp \
           vpresupuesto.cpp \
           MItemPresupuesto.cpp \
           ../../src/mclientes.cpp \
           mvpresupuestos.cpp \
           vlistapresupuesto.cpp \
           formmodificarpresupuesto.cpp

FORMS += FormPresupuestoBase.ui \
         ../ventas/FormAgregarVentaBase.ui

TARGET = presupuesto

DESTDIR = ../../bin/plugins

RESOURCES += presupuesto.qrc

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte \
               ../descuentos \
               ../ventas \
               ../productos \
               ../CtaCte \
               ../caja

OTHER_FILES += presupuesto.QSQLITE.sql \
               presupuesto.QMYSQL.sql

TRANSLATIONS += presupuesto.ts
win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -llibctacte \
        -lcaja \
        -ldescuentos

DISTFILES += ../../bin/reportes/Presupuesto.xml
