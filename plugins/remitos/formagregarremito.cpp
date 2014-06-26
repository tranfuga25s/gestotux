/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "formagregarremito.h"

#include <QTableView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCompleter>

#include "eactcerrar.h"
#include "eactguardar.h"
#include "eregistroplugins.h"
#include "../CtaCte/mcuentacorriente.h"
#include "../CtaCte/mitemcuentacorriente.h"
#include "../productos/mproductos.h"
#include "../cuotas/mplancuota.h"
#include "preferencias.h"
#include "EReporte.h"
#include "MRemito.h"
#include "mitemremito.h"
#include "NumeroComprobante.h"
#include "ecbproductos.h"
#include "mproductostotales.h"
#include "dproductostotales.h"
#include "emcliente.h"
#include "mclientes.h"
#include "eddescuento.h"
#include "preferencias.h"
#include "eregistroplugins.h"

FormAgregarRemito::FormAgregarRemito ( QWidget* parent, Qt::WFlags fl )
: EVentana ( parent, fl ), Ui::FormAgregarRemitoBase()
{
        setupUi( this );
        this->setObjectName( "formagregarremito" );
        this->setWindowTitle( "Agregar remito" );
        this->setWindowIcon( QIcon(":/imagenes/remito-nuevo.png" ) );

        PBAgregarProducto->setIcon( QIcon( ":/imagenes/add.png" ) );
        PBAgregarProducto->setText( "Agregar Producto" );
        PBEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
        PBEliminar->setText( "Eliminar seleccion" );
        PBEliminarTodo->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
        PBEliminarTodo->setText( "Eliminar todo");

        connect( PBAgregarProducto, SIGNAL( clicked() ), this, SLOT( agregarProducto() ) );

        // Inicio los modelos
        CBCliente->setEditable( true );

        connect( CBProducto, SIGNAL( agregarProducto() ), this, SLOT( agregarProducto() ) );

        DEFecha->setDate( QDate::currentDate() );

        // Modelo del tableview
        mcp = new MProductosTotales( TVProductos/*, CBProducto->listadoProductos()*/ );
        mcp->calcularTotales( true );
        preferencias *p = preferencias::getInstancia();
        p->inicio();
        p->beginGroup( "Preferencias" );
        p->beginGroup( "Remito" );
        if( p->value( "buscarPrecio", true ).toBool() )
        { mcp->buscarPrecios( true ); }
        p->endGroup();

        TVProductos->setModel( mcp );
        TVProductos->setItemDelegate( new DProductosTotales( TVProductos ) );
        TVProductos->setAlternatingRowColors( true );
        TVProductos->setSelectionBehavior( QAbstractItemView::SelectRows );
        TVProductos->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
        TVProductos->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
        TVProductos->horizontalHeader()->setMinimumSectionSize( 140 );
        TVProductos->setSortingEnabled( false );


        this->addAction( new EActGuardar( this ) );
        this->addAction( new EActCerrar( this ) );

        connect( CBCliente, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioCliente( int ) ) );

        connect( PBEliminar, SIGNAL( clicked() ), this, SLOT( eliminarProducto() ) );
        connect( PBEliminarTodo, SIGNAL( clicked() ), this, SLOT( eliminarTodo() ) );

        // Coloco el proximo numero de comprobante
        LNumeroComprobante->setText( MRemito::proximoComprobante().aCadena() );

        PBEliminarDescuento->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
        PBAgregarDescuento->setIcon( QIcon( ":/imagenes/add.png" ) );
        PBAgregarDescuento->setText( "Descuento" );
        PBEliminarDescuento->setText( "Descuento" );
        connect( PBAgregarDescuento,  SIGNAL( clicked() ), this, SLOT( agregarDescuento()  ) );
        connect( PBEliminarDescuento, SIGNAL( clicked() ), this, SLOT( eliminarDescuento() ) );

        // Verifico si la venta a cta corriente esta habilitada
        p->beginGroup( "CtaCte" );
        if( !p->value( "habilitada" ).toBool() )
        {
                RBContado->setChecked( true );
                RBCtaCte->setEnabled( false );
        }
        p->endGroup();
        p->beginGroup( "Descuentos" );
        bool usar = p->value( "usar", false ).toBool();
        p->endGroup();
        p->beginGroup( "Productos" );
        p->beginGroup( "Stock" );
        int cantidad_decimales = 0;
        if( p->value("mostrar-decimales", false ).toBool() ) {
            cantidad_decimales = p->value("cantidad-decimales", 4 ).toInt();
        }
        p->endGroup();
        p->endGroup();
        p->endGroup();
        p=0;

        DSBCant->setDecimals( cantidad_decimales );
        DSBCant->setValue( 1.0 );
        DSBCant->setPrefix( "" );

        if(  !( ERegistroPlugins::getInstancia()->existePluginExterno( "descuentos" ) ) ) {
            PBAgregarDescuento->setVisible( false );
            PBEliminarDescuento->setVisible( false );
        } else if( !usar ) {
            PBAgregarDescuento->setEnabled( false );
            PBEliminarDescuento->setEnabled( false );
        }

        if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) ) {
            RBContado->setVisible( true );
        } else {
            RBContado->setVisible( false );
        }

        DEFecha->setMinimumDate( MRemito::fechaUltimoRemito() );

        id_plan_cuota = -1;
}


