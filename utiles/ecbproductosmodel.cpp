#include "ecbproductosmodel.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

ECBProductosModel::ECBProductosModel(QObject *parent) :
QAbstractTableModel(parent)
{
    _ids = new QHash<int, int>();
    _codigos = new QHash<int, QString>();
    _proveedor = new QHash<int, int>();
    _nombres = new QHash<int, QString>();
    _stock = new QHash<int, double>();
    _habilitado = new QHash<int, bool>();
    this->_min = -1;
}

ECBProductosModel::~ECBProductosModel() {
    delete _ids;
    _ids=0;
    delete _proveedor;
    _proveedor=0;
    delete _nombres;
    _nombres=0;
    delete _stock;
    _stock=0;
    delete _habilitado;
    _habilitado=0;
    delete _codigos;
    _codigos=0;
}

QVariant ECBProductosModel::data( const QModelIndex &index, int role ) const
{
    switch( index.column() ) {
        case Ids:
        {
            if( role == Qt::DisplayRole ) {
                return _ids->value( index.row() );
            }
            break;
        }
        case Codigos:
        {
            if( role == Qt::DisplayRole ) {
                return _codigos->value( index.row() );
            }
            break;
        }
        case Nombres:
        {
            if( role == Qt::DisplayRole ) {
                return _nombres->value( index.row() );
            }
            break;
        }
        case Stock:
        {
            if( role == Qt::DisplayRole ) {
                return _stock->value( index.row() );
            }
            break;
        }
        case Habilitado:
        {
            if( role == Qt::DisplayRole ) {
                return _habilitado->value( index.row() );
            }
            break;
        }
        case IdProveedor:
        {
            if( role == Qt::DisplayRole ) {
                return _proveedor->value( index.row() );
            }
            break;
        }
        default:
        { break; }
    }
    return QVariant();
}

/*!
 * \brief ECBProductosModel::columnCount
 * \param parent
 * \return
 */
int ECBProductosModel::columnCount( const QModelIndex & ) const
{ return 6; }

/*!
 * \brief ECBProductosModel::rowCount
 * \return
 */
int ECBProductosModel::rowCount(const QModelIndex & ) const
{  return _ids->size(); }

/*!
 * \brief ECBProductosModel::inicializar
 */
void ECBProductosModel::inicializar()
{
    // Cargo los datos del modelo
    QSqlQuery cola;
    QString tcola;
    tcola.append( "SELECT id, codigo, nombre, stock, habilitado, ( SELECT id_proveedor FROM compras WHERE id  IN ( SELECT MAX( id_compra ) FROM compras_productos as  cp WHERE cp.id_producto = p.id ) ) AS id_proveedor FROM producto AS p ORDER BY nombre ASC" );
    if( cola.exec( tcola ) ) {
        int pos = 0;
        this->_ids->clear();
        this->_codigos->clear();
        this->_nombres->clear();
        this->_stock->clear();
        this->_habilitado->clear();
        this->_proveedor->clear();
        while( cola.next() ) {
            this->_ids->insert( pos, cola.record().value(0).toInt() );
            this->_codigos->insert( pos, cola.record().value(1).toString() );
            this->_nombres->insert( pos, cola.record().value(2).toString() );
            this->_stock->insert( pos, cola.record().value(3).toDouble() );
            this->_habilitado->insert( pos, cola.record().value(4).toBool() );
            this->_proveedor->insert( pos, cola.record().value(5).toInt() );
            pos++;
        }
        if( pos == 0 ) {
            qWarning( "No hay ningun producto para cargar!" );
        }
    } else {
        qWarning( "Error al intentar ejecutar la cola para cargar los productos" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}


/*!
 * \brief MProductosTotales::insertRow
 * \param row
 * \param parent
 * \return
 */
bool ECBProductosModel::insertRow( int row, const QModelIndex& parent )
{
 if( row == -1 )
 { row = this->_ids->size(); }
 beginInsertRows( parent, row, row );
 this->_ids       ->insert( row, 0 );
 this->_codigos   ->insert( row, QString() );
 this->_nombres   ->insert( row, QString() );
 this->_stock     ->insert( row, -1.0 );
 this->_habilitado->insert( row, true );
 this->_proveedor ->insert( row, 0 );
 endInsertRows();
 emit dataChanged( this->index( row, 0 ), this->index( row, this->columnCount() ) );
 emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() ) );
 return true;
}

