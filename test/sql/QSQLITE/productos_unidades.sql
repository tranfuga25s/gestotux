CREATE TABLE IF NOT EXISTS "productos_unidades" ("id_unidad" INTEGER, "id_producto" INTEGER NOT NULL, PRIMARY KEY ( id_unidad, id_producto ) );
INSERT INTO "productos_unidades"( "id_unidad", "id_producto" ) VALUES ( 1, 1 );
INSERT INTO "productos_unidades"( "id_unidad", "id_producto" ) VALUES ( 1, 2 );
INSERT INTO "productos_unidades"( "id_unidad", "id_producto" ) VALUES ( 1, 3 );