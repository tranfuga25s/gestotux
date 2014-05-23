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

    this->lineEdit()->setText( "Cargando datos..." );
    this->setEnabled( false );

    _mapa_pos_codigo = new QMap<QString, int>();
    _mapa_id_nombre = new QMap<int, QString>();
    _mapa_pos_ids = new QMap<int, int>();

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
}

ECBProductos::~ECBProductos()
{
    delete _mapa_pos_codigo;
    _mapa_pos_codigo = 0;
    delete _mapa_id_nombre;
    _mapa_id_nombre = 0;
    delete _mapa_pos_ids;
    _mapa_pos_ids = 0;
}

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
QMap<int, QString> *ECBProductos::listadoProductos()
{ return this->_mapa_id_nombre; }

/*!
 * \brief ECBProductos::setearListado
 * Coloca el listado puesto como parametro como lista de productos
 * \param lista Lista de productos
 */
void ECBProductos::setearListado( QMap<int, QString> *lista )
{
    // Verifica que los demás items estén de acuerdo con esta lista
    // El mappeo debe sacar solo los elementos menores que cero
    /*QList<int> l2 = lista->keys();
    for( int i = 0; i < l2.size(); i++ ) {
        if( l2.value(i) < 0 ) {
            // Ingreso este valor al cb
            QString texto = lista->value( l2.value( i ) );
            int indice = l2.value( i );
            int pos = this->count();
            this->insertItem( pos, texto, indice );
            this->_mapa_id_nombre->insert( indice, texto );
            this->_mapa_pos_ids->insert( pos, indice );
            this->_mapa_pos_codigo->insert( QString::number( indice ), pos );
        }
    }*/
}

/*!
 * \brief ECBProductos::idActual
 * Devuelve el identificador del producto actual
 * \return ID o -1 si el producto no está en la lista
 */
int ECBProductos::idActual() const
{
    return this->_mapa_pos_ids->value( this->currentIndex() );
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
        QMap<QString, int>::const_iterator i =  this->_mapa_pos_codigo->find( buscar );
        if( i != this->_mapa_pos_codigo->end() ) {
            this->setCurrentIndex( i.value() );
        } else {
            // Tengo que agregarlo como item exclusivo
            // Agregado al final pero con ID <= -1
            int pos_nueva = this->count();
            //this->_mapa_pos_codigo->insert( QString::number( _min ), pos_nueva );
            //this->_mapa_pos_ids->insert( pos_nueva, _min );
            //this->_mapa_id_nombre->insert( _min, this->lineEdit()->text() );
            //this->insertItem( pos_nueva, this->lineEdit()->text(), _min );
            this->setCurrentIndex( pos_nueva );
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
    return new QList<int>( _mapa_pos_ids->values() );
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
