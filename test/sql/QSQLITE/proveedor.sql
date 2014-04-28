CREATE TABLE IF NOT EXISTS proveedor ( id INTEGER PRIMARY KEY AUTOINCREMENT, nombre TEXT NOT NULL UNIQUE, direccion BLOB, email TEXT, fax TEXT, telefono_celular TEXT, telefono_linea TEXT, cuit_cuil TEXT NULL UNIQUE );
INSERT OR IGNORE INTO "sqlite_sequence" VALUES( 'proveedor', '0' );
INSERT OR IGNORE INTO proveedor VALUES ( 1, "Proveedor 1", "Direccion 1", "email1@gmail.com", null, null, null, null );
INSERT OR IGNORE INTO proveedor VALUES ( 2, "Proveedor 2", "Direccion 2", "email2@gmail.com", null, null, null, null );