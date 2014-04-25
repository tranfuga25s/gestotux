TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          excpetions

QT += sql \
      xml

TARGET = cuotas

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles \
               ../caja \
               ../CtaCte \
               ../pagos \
               ../productos \
               ../descuentos

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a


RESOURCES = cuotas.qrc

OTHER_FILES += cuotas.QMYSQL.sql \
               cuotas.QSQLITE.sql \
               ./../bin/reportes/ResumenCuotas.xml

HEADERS += cuotasplugin.h \
           mplancuota.h \
           formsimularcuotas.h \
           msimularcuotas.h \
           formprefcuotas.h \
           vplancuotas.h \
           mitemplancuota.h \
           mvplancuota.h \
           formdetallecuotas.h \
           DPagoCuota.h \
           formgenerarcomprobantescuotas.h \
           mgenerarcuotas.h \
           ../../src/mclientes.h \
           formadelantocuotas.h \
           madelantosimularcuotas.h \
           ../productos/mproductos.cpp

SOURCES += cuotasplugin.cpp \
           mplancuota.cpp \
           formsimularcuotas.cpp \
           msimularcuotas.cpp \
           formprefcuotas.cpp \
           vplancuotas.cpp \
           mitemplancuota.cpp \
           mvplancuota.cpp \
           formdetallecuotas.cpp \
           DPagoCuota.cpp \
           formgenerarcomprobantescuotas.cpp \
           mgenerarcuotas.cpp \
           ../../src/mclientes.cpp \
           formadelantocuotas.cpp \
           madelantosimularcuotas.cpp \
           ../productos/mproductos.cpp

FORMS += formsimularcuotasbase.ui \
         formprefcuotasbase.ui \
         formcuotasbase.ui \
         DPagoCuotaBase.ui \
         formgenerarcomprobantescuotasbase.ui

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
        -llibremitos \
        -lpagos \
        -llibventas \
        -lcaja \
        -ldescuentos \
        -lutiles
