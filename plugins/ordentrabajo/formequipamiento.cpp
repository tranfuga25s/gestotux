#include "formequipamiento.h"

#include "eactcerrar.h"
#include "eactguardar.h"
#include "mequipamiento.h"

#include <QDebug>
#include <QMessageBox>

FormEquipamiento::FormEquipamiento( QWidget *parent ) :
  EVentana( parent )
{
  setupUi( this );
  this->setWindowTitle( "Equipamiento" );
  this->setWindowIcon( QIcon(":/imagenes/equipamiento.png" ) );

  _id = -1;
  _editar = false;
  _mequipamiento = 0;

  CBMarca->setEditable( true );

  addAction( new EActGuardar( this ) );
  addAction( new EActCerrar( this ) );
}

/*!
 * \brief FormEquipamiento::setearModeloEquipamiento
 * Setea el modelo de equipamiento para su uso.
 * \param m Puntero al modelo a utilizar
 */
void FormEquipamiento::setearModeloEquipamiento( MEquipamiento *m )
{
  if( m != 0 ) { _mequipamiento = m; }
}

/*!
 * \brief FormEquipamiento::setearAgregar
 * Setea el estado del formulario de edición a agregado de nuevo equipamiento
 * \param estado si desea colocarse en agregar o no
 */
void FormEquipamiento::setearAgregar( bool estado )
{
  if( estado ) {
    GBHistorial->setVisible( false );
    _editar = false;
    this->setWindowTitle( "Agregar Equipamiento" );
    this->setWindowIcon( QIcon( ":/imagenes/equipamiento_agregar.png" ) );

  } else {
    GBHistorial->setVisible( true );

    _editar = true;

    this->setWindowTitle( "Editar Equipamiento" );
    this->setWindowIcon( QIcon( ":/imagenes/equipamiento_editar.png" ) );

    if( _mequipamiento == 0 ) {
        _mequipamiento = new MEquipamiento( this );
    }

    if( !_mequipamiento->existe( _id ) ) {
        qWarning( "El equipamiento no existe!" );
        return;
    }

    _mequipamiento->cargarDatos( _id );
    LEDescripcion->setText( _mequipamiento->descripcion() );
    CBMarca->setearTexto( _mequipamiento->marca() );
    LEModelo->setText( _mequipamiento->modelo() );
    CBCliente->setearId( _mequipamiento->idCliente() );
    LENumSerie->setText( _mequipamiento->numeroSerie() );
    SBCantidadElementos->setValue( _mequipamiento->cantidadElementos() );
    PTEObservaciones->setPlainText( _mequipamiento->observaciones() );
    CkBBaja->setChecked( _mequipamiento->dadoDeBaja() );
  }
}

void FormEquipamiento::setearIndice( QModelIndex indice )
{
    // Extraigo el dato del ID de equipameinto
    int temp = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    if( temp > 0 ) {
        this->_id = temp;
        this->cargarDatos();
    } else {
        qDebug() << "Identificador de ID de equipamiento incorrecto";
    }
}

/*!
 * \brief FormEquipamiento::guardar
 * Slot llamado al presionar el botón de guardar
 */
void FormEquipamiento::guardar()
{
    QMessageBox::information( this, "Error", "No implementado" );
}

void FormEquipamiento::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
  }
}

/*!
 * \brief FormEquipamiento::cargarDatos
 * Carga los datos del equipamiento seteado en this->_id
 */
void FormEquipamiento::cargarDatos()
{
    if( this->_id <= 0 ) {
        return;
    }
    // Cargo los datos
    if( _mequipamiento != 0 ) {
        _mapeador = new QDataWidgetMapper( this );
        _mapeador->setModel( _mequipamiento );
        _mapeador->addMapping( LEDescripcion      , _mequipamiento->fieldIndex( "descripcion" )       );
        _mapeador->addMapping( CBMarca            , _mequipamiento->fieldIndex( "modelo" )            );
        _mapeador->addMapping( LEModelo           , _mequipamiento->fieldIndex( "modelo" )            );
        _mapeador->addMapping( CBCliente          , _mequipamiento->fieldIndex( "id_cliente" )        , "idCliente" );
        //_mapeador->addMapping( CkBBaja          , _mequipamiento->fieldIndex( "baja" )              , ??          );
        //_mapeador->addMapping( CkBGarantia      , _mequipamiento->fieldIndex( "garantia" )          , ??          );
        _mapeador->addMapping( PTEObservaciones   , _mequipamiento->fieldIndex( "observaciones" )     );
        _mapeador->addMapping( LENumSerie         , _mequipamiento->fieldIndex( "numero_serie" )      );
        _mapeador->addMapping( SBCantidadElementos, _mequipamiento->fieldIndex( "cant_elementos" )    );
    }
}
