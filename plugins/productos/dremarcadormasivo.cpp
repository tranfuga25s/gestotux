#include "dremarcadormasivo.h"

#include <QMessageBox>
#include <QInputDialog>

#include "mproductos.h"
#include "preferencias.h"

DRemarcadorMasivo::DRemarcadorMasivo(QWidget *parent) :
QDialog( parent )
{
  setupUi(this);

  setWindowTitle( "Remarcador masivo" );
  setWindowIcon( QIcon( ":/imagenes/producto.png" ) );

  connect( RBFijo      , SIGNAL( toggled( bool ) ), this, SLOT( cambioAMontoFijo ( bool ) ) );
  connect( RBPorcentaje, SIGNAL( toggled( bool ) ), this, SLOT( cambioAPorcentaje( bool ) ) );

  modelo = new MTempProductoRemarcar( this );

  connect( DSBValor, SIGNAL( valueChanged( double ) ), modelo, SLOT( cambioValor( double ) ) );
  connect( CkBDeshabilitados, SIGNAL( toggled( bool ) ), this, SLOT( cambioDeshabilitados( bool ) ) );
  connect( CkBSinStock, SIGNAL( toggled( bool ) ), this, SLOT( cambioSinStock( bool ) ) );

  TVProductos->setModel( modelo );
  TVProductos->hideColumn( 0 );
  TVProductos->setSelectionBehavior( QAbstractItemView::SelectRows );
  TVProductos->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );

  connect( PBAgregar, SIGNAL( clicked() ), this, SLOT( agregarProducto() ) );
  connect( PBAgregarTodos, SIGNAL( clicked() ), this, SLOT( agregarTodos() ) );
  connect( PBAgregarCategoria, SIGNAL( clicked() ), this, SLOT( agregarCategoria() ) );
  connect( PBAgregarProveedor, SIGNAL( clicked() ), this, SLOT( agregarProveedor() ) );
  connect( PBEliminar, SIGNAL( clicked() ), this, SLOT( eliminarProducto() ) );
  connect( PBEliminarTodos, SIGNAL( clicked() ), this, SLOT( eliminarTodos() ) );

  PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
  PBAgregarTodos->setIcon( QIcon( ":/imagenes/add.png") );
  PBAgregarTodos->setText( "Agregar todos" );

  PBEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
  PBEliminarTodos->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
  PBEliminar->setText( "Eliminar producto" );
  PBEliminarTodos->setText( "Eliminar todos" );

  PBAgregarCategoria->setText( "Categoria" );
  PBAgregarCategoria->setIcon( QIcon( ":/imagenes/add.png" ) );

  PBAgregarProveedor->setText( "Proveedor" );
  PBAgregarProveedor->setIcon( QIcon( ":/imagenes/add.png" ) );

  // Pongo como predeterminado el porcentaje
  RBPorcentaje->setChecked( true );

  GBAvance->setVisible( false );

  _total = 0;

  preferencias *p = preferencias::getInstancia();
  p->inicio();
  p->beginGroup( "Preferencias" );
  p->beginGroup( "Productos");
  PBAgregarCategoria->setVisible( !p->value( "categorias", false ).toBool() );
  p->endGroup();
  p->endGroup();
  p=0;
}

/*!
 * \brief DRemarcadorMasivo::avanzarProgreso
 * \param cantidad
 */
void DRemarcadorMasivo::avanzarProgreso( int cantidad )
{
    int cant = PgBAvance->value() + cantidad;
    PgBAvance->setValue( cant );
    LEstado->setText( QString( "Remarcando %1 de %2 productos..." ).arg( cant ).arg( _total ) );
}

/*!
 * \brief DRemarcadorMasivo::changeEvent
 * \param e
 */
void DRemarcadorMasivo::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
    }
}

/*!
 * \brief DRemarcadorMasivo::cambioAPorcentaje
 * \param estado
 */
void DRemarcadorMasivo::cambioAPorcentaje( bool estado )
{
    if( !estado )
        return;

    DSBValor->setPrefix( "" );
    DSBValor->setSuffix( " %" );
    DSBValor->setValue( 0.0 );
    modelo->setearPorcentaje( true );
}

/*!
 * \brief DRemarcadorMasivo::cambioAMontoFijo
 * \param estado
 */
void DRemarcadorMasivo::cambioAMontoFijo( bool estado )
{
    if( !estado )
        return;

    DSBValor->setPrefix( "$ " );
    DSBValor->setSuffix( "" );
    DSBValor->setValue( 0.0 );
    modelo->setearValorFijo( true );
}

