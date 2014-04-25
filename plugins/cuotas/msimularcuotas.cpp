#include "msimularcuotas.h"
#include "mplancuota.h"

#include <QDate>

MSimularCuotas::MSimularCuotas(QObject *parent) :
    QAbstractTableModel(parent)
{
    sumatoria = new QHash<int, double>();
    faltante = new QHash<int, double>();
    cantidad = 0;
    interes = 0.0;
    importe = 0.0;
    pinicial = 0.0;
    periodo = MPlanCuota::Invalido;
}

MSimularCuotas::~MSimularCuotas()
{
    delete sumatoria;
    sumatoria=0;
    delete faltante;
    faltante = 0;
}

void MSimularCuotas::setCuotas( int c )
{
    int ant = cantidad;
    cantidad = c;
    if( ant < c ) {
        emit beginInsertRows( QModelIndex(), ant, c );
        insertRows( ant, c );
        emit endInsertRows();
    } else {
        emit beginRemoveRows( QModelIndex(), c, ant );
        removeRows( c, ant );
        emit endRemoveRows();
    }
}

void MSimularCuotas::regenerar()
{
    sumatoria->clear();
    faltante->clear();
    this->reset();
    faltante->insert( 0, importe*(1+(interes/100)) );
    sumatoria->insert( 0, 0.0 );
    double cuota = importe/cantidad;
    cuota *= 1+(interes/100);
    for( int i=1; i<=cantidad; i++ ) {
        sumatoria->insert( i, cuota * i );
        faltante->insert( i, faltante->value( i-1 ) - cuota );
    }
    emit dataChanged( index( 0, 0 ), index( rowCount(), columnCount() ) );
}

/*!
 * \fn MSimularCuotas::rowCount( const QModelIndex & ) const
 */
int MSimularCuotas::rowCount( const QModelIndex & ) const
{
    if( valido() )
        return cantidad+1;
    else
        return 0;
}

/*!
 * \fn MSimularCuotas::columnCount( const QModelIndex & ) const
 */
int MSimularCuotas::columnCount( const QModelIndex & ) const
{
    if( valido() )
        return 5;
    else
        return 0;
}

/*!
 * \fn MSimularCuotas::valido()
 */
bool MSimularCuotas::valido() const
{
    if( importe > 0 && cantidad > 0 && periodo != MPlanCuota::Invalido && interes > 0.0 && fecha_inicio.isValid() ) {
        return true;
    } else {
        return false;
        qWarning( "Modelo invalido!" );
    }

}

/*!
 * \fn MSimularCuotas::data( const QModelIndex &index, int role ) const
 */
QVariant MSimularCuotas::data( const QModelIndex &index, int role ) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( index.column() ) {
                case 0:
                {
                    if( index.row() == 0 ) {
                        return "Entrega Inicial";
                    } else {
                        return QString( "Cuota #%1" ).arg( index.row() );
                    }
                    break;
                }
                case 1:
                {
                    if( index.row() == 0 ) {
                        return fecha_inicio.toString( Qt::SystemLocaleShortDate );
                    } else {
                        return fecha_inicio.addDays( MPlanCuota::diasEnPeriodo( periodo, fecha_inicio ) * index.row() ).toString( Qt::SystemLocaleShortDate );
                    }
                    break;
                }
                case 2:
                {
                    if( index.row() == 0 ) {
                        return QString( "$ %L1" ).arg( pinicial, 10, 'f', 2 );
                    } else {
                        return QString( "$ %L1" ).arg( ( importe/cantidad )*( 1 + ( interes/100 ) ), 10, 'f', 2 );
                    }
                    break;
                }
                case 3:
                {
                    return QString( "$ %L1" ).arg( sumatoria->value( index.row() ), 10, 'f', 2 );
                    break;
                }
                case 4:
                {
                    return QString( "$ %L1" ).arg( faltante->value( index.row() ), 10, 'f', 2 );
                    break;
                }
                default:
                {
                    return QVariant();
                    break;
                }
            }
            break;
        }
        case Qt::EditRole:
        {
            switch( index.column() ) {
                case 1:
                {
                    if( index.row() == 0 ) {
                        return fecha_inicio;
                    } else {
                        return fecha_inicio.addDays( MPlanCuota::diasEnPeriodo( periodo, fecha_inicio ) * index.row() );
                    }
                    break;
                }
                case 2:
                {
                    if( index.row() == 0 ) {
                        return pinicial;
                    } else {
                        return ( importe/cantidad )*( 1 + ( interes/100 ) );
                    }
                    break;
                }
                case 3:
                {
                    return sumatoria->value( index.row() );
                    break;
                }
                case 4:
                {
                    return faltante->value( index.row() );
                    break;
                }
                default:
                {
                    return QVariant();
                }
            }
            break;
        }
        case Qt::TextAlignmentRole: {
            switch( index.column() ) {
                case 0:
                {
                    return int( Qt::AlignLeft | Qt::AlignVCenter );
                    break;
                }
                case 1:
                {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
                case 2:
                case 3:
                case 4:
                {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                    break;
                }
                default:
                {
                    return QVariant();
                }
            }
            break;
        }
        default:
        {
            return QVariant();
            break;
        }
    }
}

/*!
 * \fn MSimularCuotas::flags( const QModelIndex & ) const
 * Retorna los permisos para el modelo
 */
Qt::ItemFlags MSimularCuotas::flags(const QModelIndex & ) const
{
    return Qt::ItemFlags( !Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

QVariant MSimularCuotas::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch( section ) {
            case 0:
            {    return "#Cuota"; break; }
            case 1:
            { return "Fecha";     break; }
            case 2:
            { return "Importe";   break; }
            case 3:
            { return "Sumatoria"; break; }
            case 4:
            { return "Faltante";  break; }
            default:
            { return QString::number( section ); break; }
        }
    } else {
        return QVariant();
    }
}

