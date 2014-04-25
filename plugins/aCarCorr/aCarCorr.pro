TEMPLATE = lib

CONFIG += dll \
plugin \
exceptions \
 help

QT += sql \
 svg

HEADERS += admincarcorr.h \
 mcategoria.h \
 mestablecimiento.h \
 vcategorias.h \
 vestablecimiento.h \
 formagregar.h \
 formmudanza.h \
 formmovimiento.h \
 formventa.h \
 mtri.h \
 mcaravanas.h \
 emovimiento.h \
 formstock.h \
 einformeinterface.h \
 TipoMovs.h \
 mcaravanadueno.h \
 formmodificartri.h \
 formprefcaravanas.h \
 mprefcategorias.h \
 evlista.h \
 einformeimpresora.h \
 edcategoriaespecial.h

SOURCES += admincarcorr.cpp \
 mcategoria.cpp \
 mestablecimiento.cpp \
 vcategorias.cpp \
 vestablecimiento.cpp \
 formagregar.cpp \
 formmudanza.cpp \
 formmovimiento.cpp \
 formventa.cpp \
 mtri.cpp \
 mcaravanas.cpp \
 emovimiento.cpp \
 formstock.cpp \
 mcaravanadueno.cpp \
 formmodificartri.cpp \
 formprefcaravanas.cpp \
 mprefcategorias.cpp \
 evlista.cpp \
 einformeimpresora.cpp \
 edcategoriaespecial.cpp

TARGET = admincaravanascorrientes

DESTDIR = ../../bin/plugins

RESOURCES += aCarCorr.qrc

FORMS += FormMovimientoBase.ui \
 FormPrefCaravanas.ui

INCLUDEPATH += ../../utiles \
               ../../reporte \
               ../../src

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a

PRE_TARGETDEPS += ../../bin/libutiles.a

DISTFILES += admincaravanascorrientes.SQLITE.sql \
 admincaravanascorrientes.QMYSQL.sql

TRANSLATIONS += admincaravanascorrientes.ts
