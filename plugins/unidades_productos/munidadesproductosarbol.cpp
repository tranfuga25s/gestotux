#include "munidadesproductosarbol.h"

#include "munidadesproductos.h"
#include <QDebug>

#include <QSqlQuery>

MUnidadesProductosArbol::MUnidadesProductosArbol(QObject *parent) :
    QProxyModel(parent)
{
    // Inicializo el modelo
    this->mup = new MUnidadesProductos( this );
    this->setModel( this->mup );
    this->mup->select();

    // Recorro todos los registros y cargo los datos
    int total = mup->rowCount();
    for( int i=0; i<total; i++ ) {


        int id = mup->data( mup->index( i, 0 ), Qt::EditRole ).toInt();
        int id_padre = mup->data( mup->index( i, mup->fieldIndex( "id_padre") ), Qt::EditRole ).toInt();

        if( _adb.contains( id_padre ) ) {
            int ultima_pos = _adb.value( id_padre ).size();
            _adb[id_padre].insert( ultima_pos, id );
        } else {
            _adb.insert( id_padre, QHash<int, int>() );
            _adb[id_padre].insert( 0, id );
        }

    }

}

/**
 * @brief MUnidadesProductosArbol::columnCount
 * @param parent
 * @return
 */
int MUnidadesProductosArbol::columnCount(const QModelIndex &parent) const
{
    if( parent.isValid() ) {
        return 2;
    } else {
        return 1;
    }
}

/**
 * @brief MUnidadesProductosArbol::rowCount
 * @param parent
 * @return
 */
int MUnidadesProductosArbol::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() ) {
        return _adb.value( parent.row() ).count();
    } else {
        return _adb.count();
    }
}

/**
 * @brief MUnidadesProductosArbol::hasChildren
 * @param parent
 * @return
 */
bool MUnidadesProductosArbol::hasChildren(const QModelIndex &parent) const
{
    if( parent.isValid() ) {
        return ( _adb.value(parent.row()).count() > 0 );
    } else {
        return ( _adb.count() > 0 );
    }
}

/**
 * @brief MUnidadesProductosArbol::data
 * @param index
 * @param role
 * @return
 */
QVariant MUnidadesProductosArbol::data( const QModelIndex &index, int role ) const
{
    if( index.parent().isValid() ) {

    } else {

    }
}

/**
 * @brief MUnidadesProductosArbol::flags
 * @param index
 * @return
 */
Qt::ItemFlags MUnidadesProductosArbol::flags( const QModelIndex &index ) const
{
    /*QFlags f;
    f << Qt::ItemIsEnabled << Qt::ItemIsSelectable << !Qt::ItemIsEditable;
    return f;*/
}

