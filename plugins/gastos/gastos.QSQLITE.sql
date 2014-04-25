CREATE TABLE IF NOT EXISTS categoria_gastos (id_categoria_gastos INTEGER PRIMARY KEY AUTOINCREMENT, nombre TEXT UNIQUE NOT NULL, descripcion TEXT );
CREATE TABLE IF NOT EXISTS gastos ( id_gasto INTEGER PRIMARY KEY AUTOINCREMENT, id_categoria INTEGER, descripcion TEXT, costo NUMERIC NOT NULL DEFAULT 0, fecha TEXT, id_caja INTEGER );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES (  'categoria_gastos', '1' );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'gastos', '1' );