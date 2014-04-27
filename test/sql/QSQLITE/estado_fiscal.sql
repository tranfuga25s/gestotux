CREATE TABLE IF NOT EXISTS estado_fiscal ( "id_estado_fiscal" INTEGER PRIMARY KEY autoincrement, "titulo" TEXT NOT NULL );
INSERT OR IGNORE INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (1, 'Responsable Inscripto');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (2, 'Responsable Monotributista');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (3, 'No Responable Inscripto');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (4, 'Exento');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (5, 'Consumidor Final');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (6, 'Monotributista Social');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (7, 'Pequeño Contribuidor Eventual');
INSERT OR IGNORE  INTO `estado_fiscal` (`id_estado_fiscal`, `titulo`) VALUES (8, 'Pequeño Contribuyente Eventual Social');