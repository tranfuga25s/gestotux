#include "formprefbackupremoto.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>

#include "json.h"

using namespace QtJson;

FormPrefBackupRemoto::FormPrefBackupRemoto(QWidget *parent, Qt::WFlags fl )
    : QWidget ( parent, fl ), Ui::FormPrefBackupRemoto()
{
    setupUi(this);
    this->setWindowTitle( "Backup Remoto" );
    this->setWindowIcon( QIcon( ":/imagenes/backup.png" ) );
    this->setAttribute( Qt::WA_DeleteOnClose );

    connect( PBVerificar, SIGNAL( clicked() ), this, SLOT( cargarDatos() ) );

    connect( GBUsar, SIGNAL( toggled( bool ) ), GBHost, SLOT( setChecked( bool ) ) );

    connect( LENumeroCliente, SIGNAL( editingFinished() ), this, SLOT( verificarDatosValidos() ) );
    connect( LEContra, SIGNAL( editingFinished() ), this, SLOT( verificarDatosValidos() ) );

    GBDatos->setEnabled( false );
    PBVerificar->setEnabled( false );
}

/*!
    \fn FormPrefBackupRemoto::guardar()
*/
void FormPrefBackupRemoto::guardar()
{
    preferencias * p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "BackupRemoto" );
    p->setValue( "habilitado", GBUsar->isChecked() );
    p->setValue( "cliente", LENumeroCliente->text() );
    p->setValue( "contra", LEContra->text() );
    p->setValue( "servidor", LEServidor->text() );
    p->endGroup();
    p->endGroup();
    delete p;
    p=0;
}


/*!
    \fn FormPrefBackupRemoto::aplicar()
*/
void FormPrefBackupRemoto::aplicar()
{
    this->guardar();
}


/*!
    \fn FormPrefBackupRemoto::cargar()
*/
void FormPrefBackupRemoto::cargar()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "BackupRemoto" );
    GBUsar->setChecked( p->value( "habilitado", false ).toBool() );
    LENumeroCliente->setText( p->value( "cliente", "" ).toString() );
    LEContra->setText( p->value( "contra", "" ).toString() );
    LEServidor->setText( p->value( "servidor", "http://www.gestotux.com.ar/" ).toString() );
    p->endGroup();
    p->endGroup();
    delete p;
    p=0;
    verificarDatosValidos();
}

void FormPrefBackupRemoto::cargarDatos()
{
  // conecto al servidor e intento cargar los datos
  GBDatos->setEnabled( false );
  PBVerificar->setText( "Comprobando..." );
  manager = new QNetworkAccessManager( this );
  connect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuesta( QNetworkReply* ) ) );

  QUrl url( LEServidor->text() + "/usuarios/verificar" );
  url.addQueryItem( "num_cliente", LENumeroCliente->text() );
  url.addQueryItem( "codigo", LEContra->text() );
  QNetworkRequest req( url );
  req.setHeader( QNetworkRequest::ContentTypeHeader, "application/octet-stream" );
  manager->post( req, url.encodedQuery() );
}

/*!
 * Slot que verifique que se carge algún dato en la entrada de numero de usuario y contraseña para habilitar el botón
 */
void FormPrefBackupRemoto::verificarDatosValidos()
{
    bool habilitar = false;
    if( !LENumeroCliente->text().isEmpty() ) {
        habilitar = true;
    } else {
        habilitar = false;
    }
    if( !LEContra->text().isEmpty() ) {
        habilitar = true;
    } else {
        habilitar = false;
    }
    PBVerificar->setEnabled( habilitar );
}


void FormPrefBackupRemoto::respuesta( QNetworkReply *resp )
{
    PBVerificar->setText( "Comprobar" );
    if( resp->error() != QNetworkReply::NoError ) {
        QMessageBox::warning( this, "Error", resp->errorString().toLocal8Bit() );
        qDebug() << resp->readAll();
    } else  if( resp->isFinished() ) {
        QByteArray cont( resp->readAll() );
        bool ok = false;
        QVariantMap mapa = Json::parse( cont, ok ).toMap();
        if( !ok ) {
            QMessageBox::warning( this, "Error", "Error de interpretación de los datos descargados. Intente nuevamente." );
            qDebug() << cont;
            return;
        }
        if( mapa["error"].toBool() ) {
            QMessageBox::warning( this, "Error", QString( "Error: %1" ).arg( mapa["texto"].toString() ).toLocal8Bit() );
            return;
        }
        if( mapa["suspendido"].toBool() ) {
            QMessageBox::warning( this, "Suspendido", "Su servicio de backup se encuentra suspendido" );
            return;
        }
        PBCantidad->setRange( 0, mapa["limite_cantidad"].toInt() );
        PBCantidad->setValue( mapa["cantidad"].toInt() );
        PBEspacio->setRange( 0, mapa["limite_espacio"].toInt() );
        PBEspacio->setValue( mapa["espacio"].toInt() );
        LNombre->setText( QString( "Nombre del servicio: %1" ).arg( mapa["nombre"].toString() ) );
        LEspacio->setText( QString( "Espacio Usado: %1 %2 de %3 Gb" ).arg( mapa["espacio"].toInt() ).arg( "" ).arg( mapa["limite_espacio"].toInt() ) );
        LCantidad->setText( QString( "Cantidad utilizada: %1 de %2 " ).arg( mapa["cantidad"].toInt() ).arg( mapa["limite_cantidad"].toInt() ) );
        GBDatos->setEnabled( true );
    }
    resp->deleteLater();
    PBVerificar->setEnabled( false );
}
