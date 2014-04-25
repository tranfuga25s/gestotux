#include "mtempproductoremarcar.h"

#include "mproductos.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMutableListIterator>

MTempProductoRemarcar::MTempProductoRemarcar( QObject *parent ) :
  QAbstractTableModel( parent )
{
  _id_productos = new QList<int>();
  _codigos = new QList<QString>();
  _nombres = new QList<QString>();
  _precio_original = new QList<double>();
  _sin_stock = new QList<bool>();
  _deshabilitado = new QList<bool>();

  _fijo = false;
  _porcentaje = false;
  _valor = 0.0;

}

void MTempProductoRemarcar::setearValorFijo( bool p )
{
  _fijo = p;
  _porcentaje = !p;
  emit dataChanged( index( 0, 3 ), index( rowCount(), 3 ) );
}

void MTempProductoRemarcar::setearPorcentaje( bool p )
{
  _fijo = !p;
  _porcentaje = p;
  emit dataChanged( index( 0, 3 ), index( rowCount(), 3 ) );
}

int MTempProductoRemarcar::columnCount( const QModelIndex & ) const
{ return 5; }

int MTempProductoRemarcar::rowCount( const QModelIndex & ) const
{ return _id_productos->size(); }

QVariant MTempProductoRemarcar::data(const QModelIndex &index, int role) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( index.column() ) {
                case 0: // #Id de producto
                {
                    return QString( "#%1" ).arg( _id_productos->at( index.row() ) );
                    break;
                }
                case 1: // Codigo del producto
                {
                    return QString( "%1" ).arg( _codigos->at( index.row() ) );
                    break;
                }
                case 2: // Nombre del producto
                {
                    return _nombres->at( index.row() );
                    break;
                }
                case 3: // Precio Original
                {
                    return QString( "$ %L1" ).arg( _precio_original->at( index.row() ), 10, 'f', 2 );
                    break;
                }
                case 4: // Precio Nuevo
                {
                    if( _porcentaje ) {
                        return QString( "$ %L1" ).arg( _precio_original->at( index.row() )*(_valor+1.0), 10, 'f', 2 );
                    } else if( _fijo ) {
                        return QString( "$ %L1" ).arg( _precio_original->at( index.row() )+_valor, 10, 'f', 2 );
                    }
                    break;
                }
                default:
                { break; }
            }
            break;
        }
        default:
        { break; }
    }
    return QVariant();
}

QVariant MTempProductoRemarcar::headerData(int section, Qt::Orientation orientation, int role) const
{
  if( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch( section ) {
          case 0:
            { return "#ID Producto"; break; }
          case 1:
            { return QString::fromUtf8( "Código" ); break; }
          case 2:
            { return QString::fromUtf8( "Nombre del producto" ); break; }
          case 3:
            { return QString::fromUtf8( "Precio Original" ); break; }
          case 4:
            { return QString::fromUtf8( "Nuevo precio" ); break; }
          default:
            { return QAbstractTableModel::headerData( section, orientation, role ); break; }
        }
  }
  return QAbstractTableModel::headerData( section, orientation, role );
}

void MTempProductoRemarcar::agregarProducto( int id )
{

    // Busco si no existe ya
    if( _id_productos->indexOf( id ) != -1 )
        return;

    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id, codigo, nombre, precio_venta, habilitado, stock FROM producto WHERE id = %1" ).arg( id ) ) ) {
        cola.next();
        beginInsertRows( QModelIndex(), rowCount(), rowCount() );
        int pos = rowCount();
        _id_productos->insert( pos, cola.record().value(0).toInt() );
        _codigos->insert( pos, cola.record().value(1).toString() );
        _nombres->insert( pos, cola.record().value(2).toString() );
        _precio_original->insert( pos, cola.record().value(3).toDouble() );
        _deshabilitado->insert( pos, !cola.record().value(4).toBool() );
        if( cola.record().value(5).toInt() > 0 ) {
            _sin_stock->insert( pos, false );
        } else {
            _sin_stock->insert( pos, true );
        }
        endInsertRows();
    } else {
        qWarning( "Error al consultar los datos del producto" );
        qDebug( cola.lastError().text().toLocal8Bit() );
        qDebug( cola.lastQuery().toLocal8Bit() );
    }
}

