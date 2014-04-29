CREATE TABLE IF NOT EXISTS compras_productos ( id INTEGER PRIMARY KEY AUTOINCREMENT, id_compra NUMERIC , id_producto NUMERIC  , precio_compra NUMERIC , cantidad NUMERIC  );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'compras_productos', '1' );
INSERT INTO compras_productos VALUES ( 1, 1, 1, 10.0, 1.0 );
INSERT INTO compras_productos VALUES ( 2, 2, 2, 10.0, 2.0 );