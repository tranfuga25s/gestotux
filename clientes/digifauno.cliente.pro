# El Fauno  - Veterinaria y peluqueria canina
TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
 ordered

SUBDIRS = utiles \
        src \
	utiles/estilos/estilos.pro \
	plugins/digifauno/digifauno.pro \
	plugins/ventas/ventas.pro \
	plugins/gastos/gastos.pro \
	plugins/productos/productos.pro

NOMBRE_PROGRAMA = Digifauno
ARCHIVO_RC = digifauno/icono.rc

unix {
  system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
  system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
