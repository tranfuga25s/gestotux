# Compilación para JHAU-PORA
TEMPLATE = subdirs
CONFIG += warn_on \
          qt \
          thread  \
          ordered

SUBDIRS = reporte/reporte.pro \
          utiles/utiles.pro \
          utiles/estilos/estilos.pro \
          plugins/default/default.pro \
          plugins/CtaCte/CtaCteLib.pro \
          plugins/ventas/VentasLib.pro \
          plugins/caja/CajaLib.pro \
          plugins/cuotas/CuotasLib.pro \
          plugins/descuentos/DescuentosLib.pro \
          plugins/proveedor/proveedor.pro \
          plugins/productos/productos.pro \
          plugins/compras/compras.pro \
          plugins/ventas/ventas.pro \
          plugins/informes/informa.pro \
          plugins/informes/balance/balance.pro \
          plugins/informes/compras/compras.pro \
          src/src.pro \
          plugins/jhau-pora/jhau-pora.pro

NOMBRE_PROGRAMA = jhau-pora
ARCHIVO_RC = jhau-pora/jhau-pora.rc

unix {
  system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
  system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}