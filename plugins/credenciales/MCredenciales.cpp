#include "MCredenciales.h"

/*
CREATE TABLE IF NOT EXISTS `credenciales` (
  `numero_credencial` bigint(20) NOT NULL AUTO_INCREMENT,
  `nombre` text COLLATE utf8_spanish_ci NOT NULL,
  `dni` int(11) NOT NULL,
  `medio` text COLLATE utf8_spanish_ci NOT NULL,
  `acc1` tinyint(1) NOT NULL,
  `acc2` tinyint(1) NOT NULL,
  `acc3` tinyint(1) NOT NULL,
  `acc4` tinyint(1) NOT NULL,
  `acc5` tinyint(1) NOT NULL,
  `acc6` tinyint(1) NOT NULL,
  PRIMARY KEY (`numero_credencial`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci AUTO_INCREMENT=1 ;
*/

MCredenciales::MCredenciales(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable( "credenciales" );
    setHeaderData( 0, Qt::Horizontal, "#Credencial" );
    setHeaderData( 1, Qt::Horizontal, "Equipo" );
    setRelation( 1, QSqlRelation( "equipos", "id_equipo", "nombre" ) );
    setHeaderData( 2, Qt::Horizontal, "Nombre" );
    setHeaderData( 3, Qt::Horizontal, "DNI" );
    setHeaderData( 4, Qt::Horizontal, "Medio" );
    setHeaderData( 5, Qt::Horizontal, "Vest" );
    setHeaderData( 6, Qt::Horizontal, "Conf Pren" );
    setHeaderData( 7, Qt::Horizontal, "Cab" );
    setHeaderData( 8, Qt::Horizontal, "Tech" );
    setHeaderData( 9, Qt::Horizontal, "Plat SO" );
    setHeaderData( 10, Qt::Horizontal, "Plat SE" );
}
