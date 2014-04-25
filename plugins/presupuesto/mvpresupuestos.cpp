#include "mvpresupuestos.h"
#include <QDate>
#include <QColor>

MVPresupuestos::MVPresupuestos(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable( "v_presupuestos" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "#Presupuesto" );
    setHeaderData( 2, Qt::Horizontal, "Destinatario" );
    setHeaderData( 3, Qt::Horizontal, "Fecha" );
    setHeaderData( 4, Qt::Horizontal, "Total" );
}


QVariant MVPresupuestos::data(const QModelIndex& idx, int role) const
{
 switch( role )
 {
        case Qt::DisplayRole:
        {
                switch( idx.column() )
                {
                        case 1:
                        case 2:
                        {
                                return QSqlRelationalTableModel::data( idx, role ).toString();
                                break;
                        }
                        case 3:
                        {
                                return QSqlRelationalTableModel::data( idx, role ).toDate();
                                break;
                        }
                        case 4:
                        {
                                return QString( "$ %L1" ).arg( QSqlRelationalTableModel::data(idx,role).toDouble(), 10, 'f', 2 ).append("  ");
                                break;
                        }
                        default:
                        {
                                return QSqlRelationalTableModel::data( idx, role );
                                break;
                        }
                }
                break;
        }
        case Qt::EditRole:
        {
                switch( idx.column() )
                {
                        case 1:
                        {
                                return QSqlRelationalTableModel::data( idx, role );
                                break;
                        }
                        case 2:
                        {
                                return QSqlRelationalTableModel::data( idx, role ).toString();
                                break;
                        }
                        case 3:
                        {
                                return QSqlRelationalTableModel::data( idx, role ).toDate();
                                break;
                        }
                        case 4:
                        {
                                return QSqlRelationalTableModel::data( idx, role ).toDouble();
                                break;
                        }
                        default:
                        {
                                return QSqlRelationalTableModel::data( idx, role);
                                break;
                        }
                }
                break;
        }
        case Qt::TextAlignmentRole:
        {
                switch( idx.column() )
                {
                        case 2:
                        {  return int( Qt::AlignLeft | Qt::AlignVCenter ); break; }
                        case 1:
                        case 3:
                        { return int( Qt::AlignCenter | Qt::AlignVCenter ); break; }
                        case 4:
                        { return int( Qt::AlignRight | Qt::AlignVCenter ); break; }
                        default:
                        { return QSqlRelationalTableModel::data( idx, role ); break; }
                }
                break;
        }
        default:
        {
                return QSqlRelationalTableModel::data( idx, role );
                break;
        }
 }
}

Qt::ItemFlags MVPresupuestos::flags(const QModelIndex & ) const
{
    return Qt::ItemFlags( !Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}