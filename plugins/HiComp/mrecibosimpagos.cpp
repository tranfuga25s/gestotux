#include "mrecibosimpagos.h"

#include <QDate>

MRecibosImpagos::MRecibosImpagos(QObject *parent) :
QSqlTableModel(parent)
{
    setTable( "v_recibos_impagos" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "#Serie" );
    setHeaderData( 2, Qt::Horizontal, "Destinatario" );
    setHeaderData( 3, Qt::Horizontal, "Fecha emision" );
    setHeaderData( 4, Qt::Horizontal, "Monto" );
    setHeaderData( 5, Qt::Horizontal, "Cancelado" );
}

QVariant MRecibosImpagos::data( const QModelIndex &idx, int role ) const
{
    switch( idx.column() ) {
        case 0:
        case 1:
        {
            switch ( role ) {
                case Qt::DisplayRole: {
                    return QString( "#%1" ).arg( QSqlTableModel::data( idx, Qt::DisplayRole ).toString() );
                    break;
                }
                case Qt::EditRole: {
                    if( idx.column() == 0 ) {
                        return QSqlTableModel::data( idx, Qt::EditRole ).toInt();
                    } else {
                        return QSqlTableModel::data( idx, Qt::EditRole ).toString();
                    }
                    break;
                }
                default: { break; }
            }
            break;
        }
        case 3:
        {
            switch ( role ) {
                case Qt::DisplayRole: {
                    return QSqlTableModel::data( idx, Qt::DisplayRole ).toDate().toString( Qt::LocaleDate );
                    break;
                }
                case Qt::TextAlignmentRole: {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
                default: { break; }
            }
            break;
        }
        case 4:
        {
            switch ( role ) {
                case Qt::DisplayRole: {
                    return QString( "$ %L1" ).arg( QSqlTableModel::data( idx, Qt::DisplayRole ).toDouble(), 10, 'f', 2 );
                    break;
                }
                case Qt::TextAlignmentRole: {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                    break;
                }
                default: { break; }
            }
            break;
        }
        case 5: {
            switch ( role ) {
                case Qt::DisplayRole: {
                return QSqlTableModel::data( idx, Qt::DisplayRole ).toBool();
                    break;
                }
                case Qt::TextAlignmentRole: {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
                default: { break; }
            }
            break;
        }
        default: { break; }
    }
    return QSqlTableModel::data( idx, role );
}
