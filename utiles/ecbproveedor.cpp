#include "ecbproveedor.h"

#include <QCompleter>
#include <QTimer>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

ECBProveedor::ECBProveedor(QWidget *parent) :
    QComboBox(parent), filtro()
{
    this->setObjectName( "SelectorProveedor" );
    // Seteo las propiedades del combobox
    this->setInsertPolicy( QComboBox::NoInsert );
    this->setSizeAdjustPolicy( QComboBox::AdjustToContentsOnFirstShow );
    this->setEditable( true );
    this->completer()->setCompletionMode( QCompleter::PopupCompletion );
    this->setCurrentIndex( -1 );
    this->setInsertPolicy( QComboBox::NoInsert );
    this->connect( this->lineEdit(), SIGNAL( returnPressed() ), this, SLOT( verificarExiste() ) );

    this->lineEdit()->setText( "Cargando datos..." );
    this->setEnabled( false );

    ids = new QList<int>();

    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );

    connect( this, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioProveedor( int ) ) );

    _inicializado = false;
    _id = -1;
}

void ECBProveedor::setearFiltro( const QString f ) {
    this->filtro = f;
    // Hago la carga diferida
    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );
    _inicializado = false;
}

ECBProveedor::~ECBProveedor()
{
    delete ids;
}

/*!
 * \fn ECBClientes::inicializar()
 * Funcion que carga los datos y setea todo como para su uso
 */
void ECBProveedor::inicializar()
{
    // Cargo los datos del modelo
    QSqlQuery cola;
    // Limpio el combobox para que no cargue datos repetidos
    this->clear();
    if( cola.exec( QString( "SELECT id, nombre FROM proveedor %1 ORDER BY nombre ASC" ).arg( filtro ) ) ) {
        int pos = 0;
        int npos = -1;
        while( cola.next() ) {
            this->insertItem( pos, cola.record().value(1).toString() );
            int id = cola.record().value(0).toInt();
            if( id == _id ) { npos = pos; }
            ids->insert( pos, id );
            pos++;
        }
        if( pos == 0 ) {
            qWarning( "No hay ningun cliente para cargar!" );
            this->lineEdit()->setText( "No hay proveedores cargados..." );
        }
        this->setEnabled( true );
        this->setCurrentIndex( npos );
        _inicializado = true;
    } else {
        qWarning( "Error al intentar ejecutar la cola para cargar los proveedor" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
 * \fn ECBClientes::idClienteActual()
 * Devuelve el id de cliente actual
 */
int ECBProveedor::idProveedorActual()
{
    return ids->value( this->currentIndex() );
}

void ECBProveedor::setearId( const int id_proveedor )
{
    if( ! _inicializado ) {
        _id = id_proveedor;
        return;
    }
    int pos = this->ids->indexOf( id_proveedor );
    if( pos < 0 ) {
        qDebug( "Error buscando el id de proveedor desde cbproveedor" );
        this->setCurrentIndex( -1 );
    } else {
        this->setCurrentIndex( pos );
        emit cambioIdProveedor( idProveedorActual() );
    }
    _id = 0;
}

/*!
 * \fn ECBClientes::verificarExiste()
 * Ve si el numero de cliente o texto buscado existe en la lista. Si existe lo coloca como item actual.
 */
void ECBProveedor::verificarExiste()
{
    // Veo que tipo de entrada es
    QString buscar = this->lineEdit()->text();
    int b = this->findText( buscar );
    if( b != -1 ) {
        this->setCurrentIndex( b );
        emit cambioIdProveedor( idProveedorActual() );
    } else {
        // No es un nombre de cliente sino un numero de cliente.
        //int c =  this->findData( buscar );
        int c = ids->indexOf( buscar.toInt() );
        if( c != -1 ) {
            this->setCurrentIndex( c );
            emit cambioIdProveedor( idProveedorActual() );
        } else {
            QMessageBox::information( this, "No encontrado", "El cliente o codigo de cliente buscado no existe." );
        }
    }
    return;
}
