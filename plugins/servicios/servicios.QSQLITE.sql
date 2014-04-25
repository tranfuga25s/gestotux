CREATE TABLE IF NOT EXISTS "main"."servicios" ("id_servicio" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "nombre" TEXT NOT NULL , "descripcion" TEXT, "fecha_alta" DATETIME NOT NULL , "fecha_baja" DATETIME, "precio_base" DOUBLE NOT NULL , "periodo" INTEGER NOT NULL , "dia_cobro" INTEGER NOT NULL , "forma_incompleto" INTEGER NOT NULL );
CREATE TABLE IF NOT EXISTS "main"."servicios_clientes" ("id_servicio" INTEGER NOT NULL , "id_cliente" INTEGER NOT NULL , "fecha_alta" DATETIME NOT NULL , "fecha_baja" DATETIME, "razon" TEXT NULL, PRIMARY KEY ( "id_servicio", "id_cliente" ) );
CREATE TABLE IF NOT EXISTS "main"."recargos" ("id_recargo" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "id_servicio" INTEGER NOT NULL, "cant_dias" INTEGER NOT NULL , "porcentaje" DOUBLE, "recargo" DOUBLE);
CREATE TABLE IF NOT EXISTS "main"."periodo_servicio" ("id_periodo_servicio" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL , "id_servicio" INTEGER NOT NULL , "periodo" INTEGER NOT NULL , "ano" INTEGER NOT NULL , "fecha" DATETIME NOT NULL , "fecha_inicio" DATETIME NOT NULL , "fecha_fin" DATETIME NOT NULL );
CREATE TABLE IF NOT EXISTS "main"."cobro_servicio_cliente_periodo" ("id_periodo_servicio" INTEGER NOT NULL , "id_servicio" INTEGER NOT NULL , "id_cliente" INTEGER NOT NULL , "id_factura" INTEGER NOT NULL , "id_recibo" INTEGER, "id_ctacte" INTEGER, PRIMARY KEY ("id_periodo_servicio", "id_servicio", "id_cliente"));
CREATE TABLE IF NOT EXISTS "main"."recargo_cobro_servicio_cliente" ("id_periodo_servicio" INTEGER NOT NULL , "id_servicio" INTEGER NOT NULL , "id_cliente" INTEGER NOT NULL , "id_recargo" INTEGER NOT NULL , "fecha_generado" DATETIME NOT NULL  DEFAULT CURRENT_TIMESTAMP, "texto_detalle" TEXT NOT NULL , "precio" DOUBLE NOT NULL , "id_ctacte" INTEGER NOT NULL , "id_recibo" INTEGER, PRIMARY KEY ("id_periodo_servicio", "id_servicio", "id_cliente", "id_recargo") );
INSERT OR IGNORE INTO "main"."sqlite_sequence" ( "name", "seq" ) VALUES ( "servicios", 0 );
INSERT OR IGNORE INTO "main"."sqlite_sequence" ( "name", "seq" ) VALUES ( "recargos", 0 );
INSERT OR IGNORE INTO "main"."sqlite_sequence" ( "name", "seq" ) VALUES ( "periodo_servicio", 0 );
INSERT OR IGNORE INTO "main"."sqlite_sequence" ( "name", "seq" ) VALUES ( "cobro_servicio_cliente_periodo", 0 );
CREATE VIEW IF NOT EXISTS `v_servicios_clientes` AS SELECT id_servicio, id_cliente, fecha_alta, fecha_baja, razon, id_servicio AS idservicio, id_cliente AS idcliente FROM servicios_clientes;