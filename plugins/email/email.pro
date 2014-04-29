TEMPLATE = lib

CONFIG += dll \
          plugin \
          help

QT += sql \
      network

TARGET = email

DESTDIR = ../../bin/plugins/

LIBS += ../../bin/libutiles.a

PRE_TARGETDEPS += ../../bin/libutiles.a

SOURCES += emodelomails.cpp \
           eservidoremail.cpp \
           formprefemail.cpp \
    smtpclient.cpp \
    quotedprintable.cpp \
    mimetext.cpp \
    mimepart.cpp \
    mimemultipart.cpp \
    mimemessage.cpp \
    mimeinlinefile.cpp \
    mimehtml.cpp \
    mimefile.cpp \
    mimecontentformatter.cpp \
    mimeattachment.cpp \
    emailaddress.cpp \
    enviadoremail.cpp \
    pluginemail.cpp

HEADERS += emodelomails.h \
           eservidoremail.h \
           formprefemail.h \
    SmtpMime \
    smtpclient.h \
    quotedprintable.h \
    mimetext.h \
    mimepart.h \
    mimemultipart.h \
    mimemessage.h \
    mimeinlinefile.h \
    mimehtml.h \
    mimefile.h \
    mimecontentformatter.h \
    mimeattachment.h \
    emailaddress.h \
    enviadoremail.h \
    pluginemail.h

FORMS += FormPrefEmailBase.ui

INCLUDEPATH += ../../src \
               ../../utiles

OTHER_FILES += email.QMYSQL.sql \
              email.QSQLITE.sql

RESOURCES += email.qrc

exists( ../../travis.pri ) {
    include( ../../travis.pri )
}