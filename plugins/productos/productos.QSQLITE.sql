CREATE TABLE IF NOT EXISTS producto (id INTEGER PRIMARY KEY AUTOINCREMENT, id_categoria NUMERIC, codigo TEXT NOT NULL, nombre TEXT NOT NULL, precio_costo NUMERIC NULL, precio_venta NUMERIC, descripcion BLOB, marca TEXT, stock NUMERIC DEFAULT NULL, habilitado NUMERIC NOT NULL DEFAULT 1, modelo TEXT NULL, CONSTRAINT 'codigo_unico' UNIQUE ( codigo ) ON CONFLICT ABORT );
CREATE TABLE IF NOT EXISTS categoria_producto (id INTEGER PRIMARY KEY AUTOINCREMENT, nombre TEXT, descripcion TEXT );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'categoria_producto', '0' );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'producto', '0' );
INSERT OR IGNORE INTO "categoria_producto"( "id" ) VALUES ( 0 );