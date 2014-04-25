#include "vordentrabajo.h"

#include "eactcerrar.h"
#include "eactimprimir.h"
#include "eactpdf.h"
#include "mordentrabajo.h"

#include "ordentrabajowizard.h"
#include "formordentrabajo.h"

#include <QTableView>
#include <QMenu>
#include <QMessageBox>

VOrdenTrabajo::VOrdenTrabajo( QWidget *parent ) :
EVLista( parent )
{
    setObjectName( "visor_ordenes_trabajo" );
    setWindowTitle( "Ordenes de trabajo" );
    setWindowIcon( QIcon( ":/imagenes/orden_trabajo.png" ) );

    modelo = new MOrdenTrabajo( this );
    modelo->relacionarDatos();

    vista->setModel( modelo );
    vista->hideColumn( modelo->fieldIndex( "id_equipamiento" ) );
    vista->hideColumn( modelo->fieldIndex( "id_orden_trabajo" ) );
    modelo->select();

    ActVer = new QAction( this );
    ActVer->setText( "Ver" );
    ActVer->setStatusTip( "Muestra la información de una orden de trabajo" );
    ActVer->setIcon( QIcon( ":/imagenes/detalle_orden_trabajo.png" ) );
    connect( ActVer, SIGNAL( triggered() ), this, SLOT( ver() ) );

    ActAgregar->setIcon( QIcon( ":/imagenes/crear_orden_trabajo.png" ) );

    addAction( ActAgregar );
    addAction( ActVer );
    addAction( ActModificar );
    addAction( ActEliminar );
    addAction( ActImprimir );
    addAction( ActPdf );
    addAction( ActVerTodos );
    addAction( ActCerrar );
}

/*!
 * \brief VOrdenTrabajo::agregar
 * \param autoeliminarid
 */
void VOrdenTrabajo::agregar( bool )
{
    OrdenTrabajoWizard *w = new OrdenTrabajoWizard( this );
    w->exec();
    connect( w, SIGNAL( actualizarModeloOrdenTrabajo() ), this, SLOT( verTodos() ) );
}

/*!
 * \brief VOrdenTrabajo::modificar
 */
void VOrdenTrabajo::modificar()
{
    if( vista->selectionModel()->selectedRows().count() <= 0 ) {
        QMessageBox::information( this,
                                  QString::fromUtf8( "Ninguna selección" ),
                                  QString::fromUtf8( "Por favor, seleccióne una orden de trabajo para verla" ) );
        return;
    }
    foreach( QModelIndex indice, vista->selectionModel()->selectedRows() ) {
        int id_orden_trabajo = modelo->data( modelo->index( indice.row(), 0 ), Qt::EditRole ).toInt();
        FormOrdenTrabajo *form = new FormOrdenTrabajo( this );
        form->setearIdOrdenTrabajo( id_orden_trabajo );
        emit agregarVentana( form );
    }
    return;
}

/*!
 * \brief VOrdenTrabajo::eliminar
 */
void VOrdenTrabajo::eliminar()
{
    QMessageBox::information( this, "No implementado", "Característica no implementada" );
    return;
    /// @TODO: implementar eliminar orden de trabajo
}

/*!
 * \brief VOrdenTrabajo::ver
 * Muestra el formulario de las ordenes de trabajo
 */
void VOrdenTrabajo::ver()
{
    if( vista->selectionModel()->selectedRows().count() <= 0 ) {
        QMessageBox::information( this,
                                  QString::fromUtf8( "Ninguna selección" ),
                                  QString::fromUtf8( "Por favor, seleccióne una orden de trabajo para verla" ) );
        return;
    }
    foreach( QModelIndex indice, vista->selectionModel()->selectedRows() ) {
        int id_orden_trabajo = modelo->data( modelo->index( indice.row(), 0 ), Qt::EditRole ).toInt();
        FormOrdenTrabajo *form = new FormOrdenTrabajo( this );
        form->setearIdOrdenTrabajo( id_orden_trabajo );
        emit agregarVentana( form );
    }
    return;
}

/*!
 * \brief VOrdenTrabajo::aPdf
 * Manda a pdf la orden de trabajo
 */
void VOrdenTrabajo::aPdf()
{
    QMessageBox::information( this, "No implementado", "Característica no implementada" );
    return;
    /// @TODO: implementar imprimir orden de trabajo a pdf
}

/*!
 * \brief VOrdenTrabajo::imprimir
 * Manda a imprimir la orden de trabajo seleccionada
 */
void VOrdenTrabajo::imprimir()
{
    QMessageBox::information( this, "No implementado", "Caraterística no implementada" );
    return;
     /// @TODO: implementar imprimir orden de trabajo
}


/*!
 * \brief VOrdenTrabajo::menuContextual
 * Genera el menu contextual dependiendo de el item seleccionado
 * \param indice Indice seleccionado
 * \param menu Menu a utilizar
 */
void VOrdenTrabajo::menuContextual( const QModelIndex &indice, QMenu *menu )
{
    if( indice.isValid() ) {
        menu->addAction( ActVer );
        menu->addAction( ActModificar );
        menu->addAction( ActEliminar );
        menu->addAction( ActPdf );
        menu->addAction( ActImprimir );
        menu->addSeparator();
    }
    menu->addAction( ActAgregar );
    return;
}
