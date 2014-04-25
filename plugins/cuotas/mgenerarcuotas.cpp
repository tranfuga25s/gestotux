#include "mgenerarcuotas.h"

#include "mpagos.h"
#include "mplancuota.h"
#include "../ventas/MFactura.h"
#include "../remitos/MRemito.h"
#include "mclientes.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>
#include <QDebug>

MGenerarCuotas::MGenerarCuotas( QObject *parent )
: QAbstractTableModel( parent )
{
    _numeros = new QHash<int, int>();
    _planes = new QHash<int, int>();
    _clientes = new QHash<int, QString>();
    _clientes_id = new QHash<int, int>();
    _cuotas = new QHash<int, QString>();
    _importes = new QHash<int, double>();
    _comprobantes = new QHash<int, NumeroComprobante*>();

    _total = 0.0;
    _cant = 0;
    _mes_busqueda = QDate::currentDate().month();
}

MGenerarCuotas::~MGenerarCuotas()
{
    delete _numeros;
    delete _planes;
    delete _clientes;
    delete _clientes_id;
    delete _cuotas;
    delete _importes;
    delete _comprobantes;
    _numeros = 0;
    _planes = 0;
    _clientes = 0;
    _clientes_id = 0;
    _cuotas = 0;
    _importes = 0;
    _comprobantes = 0;

}

QVariant MGenerarCuotas::data( const QModelIndex &idx, int role ) const
{
    if( _numeros->keys().isEmpty() ) {
        return QVariant();
    }
    int clave = _numeros->keys().at( idx.row() );
    switch( role ) {
      case Qt::DisplayRole:
      {
          switch( idx.column() ) { // Numero interno
              case 0:
              {
                return QString( "#%1" ).arg( _numeros->value( clave ) );
                break;
              }
              case 1: // Numero de plan de cuota
              {
                return QString( "#%1" ).arg( _planes->value( clave ) );
                  break;
              }
              case 2: // Nombre del cliente
              {
                  return _clientes->value( clave );
                  break;
              }
              case 3: // # Cuota ( 1/10 )
              {
                  return _cuotas->value( clave );
                  break;
              }
              case 4: // Importe de la cuota
              {
                  return QString( "$ %L1" ).arg( _importes->value( clave ), 10, 'f', 2 );
                  break;
              }
              case 5:
              {
                  NumeroComprobante *num = _comprobantes->value( clave );
                  return num->aCadena();
                  break;
              }
              default:
              { break; }
          }
          break;
      }
      case Qt::EditRole:
      {
        switch( idx.column() )
        {
            case 0:
            {
              return _numeros->value( clave );
              break;
            }
            case 1: // Numero de plan de cuota
            {
              return _planes->value( clave );
                break;
            }
            case 2: // Nombre del cliente
            {
                return _clientes_id->value( clave );
                break;
            }
            case 3: // # Cuota ( 1/10 )
            {
                return _cuotas->value( clave );
                break;
            }
            case 4: // Importe de la cuota
            {
                return _importes->value( clave );
                break;
            }
            default:
            { break; }
        }
        break;
      }
/*      case Qt::TextAlignmentRole: {
            switch( idx.column() )
            {
                case 0:
                {
                  return _numeros->value( idx.row() );
                  break;
                }
                case 1: // Numero de plan de cuota
                {
                  return _planes->value( idx.row() );
                    break;
                }
                case 2: // Nombre del cliente
                {
                    return _clientes_id->value( idx.row() );
                    break;
                }
                case 3: // # Cuota ( 1/10 )
                {
                    return _cuotas->value( idx.row() );
                    break;
                }
                case 4: // Importe de la cuota
                {
                    return _importes->value( idx.row() );
                    break;
                }
                case 5:
                {
                    return _comprobantes->value( idx.row() ).aCadena();
                    break;
                }
                default:
                { break; }
            }
            break;
      }*/
      default:
      { break; }
    }
    return QVariant();
}

int MGenerarCuotas::columnCount( const QModelIndex & ) const
{ return 6; }

int MGenerarCuotas::rowCount( const QModelIndex & ) const
{ return _cant; }

Qt::ItemFlags MGenerarCuotas::flags( const QModelIndex & ) const
{ return Qt::ItemFlags( !Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled ); }

