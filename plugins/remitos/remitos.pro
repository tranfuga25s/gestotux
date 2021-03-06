TEMPLATE = lib

CONFIG += dll \
          plugin \
          help \
          rtti \
          exceptions

QT += sql \
      xml

TARGET = remitos

SOURCES =  remitosplugins.cpp \
           formprefremito.cpp \
           vremito.cpp \
           vitemremito.cpp \
           MVRemito.cpp \
           formagregarremito.cpp \
           MRemito.cpp \
           mitemremito.cpp \
           formremito.cpp \
           ../CtaCte/mcuentacorriente.cpp \
           ../CtaCte/mitemcuentacorriente.cpp

HEADERS  = remitosplugins.h \
           formprefremito.h \
           vremito.h \
           vitemremito.h \
           MVRemito.h \
           formagregarremito.h \
           MRemito.h \
           mitemremito.h \
           formremito.h \
           ../CtaCte/mcuentacorriente.h \
           ../CtaCte/mitemcuentacorriente.h

DESTDIR = ../../bin/plugins/

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

FORMS += FormAgregarRemitoBase.ui \
         FormPrefRemitoBase.ui

INCLUDEPATH +=  ../../src \
                ../../utiles \
                ../../reporte \
                ../productos \
                ../CtaCte \
                ../caja \
                ../descuentos

RESOURCES += remitos.qrc

OTHER_FILES += remitos.QSQLITE.sql \
               remitos.QMYSQL.sql

QMAKE_LFLAGS += -Wl,-rpath,./plugins

LIBS += -L../../bin \
        -L../../bin/plugins \
        -lutiles \
        -lreporte \
        -llibctacte \
        -lcaja \
        -lcuotas \
        -llibdescuentos

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

DISTFILES += ../../bin/reportes/Remito.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}
