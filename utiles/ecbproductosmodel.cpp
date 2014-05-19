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
{ return 5; }

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
    tcola.append( "SELECT id, codigo, nombre, stock, habilitado, ( SELECT id_proveedor FROM compras WHERE id  IN ( SELECT MAX( id_compra ) FROM compras_productos  WHERE id_producto = 1 ) ) AS id_proveedor FROM producto ORDER BY nombre ASC" );
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
 this->_ids->insert( row, 0 );
 this->_codigos->insert( row, 0 );
 this->_nombres->insert( row, 0 );
 this->_stock->insert( row, -1 );
 this->_habilitado->insert( row, true );
 this->_proveedor->insert( row, 0 );
 endInsertRows();
 emit dataChanged( this->index( row, 0 ), this->index( row, this->columnCount() ) );
 emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() ) );
 return true;
}