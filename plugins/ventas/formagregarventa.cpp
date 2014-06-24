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
#include "formagregarventa.h"

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
#include "MFactura.h"
#include "mitemfactura.h"
#include "NumeroComprobante.h"
#include "ecbproductos.h"
#include "mproductostotales.h"
#include "dproductostotales.h"
#include "emcliente.h"
#include "mclientes.h"
#include "eddescuento.h"
#include "preferencias.h"
#include "eregistroplugins.h"
#include "ecbproductosmodel.h"
#include "ecbproductosfilter.h"

FormAgregarVenta::FormAgregarVenta ( QWidget* parent, Qt::WFlags fl )
: EVentana ( parent, fl ), Ui::FormAgregarVentaBase()
{
        setupUi( this );
        this->setObjectName( "formagregarcompra" );
        this->setWindowTitle( "Agregar Venta" );
        this->setWindowIcon( QIcon(":/imagenes/factura-nueva.png" ) );

        PBAgregarProducto->setIcon( QIcon( ":/imagenes/add.png" ) );
        PBAgregarProducto->setText( "Agregar Producto" );
        PBEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
        PBEliminar->setText( "Eliminar seleccion" );
        PBEliminarTodo->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
        PBEliminarTodo->setText( "Eliminar todo");

        connect( PBAgregarProducto, SIGNAL( clicked() ), this, SLOT( agregarProducto() ) );

        // Inicio los modelos
        CBCliente->setEditable( true );

        DEFecha->setDate( QDate::currentDate() );

        ecbmprod = new ECBProductosModel( this );
        ecbmprod->inicializar();

        ecbfiltro = new ECBProductosFilter( this );
        ecbfiltro->setSourceModel( ecbmprod );
        ecbfiltro->setearNoMostrarProductosSinStock( true );

        connect( CBProducto, SIGNAL( agregarProducto() ), this, SLOT( agregarProducto() ) );
        CBProducto->setearModelo( ecbfiltro );

        // Modelo del tableview
        mcp = new MProductosTotales( TVProductos, ecbmprod );
        mcp->calcularTotales( true );
        preferencias *p = preferencias::getInstancia();
        p->inicio();
        p->beginGroup( "Preferencias" );
        p->beginGroup( "Ventas" );
        if( p->value( "buscarPrecio", true ).toBool() )
        { mcp->buscarPrecios( true ); }
        if( p->value( "siempre_cf", false ).toBool() )
        { CBCliente->setearId( 0 ); }
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

        DSBCant->setValue( 1.0 );
        DSBCant->setPrefix( "" );

        // Coloco el proximo numero de comprobante
        LNumeroComprobante->setText( LNumeroComprobante->text().append( "       <b>" ).append( MFactura::proximoComprobante().aCadena() ).append( "</b>" ) );

        PBEliminarDescuento->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
        PBAgregarDescuento->setIcon( QIcon( ":/imagenes/add.png" ) );
        PBAgregarDescuento->setText( "Descuento" );
        PBEliminarDescuento->setText( "Descuento" );
        connect( PBAgregarDescuento,  SIGNAL( clicked() ), this, SLOT( agregarDescuento()  ) );
        connect( PBEliminarDescuento, SIGNAL( clicked() ), this, SLOT( eliminarDescuento() ) );

        // Verifico si la venta a cta corriente esta habilitada
        p->beginGroup( "CtaCte" );
        bool ctacte_habilitada = p->value( "habilitada" ).toBool();
        if( !ctacte_habilitada )
        {
                RBContado->setChecked( true );
                RBCtaCte->setEnabled( false );
        }
        p->endGroup();
        p->beginGroup( "Descuentos" );
        bool usar_descuentos = p->value( "usar", false ).toBool();
        p->endGroup();
        p->beginGroup("Ventas");
        if( p->value("filtrarProveedor").toBool() ) {
            connect( CBProveedor, SIGNAL( cambioIdProveedor( int ) ), this, SLOT( cambioProveedor( int ) ) );
        } else {
            CBProveedor->setVisible( false );
        }
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

        if(  !( ERegistroPlugins::getInstancia()->existePluginExterno( "descuentos" ) ) ) {
            PBAgregarDescuento->setVisible( false );
            PBEliminarDescuento->setVisible( false );
        } else if( !usar_descuentos ) {
            PBAgregarDescuento->setEnabled( false );
            PBEliminarDescuento->setEnabled( false );
        }

        if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) ) {
            RBContado->setVisible( true );
        } else {
            RBContado->setVisible( false );
        }
        if(  !ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) &&
             !ERegistroPlugins::getInstancia()->existePluginExterno( "cuotas" ) &&
             !ctacte_habilitada ) {
            GBFormaPago->setVisible( false );
            RBOtro->setChecked( true );
        }

        DEFecha->setMinimumDate( MFactura::fechaUltimaVenta() );

        id_plan_cuota = -1;

        DSBPrecioUnitario->setVisible( false );

        CBProducto->setCurrentIndex( -1 );
}