/*!
    \fn FormAgregarRemito::agregarProducto()
    Slot llamado cuando se presiona el boton en el formulario o se presiona enter en el combobox.
 */
void FormAgregarRemito::agregarProducto()
{
 // Verificación previa
 if( DSBCant->value() == 0 )
 { QMessageBox::information( this, "Error de dato", "La cantidad a agregar debe ser mayor que cero" ); return; }

 if( CBProducto->currentText().isEmpty() )
 { QMessageBox::information( this, "Error de datos", "Por favor, ingrese un producto", QMessageBox::Ok ); return; }

 CBProducto->verificarExiste();
 mcp->agregarNuevoProducto( DSBCant->value(), CBProducto->idActual() );

 // Reseteo los ingresos de producto
 DSBCant->setValue( 1.0 );
 CBProducto->setCurrentIndex( -1 );
 // Seteo el foco
 DSBCant->setFocus();
}


/*!
    \fn FormAgregarRemito::eliminarProducto()
 */
void FormAgregarRemito::eliminarProducto()
{
 //Preguntar al usuario si esta seguro
 QItemSelectionModel *selectionModel = TVProductos->selectionModel();
 QModelIndexList indices = selectionModel->selectedIndexes();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para eliminar",
                   QMessageBox::Ok );
   return;
 }
 // cuento las filas distintas
 QModelIndex indice;
 int ultimo = -1;
 foreach( indice, indices )
 {
  if( indice.row() == ultimo )
  {
   indices.removeOne( indice );
  }
  else
  {
   ultimo = indice.row();
  }
 }
 //Hacer dialogo de confirmacion..
 int ret = QMessageBox::warning( this, "Esta seguro?",
                   QString( "Esta seguro de eliminar %1 item?").arg( indices.size() ),
                   "Si", "No" );
 if ( ret == 0 )
 {
        QModelIndex indice;
        foreach( indice, indices )
        {
                if( indice.isValid() )
                {
                    mcp->removeRow( indice.row() );
                } /*else {
                    qDebug( " Indice no valido! - " );
                }*/
        }
 }
 TVProductos->update();
 return;
}

/*!
    \fn FormAgregarRemito::eliminarTodo()
    Elimina todos los productos del remito
 */
void FormAgregarRemito::eliminarTodo()
{
 //Preguntar al usuario si esta seguro
 //Hacer dialogo de confirmacion..
 int ret = QMessageBox::warning( this, "Esta seguro?",
                   QString( "Esta seguro de eliminar todos los items?"),
                   "Si", "No" );
 if ( ret == 0 )
 {
     int fin = mcp->rowCount() - 1;
     for( int i = 0; i <= fin; i++ ) { mcp->removeRow( i ); }
 }
 TVProductos->update();
 return;
}

/*!
    \fn FormAgregarRemito::agregarDescuento()
 */
