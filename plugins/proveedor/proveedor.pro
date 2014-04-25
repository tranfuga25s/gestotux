TEMPLATE = lib

CONFIG += dll \
plugin \
help

QT += sql \
      xml

TARGET = proveedor

DESTDIR = ../../bin/plugins/

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a \
                  ../../bin/libreporte.a

TRANSLATIONS += proveedor.ts

SOURCES += dproveedor.cpp \
           mproveedor.cpp \
           vproveedor.cpp \
           proveedor.cpp \
    formagregarproveedor.cpp \
    formmodificarproveedor.cpp
HEADERS += dproveedor.h \
           mproveedor.h \
           vproveedor.h \
           proveedor.h \
    formagregarproveedor.h \
    formmodificarproveedor.h

RESOURCES += proveedor.qrc

OTHER_FILES += proveedor.ts \
               proveedor.QMYSQL.sql \
               proveedor.QSQLITE.sql

DISTFILES += ../../bin/reportes/ListadoProveedores.xml

FORMS += FormProveedorBase.ui

win32 {
    QMAKE_LFLAGS += "-Wl,-export-all-symbols"
}

DISTFILES += ../../bin/reportes/ListadoProveedores.xml
