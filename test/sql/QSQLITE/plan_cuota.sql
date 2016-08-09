 CREATE TABLE IF NOT EXISTS `plan_cuota` ( `id_plan_cuota` INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL, `id_factura` INTEGER NOT NULL, `cantidad_cuotas` INTEGER NOT NULL DEFAULT 2, `tipo_periodo` INTEGER NOT NULL DEFAULT 0, `fecha_inicio` DATETIME NOT NULL, `entrega_inicial` DOUBLE NULL DEFAULT NULL, `recargo` DOUBLE NOT NULL, `tipo_comprobante` INTEGER NOT NULL, `total` DOUBLE NOT NULL DEFAULT 0.0 );