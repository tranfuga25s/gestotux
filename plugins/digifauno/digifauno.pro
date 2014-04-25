SOURCES += digifauno.cpp \
           mdueno.cpp \
           vduenos.cpp \
           mmascota.cpp \
           formagregarmasctoa.cpp \
           formagregarservicio.cpp \
           mpeluqueria.cpp \
           vmascota.cpp \
           dmascota.cpp \
           eresumen.cpp \
           mrazas.cpp \
           dmiembros.cpp \
           resumengeneral.cpp \
           formprefopciones.cpp \
           formlistapeluqueria.cpp \
           ../proveedor/mproveedor.cpp
HEADERS += digifauno.h \
           mdueno.h \
           mmascota.h \
           formagregarmasctoa.h \
           formagregarservicio.h \
           mpeluqueria.h \
           vmascota.h \
           dmascota.h \
           eresumen.h \
           mrazas.h \
           dmiembros.h \
           resumengeneral.h \
           formprefopciones.h \
           formlistapeluqueria.h \
           vduenos.h \
           ../proveedor/mproveedor.h


CONFIG += warn_on \
          thread \
          qt \
          exceptions \
          dll \
          plugin \
          help

QT += sql \
      xml

RESOURCES = embebido.qrc

FORMS = FormAgregarMascota.ui \
         FormServPeluqueria.ui \
         FPrefGeneral.ui \
         FormPrefOpcionesBase.ui \
         FormListaPeluqueriaBase.ui

win32 {
    RC_FILE = icono.rc
}

TEMPLATE = lib

DESTDIR = ../../bin/plugins

LIBS += ../../bin/libutiles.a

TRANSLATIONS += digifauno.ts

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

TARGET = digifauno

PRE_TARGETDEPS += ../../bin/libutiles.a

OTHER_FILES = digifauno.QMYSQL.sql \
              digifauno.QSQLITE.sql
