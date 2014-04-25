#include "vequipamiento.h"

#include "mvequipamiento.h"
#include "formequipamiento.h"

#include <QTableView>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>

VEquipamiento::VEquipamiento( QWidget *parent ) :
EVLista( parent )
{
    this->setObjectName( "VisorEquipamiento" );
    this->setWindowTitle( "Equipamientos" );
    this->setWindowIcon( QIcon( ":/imagenes/equipamiento.png" ) );

    this->modelo = new MVEquipamiento( this );
    this->vista->setModel( this->modelo );
    this->vista->setSelectionBehavior( QAbstractItemView::SelectRows );
    this->vista->setAlternatingRowColors( true );
    this->vista->hideColumn( modelo->fieldIndex( "id_equipamiento" ) );
    this->vista->hideColumn( modelo->fieldIndex( "id_cliente"      ) );
    this->vista->hideColumn( modelo->fieldIndex( "fecha_baja"      ) );
    this->vista->setSortingEnabled( true );
    this->modelo->select();

    ActDarBaja = new QAction( this );
    ActDarBaja->setText( "Baja" );
    ActDarBaja->setStatusTip( "Da de baja el equipamiento seleccionado" );
    ActDarBaja->setIcon( QIcon( ":/imagenes/baja_equipamiento.png" ) );
    connect( ActDarBaja, SIGNAL( triggered() ), this, SLOT( darBaja() ) );

    ActDarAlta = new QAction( this );
    ActDarAlta->setText( "Alta" );
    ActDarAlta->setIcon( QIcon( ":/imagenes/equipamiento_agregar.png" ) );
    ActDarAlta->setStatusTip( "Da de alta nuevamente el equipamiento seleccionado" );
    connect( ActDarAlta, SIGNAL( triggered() ), this, SLOT( darAlta() ) );

    ActDarBaja->setIcon( QIcon( ":/imagenes/baja_equipamiento.png" ) );
    ActAgregar->setIcon( QIcon( ":/imagenes/equipamiento_agregar.png" ) );
    ActModificar->setIcon( QIcon( ":/imagenes/equipamiento_editar.png" ) );

    addAction( ActAgregar   );
    addAction( ActModificar );
    addAction( ActDarBaja   );
    addAction( ActEliminar  );
    addAction( ActCerrar    );
}

/*!
 * \brief VEquipamiento::agregar
 * Slot llamado para agregar un nuevo equipamiento al sistema
 */
void VEquipamiento::agregar( bool )
{
    FormEquipamiento *f = new FormEquipamiento();
    //f->setearModeloEquipamiento( qobject_cast<MEquipamiento *>( this->modelo ) );
    f->setearAgregar( true );
    emit agregarVentana( f );
}

/*!
 * \brief VEquipamiento::modificar
 * Slot llamado para abrir la ventana de modificación de un equipamiento
 */
void VEquipamiento::modificar()
{
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() <= 0 ) {
        QMessageBox::information( this, "Error", QString::fromUtf8( "Por favor, seleccione algún equipamiento para modificar" ) );
        return;
    }
    foreach( QModelIndex indice, lista ) {
        FormEquipamiento *f = new FormEquipamiento();
        //f->setearModeloEquipamiento( qobject_cast<MEquipamiento *>( this->modelo ) );
        f->setearAgregar( false );
        f->setearIndice( indice );
        emit agregarVentana( f );
    }
}

/*!
 * \brief VEquipamiento::eliminar
 * Slot llamado para eliminar un equipamiento
 */
void VEquipamiento::eliminar()
{
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() <= 0 ) {
        QMessageBox::information( this, "Error", QString::fromUtf8( "Por favor, seleccione algún equipamiento para eliminar" ) );
        return;
    }
    if( QMessageBox::question( this, "¿Está seguro?", "Está seguro que desea eliminar estos equpamientos? \n Se eliminarán todos los registros relacionados con estos datos, exceptuando las facturas y garantías" ) != QMessageBox::Ok ) {
        return;
    }
    MEquipamiento *mequipamiento = new MEquipamiento();
    foreach( QModelIndex indice, lista ) {
        int id_equipamiento = modelo->data( modelo->index( indice.row(), 0 ), Qt::DisplayRole ).toInt();
        if( !mequipamiento->eliminarConRelacionados( id_equipamiento ) ) {
            QMessageBox::information( this, "Error", "No se pudo eliminar el equipamiento " + id_equipamiento );
        }
    }
    delete mequipamiento;
    modelo->select();
}

/*!
 * \brief VEquipamiento::darBaja
 * Slot llamado para dar de baja un equipamiento
 */
void VEquipamiento::darBaja()
{
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() <= 0 ) {
        QMessageBox::information( this, "Error", QString::fromUtf8( "Por favor, seleccione algún equipamiento para dar de baja" ) );
        return;
    }
    if( QMessageBox::question( this, "¿Está seguro?", "Está seguro que desea dar de baja estos equipamientos?" ) != QMessageBox::Ok ) {
        return;
    }
    MEquipamiento *mequipamiento = new MEquipamiento();
    foreach( QModelIndex indice, lista ) {
        bool ok = false;
        QString razon = QInputDialog::getText( this, QString::fromUtf8( "Razón" ), QString::fromUtf8( "Razón:" ), QLineEdit::Normal, QString(), &ok );
        if( ok ) {
            if( !razon.isEmpty() ) {
                razon.append( QString::fromUtf8( "Razón desconocida" ) );
            }
            int id_equipamiento = modelo->data( modelo->index( indice.row(), 0 ), Qt::DisplayRole ).toInt();
            if( mequipamiento->darDeBaja( id_equipamiento, razon ) ) {
                QMessageBox::information( this, "Correcto", "El equipamiento se pudo dar de baja correctamente" );
            } else {
                QMessageBox::information( this, "Incorrecto", "El equipamiento no se pudo dar de baja" );
            }
        }
    }
    delete mequipamiento;
    modelo->select();
}

void VEquipamiento::darAlta()
{
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.size() <= 0 ) {
        QMessageBox::information( this, "Error", QString::fromUtf8( "Por favor, seleccione algún equipamiento para dar de baja" ) );
        return;
    }
    if( QMessageBox::question( this, "¿Está seguro?", "Está seguro que desea dar de alta estos equipamientos?" ) != QMessageBox::Ok ) {
        return;
    }
    MEquipamiento *mequipamiento = new MEquipamiento();
    foreach( QModelIndex indice, lista ) {
        int id_equipamiento = modelo->data( modelo->index( indice.row(), 0 ), Qt::DisplayRole ).toInt();
        if( mequipamiento->darReAlta( id_equipamiento ) ) {
            QMessageBox::information( this, "Correcto", "El equipamiento se pudo dar de baja correctamente" );
        } else {
            QMessageBox::information( this, "Incorrecto", "El equipamiento no se pudo dar de baja" );
        }
    }
    delete mequipamiento;
    modelo->select();
}

/*!
 * \brief VEquipamiento::menuContextual
 * \param indice Indice del item donde se llamó al menú
 * \param menu Menú a utilizar
 */
void VEquipamiento::menuContextual(const QModelIndex &indice, QMenu *menu)
{
    menu->addAction( ActAgregar );
    menu->addAction( ActModificar );
    if( modelo->data( modelo->index( indice.row(), modelo->fieldIndex( "fecha_baja" ) ), Qt::UserRole ).isNull() ) {
        menu->addAction( ActDarBaja );
    } else {
        menu->addAction( ActDarAlta );
    }
    menu->addAction( ActEliminar );
}
