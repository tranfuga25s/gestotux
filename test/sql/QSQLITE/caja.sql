CREATE TABLE IF NOT EXISTS caja ( id_caja INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, nombre TEXT NOT NULL, fecha_alta DATETIME NOT NULL, saldo DOUBLE NOT NULL );
INSERT INTO caja VALUES ( 1, "Caja predeterminada", "2015-04-20", 10.0);