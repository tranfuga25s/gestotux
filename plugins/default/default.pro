TEMPLATE = lib

CONFIG += dll \
          plugin

QT += sql \
      xml

INCLUDEPATH += ../../src \
               ../../utiles \
               ../../reporte

HEADERS += gestotuxdefault.h

SOURCES += gestotuxdefault.cpp

TARGET = zinfo

DESTDIR = ../../bin/plugins/

TRANSLATIONS += default.ts

RESOURCES += default.qrc

OTHER_FILES += default.QMYSQL.sql \
               default.QSQLITE.sql

DISTFILES += ../../bin/reportes/Factura.xml \
             ../../bin/reportes/Recibo.xml \
             ../../bin/reportes/Presupuesto.xml

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}