BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS clientes (id INTEGER PRIMARY KEY AUTOINCREMENT, apellido TEXT NOT NULL, nombre TEXT NOT NULL, direccion TEXT, telefono TEXT );
CREATE TABLE IF NOT EXISTS `autos` ( `id_auto` INTEGER PRIMARY KEY, `patente` TEXT UNIQUE, `id_dueno` INTEGER REFERENCES `clientes`(`id`), `marca` text NOT NULL, `modelo` text, `ano` TEXT default NULL, `color` text );
CREATE TABLE IF NOT EXISTS `presupuesto` ( `id_presupuesto` INTEGER PRIMARY KEY AUTOINCREMENT, `fecha` TEXT NOT NULL, `kilometraje` INTEGER NOT NULL, `id_auto` INTEGER REFERENCES `autos`(`id_auto`), `total` NUMERIC NOT NULL,`titulo` text, `memo` text, `contenido` blob );
INSERT OR IGNORE INTO "sqlite_sequence" VALUES('presupuesto',1);
INSERT OR IGNORE INTO "sqlite_sequence" VALUES('clientes',1);
INSERT OR IGNORE INTO "sqlite_sequence" VALUES('autos',1);
COMMIT;