/*!
    \fn FormAgregarVenta::agregarProducto()
    Slot llamado cuando se presiona el boton en el formulario o se presiona enter en el combobox.
 */
void FormAgregarVenta::agregarProducto()
{
 // Verificación previa
 if( DSBCant->value() == 0 )
 { QMessageBox::information( this, "Error de dato", "La cantidad a agregar debe ser mayor que cero" ); return; }

 if( CBProducto->currentText().isEmpty() )
 { QMessageBox::information( this, "Error de datos", "Por favor, ingrese un producto", QMessageBox::Ok ); return; }

 /*if( DSBPrecioUnitario->value() == 0.0 )
 { QMessageBox::information( this, "Error de datos", "Por favor, ingrese un precio unitario", QMessageBox::Ok ); return; }*/

 CBProducto->verificarExiste();
 mcp->agregarNuevoProducto( DSBCant->value(), CBProducto->idActual(), DSBPrecioUnitario->value() );

 // Reseteo los ingresos de producto
 DSBCant->setValue( 1.0 );
 CBProducto->setCurrentIndex( -1 );
 DSBPrecioUnitario->setValue( 0.0 );

 // Seteo el foco
 DSBCant->setFocus();
}


/*!
    \fn FormAgregarVenta::eliminarProducto()
 */
void FormAgregarVenta::eliminarProducto()
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
                }
        }
 }
 TVProductos->update();
 return;
}

/*!
    \fn FormAgregarVenta::eliminarTodo()
    Elimina todos los productos de la factura
 */
void FormAgregarVenta::eliminarTodo()
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
    \fn FormAgregarVenta::agregarDescuento()
 */
void FormAgregarVenta::agregarDescuento()
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
    \fn FormAgregarVenta::eliminarDescuento()
 */
void FormAgregarVenta::eliminarDescuento()
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
 * Slot llamado cada vez que se cambia el proveedor en el combobox
 */
void FormAgregarVenta::cambioProveedor( int id_proveedor )
{
    if( id_proveedor > 0 ) {
        CBProducto->filtrarPorProveedor( id_proveedor );
    }
}

/*!
 * \fn FormAgregarVenta::guardar
 * \param imprimir
 * \return ID de la venta agregada
 */
