#include "mequipos.h"

MEquipos::MEquipos(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "equipos" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "Nombre" );
    setHeaderData( 2, Qt::Horizontal, "Activo" );
}