QVariant MGenerarCuotas::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch( section ) {
            case 0:
            { return "#Num";    break; }
            case 1:
            { return "#Plan";   break; }
            case 2:
            { return "Cliente"; break; }
            case 3:
            { return "#Cuota";  break; }
            case 4:
            { return "Importe"; break; }
            case 5:
            { return "#Recibo"; break; }
            default:
            { return section;   break; }
        }
    }
    return QAbstractTableModel::headerData( section, orientation, role );
}

bool MGenerarCuotas::calcularComprobantes()
{
   _cant = 0;
   _total = 0.0;
   // Fecha del mes actual - Inicio del mes
   QDate inicio_mes = QDate( QDate::currentDate().year(), _mes_busqueda, 1 );
   QDate fin_mes = inicio_mes;
   fin_mes = fin_mes.addMonths( 1 );
   // Busco el próximo número de recibo
   //NumeroComprobante *num = new NumeroComprobante( 0, -1, -1 );
   NumeroComprobante num = MPagos::proximoSerieNumeroRecibo();

   QSqlQuery cola;
   if( cola.exec( QString( " SELECT ic.id_item_cuota, ic.id_plan_cuota, ic.num_cuota, pc.cantidad_cuotas, ic.monto, pc.id_factura, pc.tipo_comprobante "
                           " FROM item_cuota AS ic "
                           "      INNER JOIN plan_cuota AS pc ON ic.id_plan_cuota = pc.id_plan_cuota "
                           " WHERE ic.fecha_pago IS NULL "
                           "   AND ic.id_recibo  IS NULL " // Cuando se emite el recibo de la cuota este campo no debe estar vacío
                           "   AND ic.fecha_vencimiento <= \"%1\" "
                           "   AND ic.fecha_vencimiento >= \"%2\" "
                           " GROUP BY ic.id_plan_cuota   "
                           " HAVING MIN( ic.num_cuota )  "
                           " ORDER BY ic.id_plan_cuota, ic.num_cuota " )
                    .arg( fin_mes.toString( Qt::ISODate ) )
                    .arg( inicio_mes.toString( Qt::ISODate ) ) ) ) {
       while( cola.next() ) {
           beginInsertRows( QModelIndex(), _cant, _cant );
           _numeros ->insert( _cant, cola.record().value( "id_item_cuota" ).toInt() );
           _planes  ->insert( _cant, cola.record().value( "id_plan_cuota" ).toInt() );
           _cuotas  ->insert( _cant, QString( "%2/%1" )
                                          .arg( cola.record().value( "cantidad_cuotas" ).toInt() )
                                          .arg( cola.record().value( "num_cuota" ).toInt() ) );
           if( cola.record().value( "tipo_comprobante" ).toInt() == MPlanCuota::Factura ) {
               int id_factura = cola.record().value("id_factura").toInt();
               int id_cliente = MFactura::obtenerIdCliente( id_factura );
               _clientes_id->insert( _cant, id_cliente );
               _clientes->insert( _cant, MClientes::getRazonSocial( id_cliente ) );
           } else if( cola.record().value( "tipo_comprobante" ).toInt() == MPlanCuota::Remito ) {
               int id_remito = cola.record().value("id_factura").toInt();
               int id_cliente = MRemito::obtenerIdCliente( id_remito );
               _clientes_id->insert( _cant, id_cliente );
               _clientes->insert( _cant, MClientes::getRazonSocial( id_cliente ) );
           } else {
               qDebug() << "Tipo de comprobante desconocido";
           }
           double monto = cola.record().value( "monto" ).toDouble();
           _total += monto;
           _importes->insert( _cant, monto );
           _comprobantes->insert( _cant, new NumeroComprobante( num ) );
           endInsertRows();
           _cant++;
           num.siguienteNumero();
       }

       if( _cant > 0 ) {
           emit cambioTotal( _total );
           emit cambioCantidad( _cant );
           emit dataChanged( index( 0, 0 ), index( _cant, 5 ) );
           return true;
       } else {
           emit cambioTotal( 0.0 );
           emit cambioCantidad( 0 );
           return false;
       }
   } else {
       qDebug() << "Error al ejecutar la cola de averiguamiento de los datos de las cuotas a pagar";
       qDebug() << cola.lastError().text();
       qDebug() << cola.lastQuery();
   }
   return false;
}


