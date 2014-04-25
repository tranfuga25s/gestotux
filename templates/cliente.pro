# Template para los archivos pro de cada cliente en particular

TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
 ordered

SUBDIRS = utiles/utiles.pro \
          src/src.pro \
	  utiles/estilo/estilo.pro

NOMBRE_PROGRAMA = ?????
ARCHIVO_RC = ????/???.rc

unix {
  system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
  system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}