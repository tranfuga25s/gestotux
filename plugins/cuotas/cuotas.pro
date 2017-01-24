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
           formsimularcuotas.h \
           msimularcuotas.h \
           formprefcuotas.h \
           vplancuotas.h \
           mvplancuota.h \
           formdetallecuotas.h \
           DPagoCuota.h \
           formgenerarcomprobantescuotas.h \
           mgenerarcuotas.h \
           ../../src/mclientes.h \
           formadelantocuotas.h \
           madelantosimularcuotas.h

SOURCES += cuotasplugin.cpp \
           formsimularcuotas.cpp \
           msimularcuotas.cpp \
           formprefcuotas.cpp \
           vplancuotas.cpp \
           mvplancuota.cpp \
           formdetallecuotas.cpp \
           DPagoCuota.cpp \
           formgenerarcomprobantescuotas.cpp \
           mgenerarcuotas.cpp \
           ../../src/mclientes.cpp \
           formadelantocuotas.cpp \
           madelantosimularcuotas.cpp

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
        -llibcaja \
        -llibremitos \
        -llibpagos \
        -llibventas \
        -llibcaja \
        -llibdescuentos \
        -llibproductos \
        -llibcuotas \
        -lctacte \
        -llibctacte \
        -lutiles \
        -lreporte \

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
