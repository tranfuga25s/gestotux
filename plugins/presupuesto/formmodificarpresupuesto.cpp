#include "formmodificarpresupuesto.h"

#include "mpresupuesto.h"
#include "mproductostotales.h"
#include "dproductostotales.h"
#include "MItemPresupuesto.h"
#include "mclientes.h"
#include "mvpresupuestos.h"
#include "NumeroComprobante.h"
#include "mdescuentos.h"

#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

FormModificarPresupuesto::FormModificarPresupuesto(QWidget *parent) :
    EVentana(parent), Ui::FormPresupuestoBase()
{

    setupUi(this);
    this->setAttribute( Qt::WA_DeleteOnClose );
    this->setObjectName( "FormModificarPresupuesto" );
    this->setWindowTitle( "Modificar Presupuesto" );
    this->setWindowIcon( QIcon( ":/imagenes/presupuesto-nuevo.png" ) );

    QAction *ActGuardar = new QAction( "Guardar", this );
    ActGuardar->setIcon( QIcon( ":/imagenes/guardar.png" ) );
    ActGuardar->setStatusTip( "Guarda el formulario de presupuesto actual y lo muestra ( Ctrl + g )" );
    ActGuardar->setShortcut( QKeySequence( "Ctrl+g" ) );
    connect( ActGuardar, SIGNAL( triggered() ), this, SLOT( guardar() ) );

    QAction *ActCancelar = new QAction( "Cancelar", this );
    ActCancelar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
    ActCancelar->setStatusTip( "Cancela los cambios realizados y cierra la ventana" );
    ActCancelar->setShortcut( QKeySequence( "Ctrl+c" ) );
    connect( ActCancelar, SIGNAL( triggered() ), this, SLOT( cancelar() ) );

     // Agrego las acciones
    addAction( ActGuardar );
    addAction( ActCancelar );

    // Permito la creacion para un no cliente
    CBCliente->setAutoCompletion( true );
    CBCliente->setEditable( true );

    // Habilita el buscador de direccion.
    connect( CBCliente, SIGNAL( cambioIdCliente( int ) ), this, SLOT( cambioCliente( int ) ) );
    //connect( CBCliente, SIGNAL( currentIndexChanged(int) ), this, SLOT( cambioCliente( int ) ) );

    // Pongo el sistema de relleno
    m = new MProductosTotales( this, CBProductos->listadoProductos() );
    m->calcularTotales( true );
    m->buscarPrecios( true );
    TVContenido->setModel( m );
    TVContenido->setItemDelegateForColumn( 1, new DProductosTotales( TVContenido ) );
    TVContenido->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVContenido->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    TVContenido->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    TVContenido->horizontalHeader()->setMinimumSectionSize( 140 );
    TVContenido->setSortingEnabled( false );

    connect( CBProductos, SIGNAL( agregarProducto() ), this, SLOT( agregarProducto() ) );

    // Pongo los botones en funcionamiento
    PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
    PBAgregar->setText( "Guardar" );
    PBEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
    PBEliminarTodo->setIcon( QIcon( ":/imagenes/eliminar.png" ) );

    connect( PBAgregar     , SIGNAL( clicked() ), this, SLOT( agregarProducto()    ) );
    connect( PBEliminar    , SIGNAL( clicked() ), this, SLOT( eliminarProducto()   ) );
    connect( PBEliminarTodo, SIGNAL( clicked() ), this, SLOT( borrarTodoProducto() ) );

    DSBCant->setValue( 1.0 );
    DSBCant->setPrefix( "" );
}

/*!
 * \fn FormModificarPresupuesto::setearIdPresupuesto( QModelIndex id )
 * Setea el indice de fila y provoca la carga de datos
 */
