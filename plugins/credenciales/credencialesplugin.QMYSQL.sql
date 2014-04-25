CREATE TABLE IF NOT EXISTS `equipos` (
  `id_equipo` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` tinytext COLLATE utf8_spanish_ci NOT NULL,
  `activo` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id_equipo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;
CREATE TABLE IF NOT EXISTS `credenciales` (
  `id_credencial` bigint(20) NOT NULL AUTO_INCREMENT,
  `equipo_id` int(11) NOT NULL,
  `nombre` text COLLATE utf8_spanish_ci NOT NULL,
  `dni` int(11) NOT NULL,
  `medio` text COLLATE utf8_spanish_ci NOT NULL,
  `acc1` tinyint(1) NOT NULL DEFAULT 0,
  `acc2` tinyint(1) NOT NULL DEFAULT 0,
  `acc3` tinyint(1) NOT NULL DEFAULT 0,
  `acc4` tinyint(1) NOT NULL DEFAULT 0,
  `acc5` tinyint(1) NOT NULL DEFAULT 0,
  `acc6` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`numero_credencial`),
  FOREIGN KEY (`equipo_id`) REFERENCES `equipos`(`id_equipo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;