void FormAgregarRemito::agregarDescuento()
{
    // Cargo el dialogo y conecto la señal
    if( mcp->conteoItems() <= 0 ) {
        QMessageBox::warning( this, "Error", "Por favor, inserte al menos un producto o item para poder agregar un descuento" );
        return;
    }
    EDDescuento *d = new EDDescuento( this );
    connect( d, SIGNAL( agregarDescuento( QString, double ) ), mcp, SLOT( agregarDescuento( QString, double ) ) );
    d->exec();
}

/*!
    \fn FormAgregarRemito::eliminarDescuento()
 */
void FormAgregarRemito::eliminarDescuento()
{
    if( TVProductos->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::information( this, "Error", "El item seleccionado no es un descuento" );
        return;
    }
    QModelIndex mi = TVProductos->selectionModel()->selectedRows().first();
    if( mcp->esDescuento( mi ) ) {
        mcp->eliminarDescuento( mi );
    } else {
        QMessageBox::information( this, "No descuento", "El item elegido no es un descuento" );
        return;
    }
}

/*!
    \fn FormAgregarRemito::guardar()
 */
void FormAgregarRemito::guardar()
{
 if( CBCliente->currentIndex() == -1 )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese un cliente para este remito" );
  return;
 }
 if( !DEFecha->date().isValid() && DEFecha->date() <= MRemito::fechaUltimoRemito() )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese una fecha valida para este remito" );
  return;
 }
 if( !( RBContado->isChecked() || RBCuotas->isChecked() || RBCtaCte->isChecked() || RBOtro->isChecked() ) ) {
     QMessageBox::warning( this, "Faltan Datos" , "Por favor, elija una forma de pago para este remito" );
     return;
 }

 mcp->calcularTotales( false );
 if( mcp->rowCount() < 1 )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese una cantidad de productos vendidos distinta de cero para este remito" );
  mcp->calcularTotales( true );
  return;
 }
 // Verifico que no se le venda a cuenta corriente a un cliente consumidor final
 if( RBCtaCte->isChecked() && CBCliente->currentIndex() <=  0 ) {
     QMessageBox::warning( this, "Error", "No se puede vender a consumidor final en cuenta corriente. Se eligirá pago a contado" );
     RBCtaCte->setChecked( false );
     RBContado->setChecked( true );
 }
 // Remuevo el texto que dice "Observaciones:"
 QString observacion = PTEObservaciones->toPlainText();
 observacion.remove( "Observaciones: " );
 //Inicio una transacción
 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
 //seteo el modelo para que no calcule totales y subtotales
 mcp->calcularTotales( false );
 // veo el id del proveedor
 int id_cliente = CBCliente->idClienteActual();
 MRemito::FormaPago id_forma_pago;
 if( RBCtaCte->isChecked() && RBCtaCte->isEnabled() )
 {
   id_forma_pago = MRemito::CuentaCorriente;
 }
 else if( RBContado->isChecked() )
 {
   id_forma_pago = MRemito::Contado;
 }
 else if( RBCuotas->isChecked() )
 {
     id_forma_pago = MRemito::Cuotas;
     if( id_plan_cuota == -1 ) {
         // Todavía no se pudo hacer el plan de cuotas
         emit emitirPlanCuota( CBCliente->idClienteActual(), mcp->total(), MPlanCuota::Remito );
         return;
     } else {
         // Si paso por aquí el plan de cuota fue creado pero todavía no se le asigno el id de factura
         QMessageBox::information( this, "Paso", "Plan de cuota emitido" );
     }
 } else if( RBOtro->isChecked() ){
     id_forma_pago = MRemito::Otro;
 } else {
     QMessageBox::warning( this, "Faltan Datos" , "Por favor, elija una forma de pago para esta remito" );
     mcp->calcularTotales( true );
     return;
 }
 // Genero el remito
 MRemito *remito = new MRemito();
 int id_remito = remito->agregarVenta( DEFecha->dateTime(), id_cliente, id_forma_pago, mcp, observacion );
 if( id_remito == -1 ) {
    QMessageBox::information( this, "Error", "No se pudo agregar el remito" );
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
    return;
 }
 // si el plan de cuota fue utilizado tengo que asociarlo con la factura
 if( id_forma_pago == MRemito::Cuotas ) {
     emit emitirPlanCuotaSetIdFactura( id_plan_cuota, id_remito );
 }
 if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit() ) {
   // Ver si quiere ver la factura o imprimirla
   int respuesta = QMessageBox::question( this, "Correcto", "El remito se ha registrado correctamente. Desea imprimir un comprobante?",
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
   switch( respuesta )
   {
    case QMessageBox::Yes:
    {
     ParameterList lista;
     lista.append( "cliente", CBCliente->currentText());
     lista.append( "direccion", LEDireccion->text() );
     lista.append( "id_remito", id_remito );

     EReporte *rep = new EReporte( this );
     rep->remito();
     if( !rep->hacer( lista ) ) {
         QMessageBox::critical( this, "Error", "No se pudo imprimir el reporte. Consulte con el administrador del sistema" );
         return;
     }
     break;
    }
    case QMessageBox::No:
    case QMessageBox::Cancel:
    {
           //qDebug( "Respondio no o cancelar" );
           break;
    }
   }
   /// @TODO: Agregar pregunta para garantía!
   this->close();
   return;

  } else {
   QMessageBox::information( this, "Incorrecto" , "El remito no se pudo guardar correctamente." );
   return;
  }
}

