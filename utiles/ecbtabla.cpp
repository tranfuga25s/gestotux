#include "ecbtabla.h"

#include <QCompleter>
#include <QLineEdit>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

ECBTabla::ECBTabla(QWidget *parent) :
 QComboBox(parent)
{
    setObjectName( "CBTabla" );
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
    codigos = new QList<int>();

    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );

    connect( this, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioIdSeleccionado( int ) ) );

    _inicializado = false;
    _id = -1;
    _texto = QString();
    _tabla = QString();
    _campo_id = QString();
    _campo_texto = QString();
    _campo_orden = QString();
    _campo_busqueda = QString();
    _busqueda = false;

}

ECBTabla::ECBTabla( QWidget *parent, QString tabla, QString tid, QString texto, QString orden ) :
 QComboBox(parent)
{
    setObjectName( "CBTabla" );
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
    codigos = new QList<int>();

    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );

    connect( this, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioIdSeleccionado( int ) ) );

    _inicializado = false;
    _id = -1;
    _texto = QString();
    _tabla = tabla;
    _campo_id = tid;
    _campo_texto = texto;
    _campo_orden = orden;
}

/*!
 * \fn ECBTabla::setearFiltro( const QString f )
 * Setea el filtro al actual
 * \param f Filtro tal cual se lo pasaría a la base de datos
 * \param inmmediato Si esta en falso ( valor prederminado ) se esperarn 900 ms para ejecutar las consultas
 */
void ECBTabla::setearFiltro( const QString f , const bool inmmediate ) {
    this->filtro = f;
    // Hago la carga diferida
    if( !inmmediate ) {
        QTimer timer;
        timer.singleShot( 900, this, SLOT( inicializar() ) );
        _inicializado = false;
    } else {
        _inicializado = false;
        inicializar();
    }
}

void ECBTabla::setearTexto( const QString t )
{
    if( !_inicializado ) {
        _texto = t;
        return;
    }
    if( this->ids->isEmpty() || this->count() == 0 ) {
        return;
    }

    // Buscar El elemento
    int id = this->findText( _texto );
    if( id < 0 ) {
        qDebug( "Error buscando el texto en el combo box" );
        this->setCurrentIndex( -1 );
    } else {
        this->setCurrentIndex( id );
        emit cambioId( idActual() );
    }
}

ECBTabla::~ECBTabla()
{
    delete ids;
    delete codigos;
}

/*!
 * \fn ECBTabla::inicializar()
 * Funcion que carga los datos y setea todo como para su uso
 */
