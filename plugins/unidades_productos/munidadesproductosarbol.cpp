#include "munidadesproductosarbol.h"

#include "munidadesproductos.h"
#include <QDebug>

MUnidadesProductosArbol::MUnidadesProductosArbol(QObject *parent) :
    QProxyModel(parent)
{
    // Inicializo el modelo
    this->mup = new MUnidadesProductos( this );
    this->setModel( this->mup );
    this->mup->select();

    // Recorro todos los registros y cargo los datos
    int total = mup->rowCount();
    qDebug() << "Total de registros: " << total;
    for( int i=0; i<total; i++ ) {


        int id = mup->data( mup->index( i, 0 ), Qt::EditRole ).toInt();
        int id_padre = mup->data( mup->index( i, mup->fieldIndex( "id_padre") ), Qt::EditRole ).toInt();

        if( _adb.contains( id_padre ) ) {
            qDebug() << "Agregado hijo";
            int ultima_pos = _adb.value( id_padre ).size();
            _adb[id_padre].insert( ultima_pos, id );
        } else {
            qDebug() << "Agregado padre";
            _adb.insert( id_padre, QHash<int, int>() );
            _adb[id_padre].insert( 0, id );
        }

    }
    qDebug() << _adb;
}

