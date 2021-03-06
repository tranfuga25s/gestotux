CREATE TABLE IF NOT EXISTS producto (id INTEGER PRIMARY KEY AUTOINCREMENT, id_categoria NUMERIC, codigo TEXT NOT NULL, nombre TEXT NOT NULL, precio_costo NUMERIC NULL, precio_venta NUMERIC, descripcion BLOB, marca TEXT, stock NUMERIC DEFAULT NULL, habilitado NUMERIC NOT NULL DEFAULT 1, modelo TEXT NULL, CONSTRAINT 'codigo_unico' UNIQUE ( codigo ) ON CONFLICT ABORT );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'producto', '0' );
INSERT OR IGNORE INTO producto VALUES ( 1, 0, 1, "Producto 1", 10.0, 15.0, "Descripcion 1", "Marca 1", 1, 1, "Modelo 1" );
INSERT OR IGNORE INTO producto VALUES ( 2, 1, 2, "Producto 2", 10.0, 15.0, "Descripcion 2", "Marca 2", 1, 1, "Modelo 2" );
INSERT OR IGNORE INTO producto VALUES ( 3, 1, 6, "Producto 3", 10.0, 15.0, "Descripcion 3", "Marca 3", 1, 1, "Modelo 3" );
INSERT OR IGNORE INTO producto VALUES ( 4, 1, 3, "Producto 4", 10.0, 15.0, "Descripcion 3", "Marca 3", 1, 0, "Modelo 3" );
INSERT OR IGNORE INTO producto VALUES ( 5, 1, 5, "Producto 5", 10.0, 15.0, "Descripcion 4", "Marca 4", 1, 'true', "Modelo 4" );