/*!
 * \brief ECBProductosModelo::removeRow
 * \param row
 * \param parent
 * \return
 */
bool ECBProductosModel::removeRow( int row, const QModelIndex& parent )
{
  beginRemoveRows( parent, row, row );
  // Actualizo los indices de todos los que están después de la fila que estamos eliminando.
  for( int i = row; i < this->_ids->count(); i++ ) {
      this->_ids->insert( i, this->_ids->value( i + 1 ) );
      this->_codigos->insert( i, this->_codigos->value( i + 1 ) );
      this->_nombres->insert( i, this->_nombres->value( i + 1 ) );
      this->_stock->insert( i, this->_stock->value( i + 1 ) );
      this->_habilitado->insert( i, this->_habilitado->value( i + 1 ) );
      this->_proveedor->insert( i, this->_proveedor->value( i + 1 ) );
  }
  // Las posiciones estan basadas en indice base 0
  this->_ids       ->remove( this->_ids->size()-1        );
  this->_codigos   ->remove( this->_codigos->size()-1    );
  this->_nombres   ->remove( this->_nombres->size()-1    );
  this->_stock     ->remove( this->_stock->size()-1      );
  this->_habilitado->remove( this->_habilitado->size()-1 );
  this->_proveedor ->remove( this->_proveedor->size()-1  );
  endRemoveRows();
  emit dataChanged( this->index( row, 0 ), this->index( this->rowCount(), this->columnCount() ) );
  return true;
}

/*!
 * \brief ECBProductosModel::agregarItem
 * \param texto
 * \param stock
 * \param habilitado
 * \param proveedor
 */
int ECBProductosModel::agregarItem( const QString texto, double stock, bool habilitado, int proveedor )
{
    int pos = this->_ids->size();
    this->insertRow( -1 );

    this->_ids       ->insert( pos, _min                          );
    this->_codigos   ->insert( pos, QString::number( this->_min ) );
    this->_nombres   ->insert( pos, texto                         );
    this->_habilitado->insert( pos, habilitado                    );
    this->_stock     ->insert( pos, stock                         );
    this->_proveedor ->insert( pos, proveedor                     );

    this->_min--;

    emit dataChanged( this->index( pos, 0 ), this->index( pos, this->columnCount() ) );
    emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() ) );
    return pos;
}

/*!
 * \brief ECBProductosModel::arreglarItemTemporal
 * Actualiza el valor de ID temporal de un producto que no estaba en la base de datos con el nuevo dato.
 * \param anterior ID temporal anterior.
 * \param nuevo ID Nuevo
 * \return
 */
bool ECBProductosModel::arreglarItemTemporal( const int anterior, const int nuevo )
{
    if( anterior > 0 ) {
        return false;
    }
    QHash<int, int>::iterator pos_anterior = this->_ids->find( anterior );
    if( pos_anterior == this->_ids->end() ) {
        qDebug() << "El ID anterior no estaba en la lista";
        return false;
    }
    this->_ids->insert( pos_anterior.key(), nuevo );
    /// @TODO Ver si se utiliza la versión de agregar por ventana de agregar producto para actualizar los otros datos.
    return true;
}

/*!
 * \brief ECBProductosModel::existeID
 * Verifica si existe el ID de producto elegido
 * \param id_producto Identificador del producto
 * \return Verdadero si existe
 */
bool ECBProductosModel::existeID( const int id_producto )
{
    return this->_ids->contains( id_producto );
}

/*!
 * \brief ECBProductosModel::nombreProductoSegunID
 * Devuelve el nombre del producto que está indicado según el ID indicado
 * \param id_producto ID Producto
 * \return Devolverá un valor vacío si el producto no existe.
 */
QString ECBProductosModel::nombreProductoSegunID( const int id_producto )
{
    int pos = this->_ids->key( id_producto );
    if( pos >= 0 ) {
        return this->_nombres->value( pos );
    } else {
        return QString();
    }
}
