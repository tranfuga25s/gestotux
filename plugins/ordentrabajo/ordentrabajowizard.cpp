#include "ordentrabajowizard.h"

#include <QSqlDatabase>
#include <QMessageBox>

#include "wizard/paginacliente.h"
#include "wizard/paginaclientenuevo.h"
#include "wizard/paginaequipamiento.h"
#include "wizard/paginaequipamientonuevo.h"
#include "wizard/paginaequipamientoexistente.h"
#include "wizard/paginafinal.h"

#include "mordentrabajo.h"
#include "mclientes.h"
#include "mequipamiento.h"
#include "mhistorialordentrabajo.h"
#include "mtipooperacionordentrabajo.h"

OrdenTrabajoWizard::OrdenTrabajoWizard( QWidget *parent ) :
QWizard(parent)
{
    setPage( Pagina_Cliente, new PaginaCliente );
    setPage( Pagina_ClienteNuevo, new PaginaClienteNuevo );
    setPage( Pagina_Equipamiento, new PaginaEquipamiento );
    setPage( Pagina_EquipamientoNuevo, new PaginaEquipamientoNuevo );
    setPage( Pagina_EquipamientoExistente, new PaginaEquipamientoExistente );
    setPage( Pagina_Final, new PaginaFinal );

    setStartId( Pagina_Cliente );

    setOption( HaveHelpButton, false );
    /// @TODO: Agregar logotipo para tutorial de orden de trabajo.
    //setPixmap( QWizard::LogoPixmap, QPixmap( ":/images/logo.png" ) );

    setWindowTitle( "Agregar orden trabajo" );
}

void OrdenTrabajoWizard::done( int result )
{
    // Utilizando los datos indicados por el usuario y genero la orden de trabajo.
    if( result == QDialog::Accepted ) {
        QSqlDatabase::database().transaction();

        int id_cliente = -1;
        if( visitedPages().contains( Pagina_Cliente ) ) {
            id_cliente = field( "cliente.id_cliente" ).toInt();
        } else {
            // Guardo los datos del cliente
            id_cliente = MClientes::agregarClientePredeterminado( field( "cliente.razonsocial" ).toString(),
                                                                  field( "cliente.nombre"      ).toString(),
                                                                  field( "cliente.apellido"    ).toString(),
                                                                  field( "cliente.direccion"   ).toString(),
                                                                  field( "cliente.telefono"    ).toString(),
                                                                  field( "cliente.inscripcion" ).toInt() );
            if( id_cliente == -1 ) {
                QMessageBox::warning( this, "Error", "No se pudo agregar el cliente predeterminado" );
                QSqlDatabase::database().rollback();
                return;
            }
        }

        int id_equipamiento = -1;
        if( visitedPages().contains( Pagina_EquipamientoExistente ) ) {
            id_equipamiento = field( "equipamiento.id_equipamiento" ).toInt();
        } else {
            id_equipamiento = MEquipamiento::agregarEquipamiento( id_cliente,
                                                                  field( "equipamiento.descripcion"   ).toString(),
                                                                  field( "equipamiento.marca"         ).toString(),
                                                                  field( "equipamiento.modelo"        ).toString(),
                                                                  field( "equipamiento.numero_serie"  ).toString(),
                                                                  field( "equipamiento.observaciones" ).toString() );
            if( id_equipamiento == -1 ) {
                QMessageBox::warning( this, "Error", "No se pudo agregar el equipamiento" );
                QSqlDatabase::database().rollback();
                return;
            }
        }

        MOrdenTrabajo *mot = new MOrdenTrabajo();
        int id_orden_trabajo = mot->agregarOrdenTrabajo( id_cliente,
                                                         id_equipamiento,
                                                         field( "orden_trabajo.id_tecnico"    ).toInt()     ,
                                                         field( "orden_trabajo.requerente"    ).toString()  ,
                                                         field( "orden_trabajo.ingresante"    ).toString()  ,
                                                         field( "orden_trabajo.fecha_ingreso" ).toDateTime(),
                                                         field( "orden_trabajo.fecha_entrega" ).toDateTime(),
                                                         field( "orden_trabajo.fecha_vencimiento" ).toDateTime(),
                                                         field( "orden_trabajo.causa_ingreso" ).toString()  );
        if( id_orden_trabajo == -1 ) {
            QMessageBox::critical( this, "Error", "No se pudo guardar la orden de trabajo" );
            QSqlDatabase::database().rollback();
            return;
        }
        delete mot;

        MHistorialOrdenTrabajo *mhot = new MHistorialOrdenTrabajo();
        // Genero el primer elemento de ingreso.
        if( !mhot->agregarHistorial( id_orden_trabajo,
                                     QDateTime::currentDateTime(),
                                     field( "orden_trabajo.causa_ingreso" ).toString(),
                                     0.0,
                                     MTipoOperacionOrdenTrabajo::IngresoAlTaller,
                                     field( "orden_trabajo.id_tecnico"    ).toInt() ) ) {
            QMessageBox::critical( this, "Error", "No se pudo guardar el primer historial de la orden de trabajo" );
            QSqlDatabase::database().rollback();
            return;
        }
        delete mhot;

        if( QSqlDatabase::database().commit() != true ) {
            QMessageBox::information( this, "Error", "No se pudo hacer el commit de la base de datos" );
            QSqlDatabase::database().rollback();
            return;
        }
        emit actualizarModeloClientes();
        emit actualizarModeloEquipamientos();
        emit actualizarModeloOrdenTrabajo();

        // Pregunto si quiere imprimir la orden de trabajo
        if( QMessageBox::question( this,
                                   QString::fromUtf8( "Imprimir" ),
                                   QString::fromUtf8( "Desea imprimir la etiqueta de identificación para el nuevo equipamiento?" ),
                                   QMessageBox::Yes, QMessageBox::No )
            == QMessageBox::Yes ) {
            QMessageBox::information( this, "Error", "Todavia no implementado" );
        }
        // Pregunto si quiere imprimir la etiqueta para el equipamiento
        if( field( "equipamiento.nuevo" ).toBool() ) {
            if( QMessageBox::question( this,
                                       QString::fromUtf8( "Imprimir" ),
                                       QString::fromUtf8( "Desea imprimir la etiqueta de identificación para el nuevo equipamiento?" ),
                                       QMessageBox::Yes, QMessageBox::No )
                == QMessageBox::Yes ) {
                QMessageBox::information( this, "Error", "Todavia no implementado" );
            }
        }
    }
    QDialog::done( result );
}


