TEMPLATE = subdirs

SUBDIRS +=  ProductosTest \
            PeriodoServicioTest \
            #BajaClienteServicioTest \
            PreferenciasTest \
            EquipamientosTest \
            CancelacionPeriodoServicio \
    ECBProductosTest \
    ComprasTest \
    ProveedorTest \
    VentasTest

OTHER_FILES += sql/dependences.ini \
               sql/QSQLITE/productos.sql \
               sql/QSQLITE/servicios.sql \
               sql/QSQLITE/categorias_productos.sql \
               sql/QSQLITE/periodo_servicio.sql \
               sql/QSQLITE/cobro_servicio_periodo.sql \
               sql/QSQLITE/provincias.sql \
               sql/QSQLITE/equipamientos.sql \
               sql/QSQLITE/paises.sql \
               sql/QSQLITE/escado_fiscal.sql \
               sql/QSQLITE/compras.sql \
               sql/QSQLITE/compras_productos.sql \
    sql/QSQLITE/proveedor.sql