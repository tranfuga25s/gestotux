#include "mestadofiscal.h"

MEstadoFiscal::MEstadoFiscal(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "estado_fiscal" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "Titulo" );
}