/*!
 * \brief DRemarcadorMasivo::agregarProducto
 */
void DRemarcadorMasivo::agregarProducto()
{
    modelo->agregarProducto( CBProductos->idActual() );
}

/*!
 * \brief DRemarcadorMasivo::agregarCategoria
 */
void DRemarcadorMasivo::agregarCategoria()
{
    int id_categoria = 0;
    QVector<int> lista = MProductos::idsSegunCategoria( id_categoria );
    foreach( int id_producto, lista ) {
        modelo->agregarProducto( id_producto );
    }
}

/*!
 * \brief DRemarcadorMasivo::agregarTodos
 */
void DRemarcadorMasivo::agregarTodos()
{
    foreach( int id_producto, *(CBProductos->getListaIDs()) ) {
        modelo->agregarProducto( id_producto );
    }
}

/*!
 * \brief DRemarcadorMasivo::agregarProveedor
 */
void DRemarcadorMasivo::agregarProveedor()
{
    /// @TODO: Implementar agregado de productos x proveedor
    qWarning( "Todavía no implementado!" );
}

/*!
 * \brief DRemarcadorMasivo::eliminarProducto
 */
void DRemarcadorMasivo::eliminarProducto()
{
  if( TVProductos->selectionModel()->selectedRows().size() <= 0 ) {
      QMessageBox::information( this, "Error", "Por favor, seleccione al menos un producto para sacar del listado" );
      return;
  }
  foreach( QModelIndex idx, TVProductos->selectionModel()->selectedRows() ) {
      modelo->eliminarProducto( idx );
  }
}

/*!
 * \brief DRemarcadorMasivo::eliminarTodos
 */
void DRemarcadorMasivo::eliminarTodos()
{ modelo->eliminarTodos(); }

/*!
 * \brief DRemarcadorMasivo::accept
 * Slot llamado al presionar el boton de aceptar
 */
void DRemarcadorMasivo::accept()
{
    GBAvance->setVisible( true );
    PgBAvance->setRange( 0, modelo->rowCount() );
    this->_total = modelo->rowCount();
    connect( modelo, SIGNAL( aumentoProgreso( int ) ), this, SLOT( avanzarProgreso( int ) ) );
    GBRemarcar->setEnabled( false );
    CBProductos->setEnabled( false );
    PBAgregar->setEnabled( false );
    PBAgregarTodos->setEnabled( false );
    PBEliminar->setEnabled( false );
    PBEliminarTodos->setEnabled( false );
    QPair<int,int> dato = modelo->remarcar();
    if( dato.second != 0 ) {
        QMessageBox::information( this, "Estado", QString( "No se pudieron remarcar %1 productos. Los remarcados han sido eliminados de la lista" ).arg( dato.second ) );
        GBRemarcar->setEnabled( true );
        CBProductos->setEnabled( true );
        PBAgregar->setEnabled( true );
        PBAgregarTodos->setEnabled( true );
        PBEliminar->setEnabled( true );
        PBEliminarTodos->setEnabled( true );
    } else {
        QMessageBox::information( this, "Correcto", QString( "Se han remarcado %1 producto(s) correctamente" ).arg( dato.first ) );
        QDialog::accept();
    }
    return;
}

/*!
 * \brief DRemarcadorMasivo::cambioSinStock
 * Slot llamado cuando se cambia el estado de la casilla de remarcar productos sin stock
 * \param estado Seleccion de la casilla
 */
void DRemarcadorMasivo::cambioSinStock( bool estado )
{
    modelo->cambioSinStock( estado );
    recalcularFiltro();
}

/*!
 * \brief DRemarcadorMasivo::cambioDeshabilitados
 * Slot llamado cuando se cambia el estado de la casilla de remarcar productos deshabilitados
 * \param estado Seleccion de la casilla
 */
void DRemarcadorMasivo::cambioDeshabilitados( bool estado )
{
    modelo->cambioDeshabilitados( estado );
    recalcularFiltro();
}

/*!
 * \brief DRemarcadorMasivo::recalcularFiltro
 * Metodo que permite generar el filtro necesario para filtrar los elementos del combo box de seleccion de productos disponibles
 */
void DRemarcadorMasivo::recalcularFiltro() {
    if( CkBDeshabilitados->isChecked() ) {
        CBProductos->setearMostrarDeshabilitados( true );
    } else {
        CBProductos->setearMostrarDeshabilitados( false );
    }
    if( CkBSinStock->isChecked() ) {
        CBProductos->setearMostrarSinStock( true );
    } else {
        CBProductos->setearMostrarSinStock( false );
    }
}