void ECBTabla::inicializar()
{
    if( _tabla.isEmpty() )        { qWarning( "No se seteo la tabla para el modelo." );          return; }
    if( _campo_id.isEmpty() )     { qWarning( "No se seteo el campo de id para el modelo." );    return; }
    if( _campo_texto.isEmpty() )  { qWarning( "No se seteo el campo de texto para el modelo." ); return; }

    if( !_campo_orden.isEmpty() && !_campo_orden.contains( " ORDER BY " ) ) { _campo_orden.prepend( " ORDER BY " ); }
    if( _busqueda && !_campo_busqueda.isEmpty() ) { _campo_busqueda.prepend( ", " ); }
    // Cargo los datos del modelo
    QSqlQuery cola;
    // Limpio el combobox para que no cargue datos repetidos
    this->clear();
    /*qDebug() << QString( "SELECT %1, %2 %6 FROM %3 %4 %5" )
                .arg( _campo_id )
                .arg( _campo_texto )
                .arg( _tabla )
                .arg( filtro )
                .arg( _campo_orden )
                .arg( _campo_busqueda );*/
    if( cola.exec( QString( "SELECT %1, %2 %6 FROM %3 %4 %5" )
                   .arg( _campo_id )
                   .arg( _campo_texto )
                   .arg( _tabla )
                   .arg( filtro )
                   .arg( _campo_orden )
                   .arg( _campo_busqueda ) ) ) {
        int npos = -1;
        int pos = 0;
        while( cola.next() ) {
            this->insertItem( pos, cola.record().value(1).toString() );
            int id = cola.record().value(0).toInt();
            QString texto = cola.record().value(1).toString();
            if( id == _id ) { npos = pos; }
            if( texto == _texto ) { npos = pos; }
            ids->insert( pos, id );
            if( _busqueda ) { codigos->insert( pos, cola.record().value(2).toInt() ); }
            pos++;
        }
        if( pos == 0 ) {
            qDebug( "No hay ningun dato para cargar!" );
            this->lineEdit()->setText( "No hay datos cargados..." );
            //qDebug() << cola.lastQuery();
            emit sinDatos();
            return;
        }
        this->setEnabled( true );
        this->setCurrentIndex( npos );
        _inicializado = true;
    } else {
        qWarning( "Error al intentar ejecutar la cola para cargar los datos" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
 * \fn ECBTabla::idActual()
 * Devuelve el id actual
 */
int ECBTabla::idActual()
{
    return ids->value( this->currentIndex() );
}

/*!
 * \fn ECBTabla::setearId( const int id )
 * Setea el id de tabla al item del combo box - ATENCION: No es el currentIndex del combobox.
 */
void ECBTabla::setearId( const int id )
{
    if( !_inicializado ) {
        _id = id;
        return;
    }
    if( this->ids->isEmpty() || this->count() == 0 ) {
        return;
    }
    if( id <= 0 ) {
        qDebug( "ID Erroneo: < o = que cero " );
        return;
    }
    int pos = this->ids->indexOf( id );
    if( pos < 0 ) {
        qDebug( "Error buscando el id de cliente desde cbcliente" );
        this->setCurrentIndex( -1 );
    } else {
        this->setCurrentIndex( pos );
        emit cambioId( idActual() );
    }
}

/*!
 * \fn ECBTabla::setearTabla( QString tabla )
 * Setea el nombre de la tabla
 * \param tabla Nombre de la tabla
 */
void ECBTabla::setearTabla(QString tabla)
{
    _tabla = tabla;
    _inicializado = false;
}

/*!
 * \fn ECBTabla::setearCampoId( QString nombre )
 * Setea el nombre del campo identificatorio de la tabla
 * \param nombre Nombre del campo
 */
void ECBTabla::setearCampoId( QString nombre )
{
    _campo_id = nombre;
    _inicializado = false;
}

/*!
 * \fn ECBTabla::setearCampoTexto( QString nombre )
 * Setea el nombre del campo que se mostrará en el texto del combobox
 * \param nombre Nombre del campo
 */
void ECBTabla::setearCampoTexto( QString nombre )
{
    _campo_texto = nombre;
    _inicializado = false;
}

/*!
 * \fn ECBTabla::setearCampoTexto( QString texto )
 * Setea el nombre del campo identificatorio de la tabla
 * \param texto Texto sql para el ORDER BY
 */
void ECBTabla::setearCampoOrden( QString texto )
{
    _campo_orden = texto;
    _inicializado = false;
}

void ECBTabla::setearCampoBusquedaExtra( QString campo )
{
    _busqueda = true;
    _campo_busqueda = campo;
}

/*!
 * \fn ECBClientes::verificarExiste()
 * Ve si el numero de cliente o texto buscado existe en la lista. Si existe lo coloca como item actual.
 */
void ECBTabla::verificarExiste()
{
    // Veo que tipo de entrada es
    QString buscar = this->lineEdit()->text();
    int b = this->findText( buscar );
    if( b != -1 ) {
        this->setCurrentIndex( b );
        emit cambioId( idActual() );
    } else {
        // No es un nombre de cliente sino un numero de cliente.
        int c = ids->indexOf( buscar.toInt() );
        if( c != -1 ) {
            this->setCurrentIndex( c );
            emit cambioId( idActual() );
        } else {
            if( _busqueda ) {
                int d = codigos->indexOf( buscar.toInt() );
                if( d != -1 ) {
                    this->setCurrentIndex( d );
                    emit cambioId( idActual() );
                } else {
                    QMessageBox::information( this, "No encontrado", "El cliente o codigo de cliente buscado no existe." );
                }
            } else {
                    QMessageBox::information( this, "No encontrado", "El texto o codigo de cliente buscado no existe." );
            }
        }
    }
    return;
}

