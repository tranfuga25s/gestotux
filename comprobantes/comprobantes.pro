#-------------------------------------------------
# Sistema de comprobantes estaticos
#-------------------------------------------------

QT       += sql svg xml

TARGET = comprobantes
TEMPLATE = lib

DEFINES += COMPROBANTES_LIBRARY

SOURCES += comprobantes.cpp \
    comprobanteestatico.cpp

HEADERS += comprobantes.h\
           comprobantes_global.h \
    comprobanteestatico.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1F2119B
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = comprobantes.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    test.svg
