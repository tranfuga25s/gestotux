CREATE TABLE IF NOT EXISTS "servicios" ("id_servicio" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "nombre" TEXT NOT NULL , "descripcion" TEXT, "fecha_alta" DATETIME NOT NULL , "fecha_baja" DATETIME, "precio_base" DOUBLE NOT NULL , "periodo" INTEGER NOT NULL , "dia_cobro" INTEGER NOT NULL , "forma_incompleto" INTEGER NOT NULL );
INSERT OR IGNORE INTO "servicios" VALUES( 0, 'servicio de internet',NULL,'2015-01-01',NULL,100,3,1,2);
INSERT OR IGNORE INTO "servicios" VALUES( 1, 'servicio de internet',NULL,'2015-01-06',NULL,100,3,1,2);
INSERT OR IGNORE INTO "servicios" VALUES( 2, 'Servicio de internet',NULL,'2015-03-25',NULL,130,3,2,2);
INSERT OR IGNORE INTO "servicios" VALUES( 3, 'servicio de internet',NULL,'2012-01-01',NULL,100,3,1,2);