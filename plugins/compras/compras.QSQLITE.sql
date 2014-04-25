CREATE TABLE IF NOT EXISTS compras ( id INTEGER PRIMARY KEY AUTOINCREMENT,id_proveedor NUMERIC NOT NULL, fecha TEXT NOT NULL, total NUMERIC NOT NULL, id_caja NUMERIC NULL, "contado" BOOL NOT NULL  DEFAULT 1 );
CREATE TABLE IF NOT EXISTS compras_productos ( id INTEGER PRIMARY KEY AUTOINCREMENT, id_compra NUMERIC , id_producto NUMERIC  , precio_compra NUMERIC , cantidad NUMERIC  );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'compras', '1' );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( 'compras_productos', '1' );
CREATE VIEW IF NOT EXISTS resumen_compra AS SELECT COUNT( id ) as cantidad, fecha, strftime( fecha, "%m" ) as mes, strftime( fecha, "%Y" ) as ano, SUM( total ) total, id_proveedor, contado FROM compras;