#include "ecbprovincias.h"

#include <QDebug>

ECBProvincias::ECBProvincias(QWidget *parent) :
    QComboBox(parent)
{
    this->setObjectName( "SelectorProvincias" );
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

    _id_provincia = -1;
    _inicializado = false;
    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );
}

void ECBProvincias::setearFiltro( const QString f ) {
    this->filtro = f;
    // Hago la carga diferida
    QTimer timer;
    timer.singleShot( 900, this, SLOT( inicializar() ) );
    _inicializado = false;
}

ECBProvincias::~ECBProvincias()
{
    delete ids;
}

/*!
 * \fn ECBProvincias::inicializar()
 * Funcion que carga los datos y setea todo como para su uso
 */
void ECBProvincias::inicializar()
{
    // Cargo los datos del modelo
    QSqlQuery cola;
    // Limpio el combobox para que no cargue datos repetidos
    this->clear();
    if( cola.exec( QString( "SELECT id_provincia, nombre FROM provincias %1 ORDER BY nombre ASC" ).arg( filtro ) ) ) {
        int pos = 0;
        int npos = -1;
        while( cola.next() ) {
            this->insertItem( pos, cola.record().value(1).toString() );
            int id = cola.record().value(0).toInt();
            if( id == _id_provincia ) { npos = pos; }
            ids->insert( pos, id );
            pos++;
        }
        if( pos == 0 ) {
            qWarning( "No hay ninguna provincia para cargar!" );
            this->lineEdit()->setText( "No hay provincias cargadas..." );
            this->setEnabled( false );
            return;
        }
        this->setEnabled( true );
        this->setCurrentIndex( npos );
    } else {
        qWarning( "ECBProvincias::inicializar: Error al intentar ejecutar la cola para cargar las provincias" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
 * \fn ECBProvincias::cambioPais( int pos )
 * Slot llamado cada vez que se cambia el indice del cb de pais
 * \param pos Posicion del combobox
 */
void ECBProvincias::cambioPais( int /* pos */ )
{
    if( this->cbpaises != 0 ) {
        if( this->cbpaises->idActual() > 0 ) {
            setearFiltro( QString( "WHERE id_pais = %1" ).arg( this->cbpaises->idActual() ) );
        }
    } else {
        qDebug( "ECBProvincias::cambioPais: No se puede filtrar por pais porque no se ha seteado el puntero correspondiente" );
    }
}

/*!
 * \fn ECBProvincias::idActual()
 * Devuelve el id de cliente actual
 */
int ECBProvincias::idActual()
{ return ids->value( this->currentIndex() ); }

void ECBProvincias::setearId( const int id_provincia )
{
    if( id_provincia <= 0 ) { return; }
    if( ! _inicializado ) {
        this->_id_provincia = id_provincia;
        return;
    }
    int pos = this->ids->indexOf( id_provincia );
    if( pos < 0 ) {
        qDebug() << "ECBProvincias::setearId: Error buscando el id de provincia desde cbprovincias: id = " << id_provincia;
        this->setCurrentIndex( -1 );
    } else {
        this->setCurrentIndex( pos );
    }
}

void ECBProvincias::setearPais( const int id_pais )
{
    if( id_pais > 0 ) {
        setearFiltro( QString( "WHERE id_pais = %1" ).arg( id_pais ) );
    } else {
        qDebug( "ECBProvincias::setearPais: Identificador de pais invalido" );
    }
}

/*!
 * \fn ECBProvincias::verificarExiste()
 * Ve si el numero de cliente o texto buscado existe en la lista. Si existe lo coloca como item actual.
 */
void ECBProvincias::verificarExiste()
{
    // Veo que tipo de entrada es
    QString buscar = this->lineEdit()->text();
    int b = this->findText( buscar );
    if( b != -1 ) {
        this->setCurrentIndex( b );
    } else {
        // No es un nombre de cliente sino un numero de cliente.
        //int c =  this->findData( buscar );
        int c = ids->indexOf( buscar.toInt() );
        if( c != -1 ) {
            this->setCurrentIndex( c );
        } else {
            QMessageBox::information( this, "No encontrado", "Provincia o codigo de provincia buscado no existe." );
        }
    }
    return;
}

/*!
 * \fn ECBProvincias::setearCBPais( ECBPaises *cb )
 * Ayuda a conectar las señales para que cuando se cambie el pais se renueve la provincia
 */
void ECBProvincias::setearCBPais( ECBPaises *cb )
{
    if( cb != 0 ) {
        connect( cb, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioPais( int ) ) );
        this->cbpaises = cb;
    } else {
        qDebug( "ECBProvincias::setearCBPais: puntero de combo box invalido" );
    }
}
