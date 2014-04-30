CREATE TABLE IF NOT EXISTS categoria_producto (id INTEGER PRIMARY KEY AUTOINCREMENT, nombre TEXT, descripcion TEXT );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'categoria_producto', '0' );
INSERT OR IGNORE INTO "categoria_producto"( "id" ) VALUES ( 0 );
INSERT OR IGNORE INTO "categoria_producto"( "id", "nombre" ) VALUES ( 1, "Categoria 1" );