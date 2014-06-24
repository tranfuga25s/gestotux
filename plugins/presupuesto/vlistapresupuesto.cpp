#include "vlistapresupuesto.h"

#include <QTableView>
#include <QHeaderView>
#include <mproductostotales.h>

#include "mdescuentos.h"

VListaPresupuesto::VListaPresupuesto(QWidget *parent) :
    EVLista(parent)
{
    this->setObjectName( "vista_presupuesto" );
    this->setWindowTitle( "Listado de items" );
    //this->setWindowIcon( QIcon( ":/imagenes/????.png" ) );

    modelo = new MProductosTotales( this );
    modelo->buscarPrecios( false );
    modelo->calcularTotales( true );
    modelo->setearSoloLectura( true );
    vista->setModel( modelo );

    addAction( ActCerrar );

}

/*!
 * \brief VListaPresupuesto::setearIdPresupuesto
 * Carga los datos del presupuesto indicado como parametro
 * \param id_presupuesto Identificador del presupuesto a cargar
 */
void VListaPresupuesto::setearIdPresupuesto( const int id_presupuesto )
{
  // Cargo todos los datos
    MItemPresupuesto *m = new MItemPresupuesto();
    m->setearId( id_presupuesto );
    m->select();
    for( int i=0; i<m->rowCount(); i++ ) {
        modelo->agregarItem(
                    m->data( m->index( i, 2 ), Qt::EditRole ).toDouble(),
                    m->data( m->index( i, 3 ), Qt::EditRole ).toString(),
                    m->data( m->index( i, 4 ), Qt::EditRole ).toDouble()
                    );
    }
    delete m;
    m=0;
    // Cargo los datos si es necesario.
    MDescuentos *md = new MDescuentos();
    md->setearIdComprobante( id_presupuesto );
    md->setearTipo( MDescuentos::Presupuesto );
    if( md->existenDatos() ) {
        md->seleccionarDatos();
        for( int i=0; i<md->rowCount(); i++ ) {
            modelo->agregarDescuento(
                    md->data( md->index( i, 1 ), Qt::DisplayRole ).toString(),
                    md->data( md->index( i, 2 ), Qt::EditRole ).toDouble()
                    );
        }
    }
    delete md;
    md=0;
}
