#include "vdiezmos.h"

#include <QTableView>
#include <QMessageBox>

#include "mdiezmos.h"
#include "ddardiezmo.h"

VDiezmos::VDiezmos( QWidget *parent ) :
EVLista(parent)
{
    setObjectName( "visor_diezmos" );
    setWindowTitle( "Diezmos" );
    setWindowIcon( QIcon( ":/imagenes/diezmo.png" ) );

    modelo = new MDiezmos( this );

    vista->setModel( modelo );
    modelo->select();

    vista->hideColumn( 0 );
    vista->hideColumn( modelo->fieldIndex( "id_referencia" ) );

    addAction( ActEliminar );
    addAction( ActCerrar );
}

/*!
 * \brief VDiezmos::agregar
 * \param autoeliminarid
 */
void VDiezmos::agregar( bool )
{ return; }

/*!
 * \brief VDiezmos::eliminar
 * Slot llamado para eliminar un elemento de diezmo
 */
void VDiezmos::eliminar()
{
    if( vista->selectionModel()->selectedRows().count() <= 0 ) {
        QMessageBox::information( this, "Error", "Por favor, seleccione al menos algún elemento para eliminarlo." );
        return;
    }
    foreach( QModelIndex indice, vista->selectionModel()->selectedRows() ) {
        int id_entrada_diezmo = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
        if( !modelo->eliminarEntrada( id_entrada_diezmo ) ) {
            QMessageBox::warning( this, "Error", "No se pudo eliminar la entrada de diezmo" );
        }
    }
}
