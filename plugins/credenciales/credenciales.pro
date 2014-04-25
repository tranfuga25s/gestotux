TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql

TARGET = credenciales

DESTDIR = ../../bin/plugins/

PRE_TARGETDEPS += ../../bin/libreporte.a \
                  ../../bin/libutiles.a

INCLUDEPATH += ../../src \
               ../../reporte \
               ../../utiles \

LIBS += ../../bin/libutiles.a \
        ../../bin/libreporte.a


HEADERS += DCredencial.h \
    credencialesplugin.h \
    MCredenciales.h \
    mequipos.h \
    vequipos.h \
    vcredenciales.h \
    dbusquedacredencial.h

SOURCES += DCredencial.cpp \
    credencialesplugin.cpp \
    MCredenciales.cpp \
    mequipos.cpp \
    vequipos.cpp \
    vcredenciales.cpp \
    dbusquedacredencial.cpp

FORMS += DCredencial.ui \
    dbusquedacredencial.ui

OTHER_FILES += \
    credencialesplugin.QMYSQL.sql \
    credencialesplugin.QSQLITE.sql

RESOURCES += credenciales.qrc
