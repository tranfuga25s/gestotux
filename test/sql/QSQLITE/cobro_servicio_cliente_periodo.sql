CREATE TABLE IF NOT EXISTS "cobro_servicio_cliente_periodo" ("id_periodo_servicio" INTEGER NOT NULL , "id_servicio" INTEGER NOT NULL , "id_cliente" INTEGER NOT NULL , "id_factura" INTEGER NOT NULL , "id_recibo" INTEGER, "id_ctacte" INTEGER, PRIMARY KEY ("id_periodo_servicio", "id_servicio", "id_cliente"));