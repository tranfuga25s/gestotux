CREATE TABLE IF NOT EXISTS "unidades_productos" ("id_unidad" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "nombre" TEXT NOT NULL  UNIQUE , "id_padre" INTEGER NOT NULL , "multiplo" DOUBLE NOT NULL  DEFAULT 0.0);
INSERT OR IGNORE INTO "unidades_productos" VALUES ( 1, "Kilo", NULL, 0.0 );
INSERT OR IGNORE INTO "unidades_productos" VALUES ( 2, "Gramos", 1, 0.001 );