void MTempProductoRemarcar::eliminarProducto( QModelIndex idx )
{

  if( idx.row() >= this->rowCount() )
      return;

  emit beginRemoveRows( QModelIndex(), idx.row(), idx.row() );
  // Elimino la fila que corresponde moviendo todos los demás elementos
  for( int i = idx.row(); i<_id_productos->size()-1; i++ ) {
      _id_productos->replace( i, _id_productos->at( i+1 ) );
      _codigos->replace( i, _codigos->at( i+1 ) );
      _nombres->replace( i, _nombres->at( i+1 ) );
      _precio_original->replace( i, _precio_original->at( i+1 ) );
      _sin_stock->replace( i, _sin_stock->at( i+1 ) );
      _deshabilitado->replace( i, _deshabilitado->at( i+1 ) );
  }
  // Elimino el último elemento de cada uno
  _id_productos->removeLast();
  _codigos->removeLast();
  _nombres->removeLast();
  _precio_original->removeLast();
  _sin_stock->removeLast();
  _deshabilitado->removeLast();
  endRemoveRows();
}

void MTempProductoRemarcar::eliminarTodos()
{
    beginRemoveRows( QModelIndex(), 0, this->rowCount() );
    _id_productos->clear();
    _codigos->clear();
    _nombres->clear();
    _precio_original->clear();
    _sin_stock->clear();
    _deshabilitado->clear();
    endRemoveRows();
}

void MTempProductoRemarcar::cambioSinStock( bool estado )
{
    if( !estado )
        return;

    for( int i = 0; i<_id_productos->size(); i++ ) {
        if( _sin_stock->at( i ) ) {
            eliminarProducto( index( i, 0 ) );
            i--; // Como corro todos los indices para arriba, tengo que volver a evaluar el elemento que acabo de eliminar ya que es un elemento siguiente traspasado
        }
    }
    return;
}

void MTempProductoRemarcar::cambioDeshabilitados( bool estado )
{
    if( !estado )
        return;

    for( int i = 0; i<_id_productos->size(); i++ ) {
        if( _deshabilitado->at( i ) ) {
            eliminarProducto( index( i, 0 ) );
            i--; // Como corro todos los indices para arriba, tengo que volver a evaluar el elemento que acabo de eliminar ya que es un elemento siguiente traspasado
        }
    }
    return;
}

/*!
 * \brief MTempProductoRemarcar::cambioValor
 * Actualiza los valores de precios nuevos
 * \param valor Valor a aplicar
 */
void MTempProductoRemarcar::cambioValor( double valor )
{
    if( _porcentaje ) {
        _valor = valor/100.0;
    } else {
        _valor = valor;
    }
    emit dataChanged( index( 0, 3 ), index( rowCount(), 3 ) );
}

/*!
 * \brief MTempProductoRemarcar::remarcar
 * Remarca los productos seleccionados
 * \return Par de int, 1º cantidad cambiados, 2º cantidad no cambiados
 */
QPair<int, int> MTempProductoRemarcar::remarcar()
{
    bool condicion = true;
    int cambiados = 0;
    int no_cambiados = 0;
    int contador = 0;
    while( condicion ) {
        if( _fijo ) {
            if( MProductos::remarcarFijo( _id_productos->at( contador ), _valor ) ) {
                cambiados++;
                eliminarProducto( index( contador, 0 ) );
                // No aumento el contador porque elimino el producto y se corren los indices hacia arriba.
            } else {
                no_cambiados++;
                contador++;
            }
        } else if( _porcentaje ) {
            if( MProductos::remarcarPorcentaje( _id_productos->at( contador ), _valor ) ) {
                cambiados++;
                eliminarProducto( index( contador, 0 ) );
            } else {
                no_cambiados++;
                contador++;
            }
        } else {
            qWarning( "Metodo desconocido!" );
            condicion = false;
        }

        if( _id_productos->size() <= 0 )
            condicion = false;

        if( contador >= _id_productos->size() )
            condicion = false;

        emit aumentoProgreso( 1 );
    }
    return QPair<int,int>( cambiados, no_cambiados );
}