void FormModificarPresupuesto::setearIdPresupuesto( QModelIndex idx )
{
    _id_presupuesto = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();

    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha, serie, numero, id_cliente, destinatario, direccion, observaciones FROM presupuestos WHERE id_presupuesto = %1" ).arg( _id_presupuesto ) ) ) {
        cola.next();
        dEFecha->setDate( cola.record().value(0).toDate() );

        NumeroComprobante *nc = new NumeroComprobante( 0, 0, 0 );
        nc->setearNumero( cola.record().value(2).toInt() );
        nc->setearNumeroSerie( cola.record().value(1).toInt() );
        LNumeroComprobante->setText( LNumeroComprobante->text().append( nc->aCadena() ) );
        delete nc;

        if( cola.record().value(3).isNull() ) {
            CBCliente->lineEdit()->setText( cola.record().value(4).toString() );
        } else {
            CBCliente->setearId( cola.record().value(3).toInt() );
        }

        LEDireccion->setText( cola.record().value(5).toString() );

        PTEObservaciones->setPlainText( cola.record().value(6).toString().insert( 0, "Observaciones: " ) );

        // Cargo los datos de los items
        MItemPresupuesto *mi = new MItemPresupuesto();
        mi->setearId( _id_presupuesto );
        mi->select();
        for( int i = 0; i < mi->rowCount(); i++ ) {
            // Relleno con los datos
            m->agregarItem(
                            mi->data( mi->index( i, mi->fieldIndex( "cantidad" ) ), Qt::EditRole ).toInt(),
                            mi->data( mi->index( i, mi->fieldIndex( "texto" ) ), Qt::EditRole ).toString(),
                            mi->data( mi->index( i, mi->fieldIndex( "precio_unitario" ) ), Qt::EditRole ).toDouble()
                          );
        }
        delete mi;

        // Busco si existieron descuentos
        MDescuentos *md = new MDescuentos();
        md->setearTipo( MDescuentos::Presupuesto );
        md->setearIdComprobante( _id_presupuesto );
        if( md->existenDatos() ) {
            md->seleccionarDatos();
            for( int i = 0; i < md->rowCount(); i++ ) {
                m->agregarDescuento(
                                    md->data( md->index( i, mi->fieldIndex( "texto" ) ), Qt::EditRole ).toString(),
                                    md->data( md->index( i, md->fieldIndex( "cantidad" ) ), Qt::EditRole ).toDouble()
                                   );
            }
        }
        delete md;

    } else {
        qDebug() << "Error al buscar los datos del presupuesto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
    \fn FormModificarPresupuesto::cancelar()
        Elimina datos temporales y cierra el formulario
 */
void FormModificarPresupuesto::cancelar()
{ this->close(); }


#include <QMessageBox>
#include <QSqlDatabase>
#include <QDir>

#include "mpresupuesto.h"
#include "EReporte.h"
/*!
    \fn FormModificarPresupuesto::guardar( bool cerrar )
 */
void FormModificarPresupuesto::guardar( bool cerrar )
{
 // Verifico que esten todos los datos - fecha valida
 if( !dEFecha->date().isValid() ) {
     QMessageBox::information( this, "Error de fecha", "La fecha no es valida. Ingrese una correcta." );
     return;
 }
 // Algun cliente seleccionado o agregado
 if( !(CBCliente->currentIndex() >= 0 ) ) {
     QMessageBox::information( this, "Error de destinatario", "El cliente no es valido o no existe un destinatario ingresado. Por favor coloque uno." );
     return;
 }
 // Cuento que tenga al menos un item
 if( m->rowCount() <= 1 ) {
     QMessageBox::information( this, "Error de items", "Por favor, ingrese al menos un item en el detalle del presupuesto." );
     return;
 }
 // Remuevo el texto que dice "Observaciones:"
 QString observacion = PTEObservaciones->toPlainText();
 observacion.remove( "Observaciones: " );
 // Inicio la transacción
 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
 MPresupuesto *mod = new MPresupuesto();

 int id_cliente = CBCliente->idClienteActual();
 bool estado = mod->modificarPresupuesto( _id_presupuesto,
                                          id_cliente,
                                          CBCliente->currentText(),
                                          LEDireccion->text(),
                                          dEFecha->dateTime(),
                                          m->total(),
                                          observacion );

 if( !estado ) {
     qDebug( "Error al intentar guardar elprespuesto." );
     QMessageBox::information( this, "Error", "No se pudo guardar los datos del presupuesto. No se guardo nada" );
     delete mod;
     return;
 }
 delete mod;
 // tengo el id del presupuesto y procedo a guardar los datos de los items
 //////////////////////////////////////////////////////////////////////////b
 // Guardo los items del presupuesto
 // elimino la fila de total
 m->calcularTotales( false );
 MItemPresupuesto *items = new MItemPresupuesto();
 if( !items->eliminarItemsDePresupuesto( _id_presupuesto ) ) {
     qWarning( "No se pudieron eliminar los items del presupuesto" );
     QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
     delete items;
     return;
 }
 for( int fila = 0; fila< TVContenido->model()->rowCount(); fila++ ) {
     if( !items->agregarItemPresupuesto( _id_presupuesto,
                                         m->data( m->index( fila, 0 ), Qt::EditRole ).toDouble(), // Cantidad
                                         m->data( m->index( fila, 1 ), Qt::DisplayRole ).toString(), // Texto
                                         m->data( m->index( fila, 2 ), Qt::EditRole ).toDouble()  // Precio unitario
                                       ) ) {
         qDebug() << QString( "No se pudo actualizar el item %1 del presupuesto a la base de datos" ).arg( fila );
         QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
         return;
     }
 }
 // Si llego hasta aca, termine de guardar todos los datos y ninguno fallo
 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit();
 delete items;
 // Imprimo el presupuesto
 ParameterList lista;
 lista.append( Parameter( "id_presupuesto", _id_presupuesto ) );
 if( id_cliente < 0 ) {
     lista.append( Parameter( "cliente_existe", false ) );
     lista.append( Parameter( "direccion", LEDireccion->text() ) );
 } else if( id_cliente >= 0 ){
     lista.append( Parameter( "cliente_existe", true ) );
     if( id_cliente > 0 ) {
         lista.append( Parameter( "direccion", MClientes::direccionEntera( id_cliente ) ) );
     } else {
         lista.append( Parameter( "direccion", LEDireccion->text() ) );
     }
 }
 EReporte *rep = new EReporte( this );
 rep->presupuesto();
 if( rep->hacer( lista, false ) ) {
     QMessageBox::warning( this, "reporte", "Presupuesto impreso correctamente" );
 } else {
     QMessageBox::warning( this, "Error", "No se pudo hacer el reporte" );
 }
 if( cerrar )
 { this->close(); }
}

/*!
    \fn FormModificarPresupuesto::guardar()
 */
void FormModificarPresupuesto::guardar()
{ guardar( true ); }


/*!
    \fn FormModificarPresupuesto::agregarProducto()
 */
void FormModificarPresupuesto::agregarProducto()
{
    // Verificación previa
    if( DSBCant->value() == 0 )
    { QMessageBox::information( this, "Error de dato", "La cantidad a agregar debe ser mayor que cero", QMessageBox::Ok ); return; }
    if( CBProductos->currentText().isEmpty() )
    { QMessageBox::information( this, "Error de datos", "Ingrese un producto a agregar", QMessageBox::Ok ); return; }

    // Llamo este metodo por si no fue agregado por un enter
    CBProductos->verificarExiste();
    // Inserto el producto
    m->agregarNuevoProducto( DSBCant->value(), CBProductos->idActual() );

    // Reseteo los ingresos de producto
    DSBCant->setValue( 1.0 );
    CBProductos->setCurrentIndex( -1 );
    // Seteo el foco
    DSBCant->setFocus();
}


/*!
    \fn FormModificarPresupuesto::eliminarProducto()
 */
void FormModificarPresupuesto::eliminarProducto()
{
 QModelIndexList lista = TVContenido->selectionModel()->selectedRows();
 if( lista.size() < 1 ) {
     QMessageBox::warning( this, "Seleccione un item",
                     "Por favor, seleccione un item para eliminar",
                     QMessageBox::Ok, QMessageBox::Cancel );
     return;
 }
 int ret = QMessageBox::question( this, QString::fromUtf8( "¿Seguro?" ), QString( "Esta seguro que desea eliminar %1 elemento(s)?" ).arg( lista.size() ), QMessageBox::Ok, QMessageBox::Cancel );
 if( ret == QMessageBox::Ok ) {
     foreach( QModelIndex item, lista )
     {
        if( item.isValid() ) {
            m->removeRow( item.row() );
        }
     }
 }
}

/*!
    \fn FormModificarPresupuesto::borrarTodoProducto()
 */
void FormModificarPresupuesto::borrarTodoProducto()
{
 int ret = QMessageBox::question( this, QString::fromUtf8( "¿Seguro?" ), QString::fromUtf8( "¿Esta seguro que desea eliminar todos los elementos del prespuesto?" ), QMessageBox::Ok, QMessageBox::Cancel );
 if( ret == QMessageBox::Ok ) {
     int fin = m->rowCount() - 1;
     for( int i = 0; i <= fin; i++ ) { m->removeRow( i );  }
     TVContenido->update();
 }
}

#include <mclientes.h>
/*!
  \fn FormModificarPresupuesto::cambioCliente( int id_combo )
  Slot llamado cada vez que el usuario cambia el contenido del combo de cliente o destinatario
  \param id_combo ID del combobox
 */
void FormModificarPresupuesto::cambioCliente( int id_cliente ) {
    LEDireccion->setText( MClientes::direccionEntera( id_cliente ) );
}
