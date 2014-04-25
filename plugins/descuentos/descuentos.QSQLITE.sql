CREATE  TABLE  IF NOT EXISTS "tipo_descuento" ("id_tipo_descuento" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "codigo" TEXT UNIQUE , "nombre" TEXT NOT NULL , "cantidad" DOUBLE NOT NULL );
CREATE  TABLE  IF NOT EXISTS "descuentos" ("id_descuento" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "texto" TEXT NOT NULL , "cantidad" DOUBLE NOT NULL );
CREATE  TABLE  IF NOT EXISTS "descuento_comprobante" ("id_descuento" INTEGER NOT NULL , "id_comprobante" INTEGER NOT NULL , "tipo" TEXT NOT NULL , PRIMARY KEY ( "id_descuento", "id_comprobante", "tipo" ) );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'tipo_descuento', '1' );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'descuentos', '1' );