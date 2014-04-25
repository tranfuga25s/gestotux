# Template para los archivos pro de cada cliente en particular

TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
 ordered

SUBDIRS = utiles/utiles.pro \
          src/src.pro \
	  utiles/estilo/estilo.pro \
	  plugins/aCarCorr/aCarCorr.pro

NOMBRE_PROGRAMA = acarcorr
ARCHIVO_RC = aCarCorr/icono.rc

unix {
  system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
  system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}