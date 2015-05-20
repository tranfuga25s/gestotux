# HiComp Computacion - Sergio Sack
TEMPLATE = subdirs
CONFIG = warn_on \
         qt \
         thread  \
         ordered

SUBDIRS = utiles/utiles.pro \
          reporte/reporte.pro \
          utiles/estilos/estilos.pro \
          plugins/HiComp/HiComp.pro \
          plugins/CtaCte/CtaCteLib.pro \
          plugins/ventas/VentasLib.pro \
          plugins/remitos/RemitosLib.pro \
          plugins/caja/CajaLib.pro \
          plugins/cuotas/CuotasLib.pro \
          plugins/descuentos/DescuentosLib.pro \
          plugins/pagos/PagosLib.pro \
          plugins/productos/productos.pro \
          plugins/pagos/pagos.pro \
          plugins/servicios/servicios.pro \
          plugins/CtaCte/CtaCte.pro \
          plugins/caja/caja.pro \
          plugins/ventas/ventas.pro \
          plugins/cuotas/cuotas.pro \
          plugins/presupuesto/presupuesto.pro \
          plugins/gastos/gastos.pro \
          plugins/compras/compras.pro \
          plugins/proveedor/proveedor.pro \
          plugins/remitos/remitos.pro \
          plugins/informes/informa.pro \
          plugins/informes/informes.pro \
          src/src.pro \
          test/test.pro

NOMBRE_PROGRAMA = ../../bin/hicomp
ARCHIVO_RC = HiComp/hicomp.rc

unix {
  #system( generar_pri.sh $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}
win32 {
 system( generar_pri.bat $$NOMBRE_PROGRAMA $$ARCHIVO_RC )
}

DEFINES += GESTOTUX_HICOMP
