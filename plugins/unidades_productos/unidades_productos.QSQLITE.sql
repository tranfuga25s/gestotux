CREATE TABLE IF NOT EXISTS "unidades_productos" ("id_unidad" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "nombre" TEXT NOT NULL  UNIQUE , "id_padre" INTEGER NOT NULL , "multiplo" DOUBLE NOT NULL  DEFAULT 0.0);
CREATE TABLE IF NOT EXISTS "productos_unidades" ("id_producto" INTEGER NOT NULL , "id_unidad" INTEGER NOT NULL , "predeterminado" BOOL NOT NULL  DEFAULT FALSE, PRIMARY KEY ("id_producto", "id_unidad"));
INSERT OR IGNORE INTO "unidades_productos"( "id_unidad", "nombre", "id_padre", "multiplo" ) VALUES ( 1, "Metricas", 0, 1.0 );
INSERT OR IGNORE INTO "unidades_productos"( "id_unidad", "nombre", "id_padre", "multiplo" ) VALUES ( 2, "Metro", 1, 1.0 );
INSERT OR IGNORE INTO "unidades_productos"( "id_unidad", "nombre", "id_padre", "multiplo" ) VALUES ( 3, "Centimetros", 1, 0.1 );
INSERT OR IGNORE INTO "unidades_productos"( "id_unidad", "nombre", "id_padre", "multiplo" ) VALUES ( 4, "Milimetros", 1, 0.001 );
INSERT OR IGNORE INTO "unidades_productos"( "id_unidad", "nombre", "id_padre", "multiplo" ) VALUES ( 3, "Kilometro", 1, 100.0 );