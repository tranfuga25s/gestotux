#include "vitemremito.h"
#include <QSqlQueryModel>
#include <QTableView>
#include <QHeaderView>

#include "mdescuentos.h"

VItemRemito::VItemRemito(QWidget *parent) :
    EVLista(parent)
{
    setObjectName( "lista_items_remito" );
    setWindowTitle( "Lista de items de remito" );

    modelo = new MProductosTotales( this );
    modelo->setearSoloLectura( true );
    modelo->buscarPrecios( false );
    modelo->calcularTotales( true );
    vista->setModel( modelo );
    vista->horizontalHeader()->setAlternatingRowColors( true );

    addAction( ActCerrar );
}

void VItemRemito::setearIdRemito( const int id_remito )
{
    QSqlQueryModel m;
    m.setQuery( QString(
                     "SELECT cantidad, texto, precio_unitario "
                     "FROM item_remito "
                     "WHERE id_remito = %1;"
                     ).arg( id_remito ) );
    for( int i = 0; i < m.rowCount(); i++ ) {
        modelo->agregarItem(
                    m.data( m.index( i, 0 ), Qt::EditRole ).toInt(),
                    m.data( m.index( i, 1 ), Qt::EditRole ).toString(),
                    m.data( m.index( i, 2 ), Qt::EditRole ).toDouble()
                    );
    }
    // veo los descuentos
    MDescuentos *md = new MDescuentos();
    md->setearTipo( MDescuentos::Remito );
    md->setearIdComprobante( id_remito );
    if( md->existenDatos() ) {
        md->seleccionarDatos();
        for( int i =0; i<md->rowCount(); i++ ) {
            modelo->agregarDescuento(
                        md->data( md->index( i, 1), Qt::DisplayRole ).toString(),
                        md->data( md->index( i, 2 ), Qt::EditRole ).toDouble() );
        }
    }
    delete md;
}
