#include "mvequipamiento.h"

#include <QDate>

MVEquipamiento::MVEquipamiento(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "v_equipamiento" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "#Serie" );
    setHeaderData( 2, Qt::Horizontal, "Descripcion" );
    setHeaderData( 3, Qt::Horizontal, "#IDCliente" );
    setHeaderData( 4, Qt::Horizontal, "Cliente" );
    setHeaderData( 5, Qt::Horizontal, "#Elementos" );
    setHeaderData( 6, Qt::Horizontal, "Fecha de Compra" );
}

QVariant MVEquipamiento::data(const QModelIndex &idx, int role) const
{
    switch( idx.column() ) {
        case 6:
        {
            switch( role ) {
                case Qt::DisplayRole:
                {
                    return QSqlTableModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                case Qt::TextAlignmentRole:
                {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                }
                case Qt::BackgroundColorRole:
                {
                    if( !QSqlTableModel::data( index( idx.row(), fieldIndex( "fecha_baja" ) ), Qt::EditRole ).isNull() ) {
                        return Qt::lightGray;
                    }
                    break;
                }
            }
            break;
        }
        case 1:
        case 5:
        {
            switch( role ) {
                case Qt::TextAlignmentRole:
                {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                }
                case Qt::BackgroundColorRole:
                {
                    if( !QSqlTableModel::data( index( idx.row(), fieldIndex( "fecha_baja" ) ), Qt::EditRole ).isNull() ) {
                        return Qt::lightGray;
                    }
                    break;
                }
            }
            break;
        }
        default:
        {
            if( role == Qt::BackgroundColorRole )
            {
                if( !QSqlTableModel::data( index( idx.row(), fieldIndex( "fecha_baja" ) ), Qt::EditRole ).isNull() ) {
                    return Qt::lightGray;
                }
            }
            break;
        }
    }
    return QSqlTableModel::data( idx, role );
}
