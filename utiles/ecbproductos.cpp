#include "ecbproductos.h"

#include <QCompleter>
#include <QTimer>
#include <QLineEdit>
#include <QDebug>

ECBProductos::ECBProductos( QWidget *parent ) :
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

    this->_min = -1;
    this->_mostrar_deshabilitados = false;
    this->_mostrar_sin_stock = false;
    this->_id_proveedor = -1;

    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );
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

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
/*!
 * \fn ECBProductos::inicializar()
 * Función que carga los datos y setea todo como para su uso
 */
void ECBProductos::inicializar()
{
    // Cargo los datos del modelo
    QSqlQuery cola;
    QString tcola;
    tcola.append( "SELECT id, codigo, nombre FROM producto WHERE " );
    if( !_mostrar_deshabilitados ) {
        tcola.append( " habilitado IN ( 1, 'true' ) " );
    }
    if( !_mostrar_sin_stock && !_mostrar_deshabilitados ) {
        tcola.append( " AND " );
    }
    if( !_mostrar_sin_stock ) {
        tcola.append( " stock > 0 " );
    }
    if( _id_proveedor > 0 ) {
        if( !_mostrar_sin_stock || !_mostrar_deshabilitados
                || ( !_mostrar_sin_stock && !_mostrar_deshabilitados ) ) {
            tcola.append( " AND " );
        }
        tcola.append( QString( " id IN ( "
                               "  SELECT DISTINCT( id_producto ) "
                               "  FROM compras_productos  "
                               "  WHERE id_compra IN (    "
                               "      SELECT id_compra    "
                               "      FROM compras        "
                               "      WHERE id_proveedor = %1"
                               "  )"
                               " )" ).arg( _id_proveedor ) );
    }
    tcola.append( " ORDER BY nombre ASC" );
    if( cola.exec( tcola ) ) {
        int pos = 0;
        this->clear();
        this->_mapa_pos_codigo->clear();
        this->_mapa_id_nombre->clear();
        this->_mapa_pos_ids->clear();
        while( cola.next() ) {
            // Pos = currentIndex();
            // id_producto = _mapa_pos_ids
            // codigo = _mapa_pos_codigo
            this->insertItem( pos, cola.record().value(2).toString(), cola.record().value(0).toInt() );
            this->_mapa_pos_codigo->insert( cola.record().value(1).toString(), pos );
            this->_mapa_id_nombre->insert ( cola.record().value(0).toInt()   , cola.record().value(2).toString() );
            this->_mapa_pos_ids->insert   ( pos, cola.record().value(0).toInt() );
            pos++;
        }
        if( pos == 0 ) {
            qWarning( "No hay ningun producto para cargar!" );
            this->lineEdit()->setText( "No hay productos cargados..." );
        }
        this->setEnabled( true );
        this->setCurrentIndex( -1 );
    } else {
        qWarning( "Error al intentar ejecutar la cola para cargar los productos" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
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
    QList<int> l2 = lista->keys();
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
    }
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
            this->_mapa_pos_codigo->insert( QString::number( _min ), pos_nueva );
            this->_mapa_pos_ids->insert( pos_nueva, _min );
            this->_mapa_id_nombre->insert( _min, this->lineEdit()->text() );
            this->insertItem( pos_nueva, this->lineEdit()->text(), _min );
            this->setCurrentIndex( pos_nueva );
            this->_min--;
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
    if( _mostrar_deshabilitados != estado ) {
        _mostrar_deshabilitados = estado;
        inicializar();
    }
}

/*!
 * \brief ECBProductos::setearMostrarSinStock
 * \param estado
 */
void ECBProductos::setearMostrarSinStock( bool estado )
{
    if( _mostrar_sin_stock != estado ) {
        _mostrar_sin_stock = estado;
        inicializar();
    }
}

/*!
 *
 */
void ECBProductos::filtrarPorProveedor( const int id_proveedor )
{
    if( id_proveedor > 0 ) {
       this->_id_proveedor = id_proveedor;
        inicializar();
    }
}