/*!
    \fn FormAgregarRemito::cambioCliente( int id_combo )
        Slot llamado cada vez que cambia el cliente para verificar si tiene cuenta corriente habilitada
        @param id_combo Indice en la lista de combobox que indica el cliente
 */
void FormAgregarRemito::cambioCliente( int /*id_combo*/ )
{
 // Busco la dirección
 int id_cliente = CBCliente->idClienteActual();
 if( id_cliente > 0 ) { // 0 => Consumidor final.
     LEDireccion->setText( MClientes::direccionEntera( id_cliente ) );
     // Veo si esta habilitado el cliente con cueta corriente y el plugin esta cargado
     if( ERegistroPlugins::getInstancia()->existePluginExterno( "ctacte" ) )
     {
         if( MCuentaCorriente::existeCuentaCliente( id_cliente ) )
         {
             if( !MCuentaCorriente::suspendida( id_cliente ) ) {
                RBCtaCte->setEnabled( true );
                RBCuotas->setEnabled( true );
                GBFormaPago->setEnabled( true );
             } else {
                 qDebug( "Cuenta corriente suspendida" );
                 RBContado->setChecked( true );
                 RBCtaCte->setEnabled( false );
                 RBCuotas->setEnabled( false );
             }
         }
         else
         {
           // qDebug( "No se encontro una cuenta corriente para el cliente." );
            RBContado->setChecked( true );
            RBCtaCte->setEnabled( false );
            RBCuotas->setEnabled( false );
         }
     }

 } else {
     //qDebug( "Cliente consumidor final - Sin direccion" );
     RBContado->setChecked( true );
     RBCtaCte->setEnabled( false );
     RBCuotas->setEnabled( false );
 }
 return;
}

/*!
 * \fn FormAgregarRemito::setearCliente( int id_cliente )
 * Setea el numero de cliente
 */
void FormAgregarRemito::setearCliente( int id_cliente ) {
    if( id_cliente < 0 )
        id_cliente = 0;

    this->CBCliente->setearId( id_cliente );
}

/*!
 * \fn FormAgregarRemito::setearFecha( QDate fecha )
 * Setea la fecha para el remito
 */
void FormAgregarRemito::setearFecha( QDate fecha )
{
    if( !fecha.isValid() )
        return;

    this->DEFecha->setDate( fecha );
}

/*!
 * \fn FormAgregarremito::setearItems( MProductosTotales *m )
 * Setea el listado de items pasado como parametro
 */
void FormAgregarRemito::setearItems( MProductosTotales *m )
{
    m->setParent( this );
    this->mcp = m;
    //this->CBProducto->setearListado( this->mcp->listaProductos() );

    mcp->calcularTotales( true );

    this->TVProductos->setModel( this->mcp );
    this->TVProductos->update();
}

void FormAgregarRemito::setearIdPlanCuota(int id_cuota)
{
    if( id_cuota <= 0 ) {
        qWarning( "Numero de plan de cuota pasado como parametro es 0" );
        return;
    }
    this->id_plan_cuota = id_cuota;
    guardar();
}
