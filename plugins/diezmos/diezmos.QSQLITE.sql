CREATE TABLE IF NOT EXISTS `diezmos` ( `id_diezmo` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `fecha` DATETIME NOT NULL, `debe` DOUBLE( 20,3 ) NULL DEFAULT 0.0, `haber` DOUBLE( 20, 4 ) NOT NULL, `id_referencia` BIGINT DEFAULT NULL, `tipo_referencia` INTEGER DEFAULT 0 );
INSERT OR IGNORE INTO "sqlite_sequence" ( "name", "seq" ) VALUES ( "diezmos", 1 );