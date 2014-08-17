CREATE TABLE IF NOT EXISTS "unidades_productos" ("id_unidad" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "nombre" TEXT NOT NULL  UNIQUE , "id_padre" INTEGER NOT NULL , "multiplo" DOUBLE NOT NULL  DEFAULT 0.0);
CREATE TABLE IF NOT EXISTS "productos_unidades" ("id_producto" INTEGER NOT NULL , "id_unidad" INTEGER NOT NULL , "predeterminado" BOOL NOT NULL  DEFAULT FALSE, PRIMARY KEY ("id_producto", "id_unidad"));