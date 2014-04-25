#include "ecbclientes.h"

#include <QCompleter>
#include <QTimer>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

ECBClientes::ECBClientes(QWidget *parent) :
    QComboBox(parent), filtro()
{
    this->setObjectName( "SelectorClientes" );
    // Seteo las propiedades del combobox
    this->setInsertPolicy( QComboBox::NoInsert );
    this->setSizeAdjustPolicy( QComboBox::AdjustToContentsOnFirstShow );
    this->setEditable( true );
    this->completer()->setCompletionMode( QCompleter::PopupCompletion );
    this->setCurrentIndex( -1 );
    this->setInsertPolicy( QComboBox::NoInsert );
    this->connect( this->lineEdit(), SIGNAL( returnPressed() ), this, SLOT( verificarExiste() ) );
    this->connect( this->lineEdit(), SIGNAL( lostFocus() ), this, SLOT( verificarExiste() ) );

    this->lineEdit()->setText( "Cargando datos..." );
    this->setEnabled( false );

    ids = new QList<int>();

    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );

    connect( this, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioCliente( int ) ) );

    _inicializado = false;
    _no_alertar_no_existentes = false;
    _id = -1;
}

void ECBClientes::setearFiltro( const QString f ) {
    this->filtro = f;
    // Hago la carga diferida
    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );
    _inicializado = false;
}

ECBClientes::~ECBClientes()
{
    delete ids;
}

/*!
 * \fn ECBClientes::inicializar()
 * Funcion que carga los datos y setea todo como para su uso
 */
void ECBClientes::inicializar()
{
    // Cargo los datos del modelo
    QSqlQuery cola;
    // Limpio el combobox para que no cargue datos repetidos
    this->clear();
    if( cola.exec( QString( "SELECT id, razon_social FROM clientes %1 ORDER BY razon_social ASC" ).arg( filtro ) ) ) {
        int npos = -1;
        int pos = 0;
        while( cola.next() ) {
            this->insertItem( pos, cola.record().value(1).toString() );
            int id = cola.record().value(0).toInt();
            if( id == _id ) { npos = pos; }
            ids->insert( pos, id );
            pos++;
        }
        if( pos == 0 ) {
            qWarning( "No hay ningun cliente para cargar!" );
            this->lineEdit()->setText( "No hay clientes cargados..." );
        }
        this->setEnabled( true );
        this->setCurrentIndex( npos );
        _inicializado = true;
    } else {
        qWarning( "Error al intentar ejecutar la cola para cargar los clientes" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
 * \fn ECBClientes::idClienteActual()
 * Devuelve el id de cliente actual
 */
int ECBClientes::idClienteActual()
{
    return ids->value( this->currentIndex() );
}

void ECBClientes::setearId(const int id_cliente)
{
    if( !_inicializado ) {
        _id = id_cliente;
        return;
    }
    int pos = this->ids->indexOf( id_cliente );
    if( pos < 0 ) {
        qDebug( "Error buscando el id de cliente desde cbcliente" );
        this->setCurrentIndex( -1 );
    } else {
        this->setCurrentIndex( pos );
        emit cambioIdCliente( idClienteActual() );
    }
}

/*!
 * \fn ECBClientes::verificarExiste()
 * Ve si el numero de cliente o texto buscado existe en la lista. Si existe lo coloca como item actual.
 */
void ECBClientes::verificarExiste()
{
    // Veo que tipo de entrada es
    if( this->lineEdit()->text().isEmpty() ||
        this->lineEdit()->text().isNull() ) {
        return;
    }
    QString buscar = this->lineEdit()->text();
    int b = this->findText( buscar );
    if( b != -1 ) {
        this->setCurrentIndex( b );
        emit cambioIdCliente( idClienteActual() );
    } else {
        // No es un nombre de cliente sino un numero de cliente.
        //int c =  this->findData( buscar );
        int c = ids->indexOf( buscar.toInt() );
        if( c != -1 ) {
            this->setCurrentIndex( c );
            emit cambioIdCliente( idClienteActual() );
        } else if( !_no_alertar_no_existentes ) {
            QMessageBox::information( this, "No encontrado", "El cliente o codigo de cliente buscado no existe." );
        }
    }
    emit cambioIdCliente( -1 );
    return;
}
