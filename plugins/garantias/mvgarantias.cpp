#include "mvgarantias.h"

#include <QDate>

MVGarantias::MVGarantias(QObject *parent) :
    QSqlTableModel(parent)
{
     setTable( "v_garantia" );
     setHeaderData( 0, Qt::Horizontal, "#IDGarantia" );
     setHeaderData( 1, Qt::Horizontal, "#Garantia" );
     setHeaderData( 2, Qt::Horizontal, "Nombre de Producto" );
     setHeaderData( 3, Qt::Horizontal, "Cliente" );
     setHeaderData( 4, Qt::Horizontal, "Fecha de Inicio" );
     setHeaderData( 5, Qt::Horizontal, "Fecha de Fin" );
     setHeaderData( 6, Qt::Horizontal, "Fecha de baja" );
     setHeaderData( 7, Qt::Horizontal, "Razon de baja" );
     setHeaderData( 8, Qt::Horizontal, "#Producto" );
     setHeaderData( 9, Qt::Horizontal, "#Equipamiento" );
}

QVariant MVGarantias::data(const QModelIndex &idx, int role) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( idx.column() ) {
                case 4:
                case 5:
                case 6:
                {
                    return QSqlTableModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                case 1:
                {
                    return QString( "#%1").arg( QSqlTableModel::data( idx, role ).toString() );
                    break;
                }
            }
            break;
        }
        case Qt::TextAlignmentRole:
        {
            switch( idx.column() ) {
                case 4:
                case 5:
                case 6:
                case 1:
                {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
            }
            break;
        }
        case Qt::BackgroundColorRole:
        {
            QDate fecha_fin = QSqlTableModel::data( index( idx.row(), fieldIndex( "fecha_fin" ) ), Qt::EditRole ).toDate();
            if( fecha_fin <= QDate::currentDate() ) {
                return Qt::lightGray;
            }
            break;
        }
    }
    return QSqlTableModel::data( idx, role );
}
