#include "vtipooperacionordentrabajo.h"

#include <QTableView>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>

#include "eactcerrar.h"

VTipoOperacionOrdenTrabajo::VTipoOperacionOrdenTrabajo( QWidget *parent ) :
EVLista( parent )
{
    this->setObjectName("VistaVTipoOperacionOrdenTrabajo");
    this->setWindowTitle( "Tipo de operacion de orden de trabajo" );
    //this->setWindowIcon( QIcon( ":/imagenes/tecnico.png" ) );

    modelo = new MTipoOperacionOrdenTrabajo( this->vista );
    this->vista->setModel( this->modelo );

    this->vista->hideColumn( 0 ); // Oculta el ID del modelo
    this->vista->setAlternatingRowColors( true );
    this->vista->setSortingEnabled( true );

    ActAgregar->setStatusTip( QString::fromUtf8( "Agregar nuevo tipo de operacion de orden de trabajo" ) );
    ActModificar->setStatusTip( QString::fromUtf8( "Modificar tipo de operacion de orden de trabajo" ) );

    this->addAction( ActAgregar );
    this->addAction( ActEliminar );
    this->addAction( new EActCerrar( this ) );

    this->modelo->select();
}

/*!
 * \fn VTipoOperacionOrdenTrabajo::agregar( bool )
 * Agrega un nuevo tecnico a la lista
 */
void VTipoOperacionOrdenTrabajo::agregar( bool )
{
 // Agrego un nuevo tipo de orden
    bool ok = false;
    QString nombre = QInputDialog::getText( this,  QString::fromUtf8( "Agregar tipo" ),  QString::fromUtf8( "Nueva tipo de orden:" ), QLineEdit::Normal, QString(), &ok );
    if( ok && !nombre.isEmpty() ) {
        // Verifico que no exista ya el nombre
        if( MTipoOperacionOrdenTrabajo::existe( nombre ) ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "El tipo de orden de trabajo que está intentando ingresar ya se encuentra en la base de datos" ) );
        } else {
            if( this->modelo->agregarTipo( nombre ) ) {
                QMessageBox::information( this, "Correcto",  QString::fromUtf8( "El tipo de orden de trabajo se agregó correctamente" ) );
            } else {
                QMessageBox::warning( this, "Incorrecto",  QString::fromUtf8( "No se pudo agregar el nuevo tipo de orden de trabajo" ) );
            }
        }
    }
}

/*!
 * \fn VTipoOperacionOrdenTrabajo::modificar()
 * Modifica el técnico que esté seleccionado
 **/
void VTipoOperacionOrdenTrabajo::modificar()
{
    if( this->vista->selectionModel()->selectedRows().size() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "Seleccióne al menos un tipo para modificarlo" ) );
        return;
    }
    // Busco solo el primero
    QModelIndex idx = this->vista->selectionModel()->selectedRows().first();
    QString nombre_anterior = idx.model()->data( idx, Qt::EditRole ).toString();
    bool ok = false;
    QString nombre = QInputDialog::getText( this,  QString::fromUtf8( "Modificar tipo de orden de trabajo" ),  QString::fromUtf8( "Nuevo nombre:" ), QLineEdit::Normal, nombre_anterior, &ok );
    if( ok && nombre_anterior != nombre && !nombre.isEmpty() ) {
        if( MTipoOperacionOrdenTrabajo::existe( nombre ) ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "El tipo de orden de trabajo que está intentando ingresar ya se encuentra en la base de datos" ) );
        } else {
            if( this->modelo->modificar( idx.row(), nombre ) ) {
                QMessageBox::information( this, "Correcto", QString::fromUtf8( "El tipo de orden de trabajo fue modificado correctamente" ) );
            } else {
                QMessageBox::warning( this, "Error", QString::fromUtf8( "No se pudo modificar el tipo de orden de trabajo" ) );
            }
        }
    }
    return;
}

/*!
 * \fn VTipoOperacionOrdenTrabajo::eliminar()
 * Verifica si se puede eliminar un tecnico
 */
void VTipoOperacionOrdenTrabajo::eliminar()
{
    if( this->vista->selectionModel()->selectedRows().size() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "Seleccióne al menos un tipo de orden para borrar" ) );
        return;
    }
    QModelIndexList indices = this->vista->selectionModel()->selectedRows();
    // Obtener la confirmación
    int ret = QMessageBox::question( this, QString::fromUtf8( "¿Está seguro?" ), QString::fromUtf8( "¿Está seguro que desea eliminar %1 tipo de orden%2?" ).arg( indices.size() ).arg( (indices.size() > 1) ? "es":"" ) );
    if( ret == QMessageBox::Ok ) {
        foreach( QModelIndex idx, indices ) {
            // Busco cada registro y verifico que no existan relaciones
            int id_tipo = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();
            if( !this->modelo->tieneDatosRelacionados( id_tipo ) ) {
                this->modelo->removeRow( idx.row() );
            }
        }
    }
    QMessageBox::information( this, "Listo", "Los items que no fueron eliminados poseen alguna orden de trabajo relacionada." );
}