/*!
 *
 * Registros ocupados:
 *
 * Pagina_cliente
 * cliente.id_cliente = Identificador del cliente
 * cliente.texto_ingresado
 *
 * Pagina_ClienteNuevo
 * cliente.nombre      = Nombre del nuevo cliente
 * cliente.razonsocial = Razon social del nuevo cliente
 * cliente.apellido    = apellido del nuevo cliente
 * cliente.direccion   = direccion del nuevo cliente
 * cliente.inscripcion = idnetificador del id de estado fiscal
 * cliente.telefono    = telefono del nuevo cliente
 *
 * Pagina_Equipamiento
 * equipamiento.nuevo     = Se declara un nuevo equipamiento.
 * equipamiento.existente = se utiliza un equipamiento ya existente.
 *
 * Pagina_EquipamientoNuevo
 * equipamiento.descripcion   = descripcion del nuevo equipamiento
 * equipamiento.modelo        = modelo del nuevo equipamiento
 * equipamiento.marca         = marca del nuevo equipamiento
 * equipamiento.observaciones = observaciones del nuevo equipamiento
 * equipamiento.numeroserie   = numero de serie del nuevo equipamiento
 *
 * Pagina_EquipamientoExistente
 * equipamiento.id_equipamiento  = identificador del equipamiento
 *
 * Pagina_Final
 * orden_trabajo.requerente    = Persona que ingresa el pedido si no es el cliente.
 * orden_trabajo.ingresante    = Ingresante de la orden ( empleado )
 * orden_trabajo.fecha_ingreso = Fecha de ingreso de la orden
 * orden_trabajo.fecha_entrega = Fecha posible de devolucion de la orden
 * orden_trabajo.causa_ingreso = Causa o falla del ingreso
 * orden_trabajo.id_tecnico    = Tecnico asignado
 */