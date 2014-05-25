#include "ecbproductos.h"

#include <QCompleter>
#include <QTimer>
#include <QLineEdit>
#include <QDebug>

#include "preferencias.h"
#include "ecbproductosmodel.h"
#include "ecbproductosfilter.h"

ECBProductos::ECBProductos( QWidget *parent, ECBProductosFilter *m  ) :
 QComboBox( parent )
{
    this->setObjectName( "SelectorProductos" );
    // Seteo las propiedades del combobox
    this->setInsertPolicy( QComboBox::NoInsert );
    this->setSizeAdjustPolicy( QComboBox::AdjustToContentsOnFirstShow );
    this->setEditable( true );
    this->completer()->setCompletionMode( QCompleter::PopupCompletion );
    this->setCurrentIndex( -1 );
    this->setInsertPolicy( QComboBox::NoInsert );
    this->connect( this->lineEdit(), SIGNAL( returnPressed() ), this, SLOT( enterApretado() ) );

    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    //this->_mostrar_stock_lista = p->value( "mostrar-stock-lista", false ).toBool();
    p->endGroup();
    p->endGroup();
    p->endGroup();
    p=0;

    if( m != 0 ) {
        modelo = m;
    } else {
        modelo = new ECBProductosFilter( this );
        ECBProductosModel *m = new ECBProductosModel( modelo );
        m->inicializar();
        modelo->setSourceModel( m );
    }

    this->setModel( modelo );
    this->setModelColumn( ECBProductosModel::Nombres );
}

ECBProductos::~ECBProductos()
{}

/*!
 * \brief ECBProductos::enterApretado
 */
void ECBProductos::enterApretado()
{
    verificarExiste();
    emit agregarProducto();
    return;
}

/*!
 * \brief ECBProductos::listadoProductos
 * Devuelve el listado de productos mapeado
 * \return
 */
ECBProductosModel *ECBProductos::listadoProductos()
{ return qobject_cast<ECBProductosModel *>(this->modelo->sourceModel()); }

/*!
 * \brief ECBProductos::setearListado
 * Coloca el modelo puesto como parametro como lista de productos para el combobox
 * \param lista Lista de productos
 */
void ECBProductos::setearListado( ECBProductosModel *lista )
{
    this->modelo->setSourceModel( lista );
}

/*!
 * \brief ECBProductos::idActual
 * Devuelve el identificador del producto actual
 * \return ID o -1 si el producto no está en la lista
 */
int ECBProductos::idActual() const
{
    return this->modelo->data( this->modelo->index( this->currentIndex(), ECBProductosModel::Ids ), Qt::EditRole ).toInt();
}

/*!
 * \brief ECBProductos::verificarExiste
 * Verifica que el elementos que está actualmente exista
 */
void ECBProductos::verificarExiste()
{
    // Veo que tipo de entrada es
    QString buscar = this->lineEdit()->text();
    int b = this->findText( buscar );
    if( b != -1 ) {
        this->setCurrentIndex( b );
    } else {
        // Busco por codigo
        int pos = this->modelo->buscarPorCodigo( buscar );
        if( pos != -1 ) {
            this->setCurrentIndex( pos );
        }
    }
}

/*!
 * \brief ECBProductos::getListaIDs
 * Devuelve la lista de ID's de productos que hay en seleccion actualmente
 * \return
 */
QList<int> *ECBProductos::getListaIDs()
{
    return modelo->getListaIDs();
}

/*!
 * \brief ECBProductos::setearMostrarHabilitados
 * \param estado
 */
void ECBProductos::setearMostrarDeshabilitados( bool estado )
{
    this->modelo->setearNoMostrarProductosDeshabilitados( estado );
}

/*!
 * \brief ECBProductos::setearMostrarSinStock
 * \param estado
 */
void ECBProductos::setearMostrarSinStock( bool estado )
{
   this->modelo->setearNoMostrarProductosSinStock( estado );
}

/*!
 * Permite filtrar los productos por proveedor
 */
void ECBProductos::filtrarPorProveedor( const int id_proveedor )
{
    modelo->setearIdProveedor( id_proveedor );
}

/**
 * @brief ECBProductos::setearModelo
 * @param modelo
 */
void ECBProductos::setearModelo(ECBProductosFilter *modelo)
{
    this->modelo = modelo;
    this->setModel( this->modelo );
    this->setModelColumn( ECBProductosModel::Nombres );
}
