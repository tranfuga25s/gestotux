#include "formmodificarservicio.h"

#include <QMessageBox>
#include <QSqlError>

#include "eactcerrar.h"
#include "eactguardar.h"

FormModificarServicio::FormModificarServicio( MServicios *m, QWidget *parent, Qt::WFlags fl )
 : EVentana( parent, fl ), Ui::FormServicioBase(), modelo( 0 ), mapa( 0 )
{
    setupUi( this );
    this->setWindowTitle( "Modificar Servicio" );
    this->setWindowIcon( QIcon( ":/imagenes/edit.png" ) );

    this->modelo = m;

    this->mapa = new QDataWidgetMapper( this );
    this->mapa->setModel( this->modelo );
    this->mapa->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );

    this->mapa->addMapping( this->LENombre          , this->modelo->fieldIndex( "nombre" )           );
    this->mapa->addMapping( this->TEDescripcion     , this->modelo->fieldIndex( "descripcion" )      );
    this->mapa->addMapping( this->DEFechaAlta       , this->modelo->fieldIndex( "fecha_alta" )       );
    this->mapa->addMapping( this->dSBPrecioBase     , this->modelo->fieldIndex( "precio_base" )      );
    this->mapa->addMapping( this->CBPeriodo         , this->modelo->fieldIndex( "periodo" )          );
    this->mapa->addMapping( this->CBInicioCobro     , this->modelo->fieldIndex( "dia_cobro" )        );
    this->mapa->addMapping( this->CBMetodoIncompleto, this->modelo->fieldIndex( "forma_incompleto" ) );

    // Coloco los periodos
    CBPeriodo->addItem( "Semanal ( 7 dias )"   , MServicios::Semanal       );
    CBPeriodo->addItem( "Quincenal ( 15 dias )", MServicios::Quincenal     );
    CBPeriodo->addItem( "Mensual"              , MServicios::Mensual       );
    CBPeriodo->addItem( "Bi-Mensual"           , MServicios::BiMensual     );
    CBPeriodo->addItem( "Trimestral"           , MServicios::Trimestral    );
    CBPeriodo->addItem( "Cuatrimestral"        , MServicios::Cuatrimestral );
    CBPeriodo->addItem( "Seximestral"          , MServicios::Seximestral   );
    CBPeriodo->addItem( "Anual"                , MServicios::Anual         );

    // Dias en el mes que se hace el batch de calcular los nuevos importes 1->31 ( cuidado con los meses  28 y 30 )
    for( int i=1; i<=31; i++ )
    {
        CBInicioCobro->addItem( QString::number( i ), QString::number( i ) );
    }

    CBMetodoIncompleto->insertItem( -1, "Division por dias y cobro de dias restantes", MServicios::DiasFaltantes );
    CBMetodoIncompleto->insertItem( -1, "Mes Completo",                                MServicios::MesCompleto   );

    addAction( new EActGuardar( this ) );
    addAction( new EActCerrar( this ) );

    connect( CkBBaja, SIGNAL( toggled( bool ) ), this, SLOT( cambiarBaja( bool ) ) );
    DEFechaBaja->setEnabled( CkBBaja->isChecked() );
}


/*!
    \fn FormModificarServicio::cambiarBaja( bool estado )
        Slot llamado para habilitar y deshabilitar el selector de fecha de cuando fue dado de baja
 */
void FormModificarServicio::cambiarBaja( bool estado )
{  DEFechaBaja->setEnabled( estado ); }

/*!
    \fn FormModificarServicio::guardar()
    Slot llamado para guardar los datos
 */
void FormModificarServicio::guardar()
{
    if( mapa->submit() ) {
        QMessageBox::information( this, "Correcto", "Los datos del servicio se pudieron actualizar correctamente" );
        emit actualizarVista();
    } else {
        qDebug( "No se pudo actualizar el servicio" );
        qDebug( modelo->lastError().text().toLocal8Bit() );
        QMessageBox::warning( this, "Error", "Existió un error al modificar los datos del servicio. No se realizaron modificaciones" );
        return;
    }

}