int FormAgregarVenta::guardar( bool imprimir )
{
 if( CBCliente->currentIndex() == -1 )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese un cliente para esta venta" );
  return -1;
 }
 if( !DEFecha->date().isValid() && DEFecha->date() <= MFactura::fechaUltimaVenta() )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese una fecha valida para esta venta" );
  return -1;
 }
 if( !( RBContado->isChecked() || RBCuotas->isChecked() || RBCtaCte->isChecked() || RBOtro->isChecked() ) ) {
     QMessageBox::warning( this, "Faltan Datos" , "Por favor, elija una forma de pago para esta venta" );
     return -1;
 }

 mcp->calcularTotales( false );
 if( mcp->rowCount() < 1 )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese una cantidad de productos vendidos distinta de cero para esta venta" );
  mcp->calcularTotales( true );
  return -1;
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
 //seteo el modelo para que no calcule totales y subtotales
 mcp->calcularTotales( false );
 // veo el id del proveedor
 int id_cliente = CBCliente->idClienteActual();
 MFactura::FormaPago id_forma_pago;
 if( RBCtaCte->isChecked() && RBCtaCte->isEnabled() )
 {
   id_forma_pago = MFactura::CuentaCorriente;
   //qDebug( "MFactura::CuentaCorriente" );
 }
 else if( RBContado->isChecked() )
 {
   id_forma_pago = MFactura::Contado;
   //qDebug( "MFactura::Contado" );
 }
 else if( RBCuotas->isChecked() )
 {
     id_forma_pago = MFactura::Cuotas;
     if( id_plan_cuota == -1 ) {
         // Todavía no se pudo hacer el plan de cuotas
         emit emitirPlanCuota( CBCliente->idClienteActual(), mcp->total(), MPlanCuota::Factura );
         return 0;
     } else {
         // Si paso por aquí el plan de cuota fue creado pero todavía no se le asigno el id de factura
         QMessageBox::information( this, "Paso", "Plan de cuota emitido" );
     }
 } else if( RBOtro->isChecked() ){
     id_forma_pago = MFactura::Otro;
 } else {
     QMessageBox::warning( this, "Faltan Datos" , "Por favor, elija una forma de pago para esta venta" );
     mcp->calcularTotales( true );
     return -1;
 }
 //Inicio una transacción
 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
 // Genero la venta
 MFactura *venta = new MFactura();
 int id_venta = venta->agregarVenta( DEFecha->dateTime(), id_cliente, id_forma_pago, mcp, observacion );
 if( id_venta == -1 ) {
    QMessageBox::information( this, "Error", "No se pudo agregar la venta" );
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
    return -1;
 }
 // si el plan de cuota fue utilizado tengo que asociarlo con la factura
 if( id_forma_pago == MFactura::Cuotas ) {
     emit emitirPlanCuotaSetIdFactura( id_plan_cuota, id_venta );
 }
 if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit() ) {
   // Ver si quiere ver la factura o imprimirla
   if( imprimir ) {
       int respuesta = QMessageBox::question( this, "Correcto", "La venta se ha registrado correctamente. Desea imprimir un comprobante de venta?",
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
       switch( respuesta )
       {
        case QMessageBox::Yes:
        {
         ParameterList lista;
         lista.append( "cliente", CBCliente->currentText());
         lista.append( "direccion", LEDireccion->text() );
         lista.append( "id_factura", id_venta );

         EReporte *rep = new EReporte( this );
         rep->factura();
         if( !rep->hacer( lista ) ) {
             QMessageBox::critical( this, "Error", "No se pudo imprimir el reporte. Consulte con el administrador del sistema" );
             return 0;
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
   }
   /// @TODO: Agregar pregunta para garantía!
   this->close();
   emit actualizarListado();
   return id_venta;

  } else {
   QMessageBox::information( this, "Incorrecto" , "La venta no se pudo guardar correctamente" );
   return -1;
  }
}

/*!
    \fn FormAgregarVenta::cambioCliente( int id_combo )
        Slot llamado cada vez que cambia el cliente para verificar si tiene cuenta corriente habilitada o si se puede venderle en cuotas.
        @param id_combo Indice en la lista de combobox que indica el cliente
 */
void FormAgregarVenta::cambioCliente( int /*id_combo*/ )
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
                GBFormaPago->setEnabled( true );
             } else {
                 qDebug( "Cuenta corriente suspendida" );
                 RBContado->setChecked( true );
                 RBCtaCte->setEnabled( false );
             }
         }
         else
         {
           // qDebug( "No se encontro una cuenta corriente para el cliente." );
            RBContado->setChecked( true );
            RBCtaCte->setEnabled( false );
         }
     }
     RBCuotas->setEnabled( true );

 } else {
     //qDebug( "Cliente consumidor final - Sin direccion" );
     RBContado->setChecked( true );
     RBCtaCte->setEnabled( false );
     RBCuotas->setEnabled( false );
 }
 return;
}

/*!
 * \fn FormAgregarVenta::setearCliente( int id_cliente )
 * Setea el numero de cliente
 */
void FormAgregarVenta::setearCliente( int id_cliente ) {
    if( id_cliente < 0 )
        id_cliente = 0;

    this->CBCliente->setearId( id_cliente );
}

/*!
 * \fn FormAgregarVenta::setearFecha( QDate fecha )
 * Setea la fecha para la factura
 */
void FormAgregarVenta::setearFecha( QDate fecha )
{
    if( !fecha.isValid() )
        return;

    this->DEFecha->setDate( fecha );
}

/*!
 * \fn FormAgregarVenta::setearItems( MProductosTotales *m )
 * Setea el listado de items pasado como parametro
 */
void FormAgregarVenta::setearItems( MProductosTotales *m )
{
    m->setParent( this );
    this->mcp = m;
    this->CBProducto->setearListado( this->mcp->listaProductos() );

    mcp->calcularTotales( true );

    this->TVProductos->setModel( this->mcp );
    this->TVProductos->update();
}


void FormAgregarVenta::setearIdPlanCuota(int id_cuota)
{
    if( id_cuota <= 0 ) {
        qWarning( "Numero de plan de cuota pasado como parametro es 0" );
        return;
    }
    this->id_plan_cuota = id_cuota;
    guardar();
}
