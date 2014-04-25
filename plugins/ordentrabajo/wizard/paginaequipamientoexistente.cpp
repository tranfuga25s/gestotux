#include "paginaequipamientoexistente.h"

#include "mequipamiento.h"
#include "ordentrabajowizard.h"

#include <QMessageBox>

PaginaEquipamientoExistente::PaginaEquipamientoExistente(QWidget *parent) :
    QWizardPage(parent)
{
    setupUi(this);

    setTitle( QString::fromUtf8( "Equipamiento existente" ) );
    setSubTitle( QString::fromUtf8( "Por favor, seleccione que equipamiento del cliente seleccionado está siendo utilizado para la orden de trabajo." ) );

    registerField( "equipamiento.id_equipamiento", TVEquipamientos );

    modelo = new MEquipamiento( TVEquipamientos );
    TVEquipamientos->setModel( modelo );
    TVEquipamientos->hideColumn( 0 );
    TVEquipamientos->setSelectionBehavior( QAbstractItemView::SelectRows );
    modelo->setFilter( QString( " id_cliente = %1 " ).arg( this->field("id_cliente").toInt() ) );
    modelo->select();
}

int PaginaEquipamientoExistente::nextId() const
{
    return OrdenTrabajoWizard::Pagina_Final;
}

bool PaginaEquipamientoExistente::validatePage()
{
    if( TVEquipamientos->selectionModel()->selectedRows().count() <= 0 ) {
        QMessageBox::information( this, "error", "Seleccione al menos un elemento de la lista" );
        return false;
    }
    QModelIndex indice = TVEquipamientos->selectionModel()->selectedRows().first();
    int id_equipamiento = modelo->data( modelo->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    setField( "equipamiento.id_equipamiento", id_equipamiento );
    return true;
}

