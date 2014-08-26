/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ebackupremoto.h"
#include "preferencias.h"

#include <QStringList>
#include <QByteArray>
#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QPushButton>
#include <QFrame>
#include <QProgressBar>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTabWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDate>
#include <QProgressDialog>
#include <QTabWidget>
#include <QSqlDriver>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QDebug>

#include "eenviobackup.h"
#include "json.h"

using namespace QtJson;

EBackupRemoto::EBackupRemoto( QWidget* parent )
: EVentana( parent ), Ui_FormBackupRemotoBase()
{
 setupUi(this);
 this->setAttribute( Qt::WA_DeleteOnClose );
 setObjectName( "backupremoto" );
 setWindowTitle( "Copia de Seguridad Remota" );
 setWindowIcon( QIcon( ":/imagenes/backup.png" ) );
 PBProgreso->setValue( 0 );
 PBEnviado->setValue( 0 );
 LDebug->setText( "Presione Iniciar para comenzar" );

 ActCerrar = new QAction( "Cerrar", this );
 ActCerrar->setStatusTip( "Cierra la ventana y cancela cualquier backup que se este realizando" );
 ActCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
 connect( ActCerrar, SIGNAL( triggered() ), this, SLOT( close() ) );

 ActIniciar = new QAction( "Iniciar", this );
 ActIniciar->setStatusTip( "Inincia la generacion de los backups" );
 ActIniciar->setIcon( QIcon( ":/imagenes/next.png" ) );
 connect( ActIniciar, SIGNAL( triggered() ), this, SLOT( iniciar() ) );

 ActDetener = new QAction( "Detener", this );
 ActDetener->setStatusTip( QString::fromUtf8( "Detiene la ejecución actual del backup" ) );
 ActDetener->setIcon( QIcon( ":/imagenes/stop.png" ) );
 connect( ActIniciar, SIGNAL( triggered() ), this, SLOT( detener() ) );
 connect( this, SIGNAL( cambiarDetener( bool ) ), ActDetener, SLOT( setEnabled( bool ) ) );

 emit cambiarDetener( false );

 addAction( ActIniciar );
 addAction( ActDetener );
 addAction( ActCerrar );

 ChBBaseDatos->setCheckState( Qt::Checked );

 Pestanas->setTabIcon( 0, QIcon( ":/imagenes/backup1.png" ) );
 Pestanas->setTabIcon( 1, QIcon( ":/imagenes/backup2.png" ) );
 Pestanas->widget( 0 )->setObjectName( "crearBackup" );
 Pestanas->widget( 1 )->setObjectName( "restaurarBackup" );
 Pestanas->setCurrentIndex( 0 );
 connect( Pestanas, SIGNAL( currentChanged( int ) ), this, SLOT( cambiopestana( int ) ) );

 manager = 0;
 req = 0;
 ids = "";
 terminar = false;
 anteriores = 0;
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "BackupRemoto" );
 _host = p->value( "servidor", "http://trafu.no-ip.org/").toString();
 p->endGroup();
 p->endGroup();
 p=0;

 PBDescarga->setValue( 0 );
 PBRestauracion->setValue( 0 );
 PBEnviado->setValue( 0 );
 PBProgreso->setValue( 0 );
}


EBackupRemoto::~EBackupRemoto()
{
    /*if( !manager )
        delete manager;

    if( !req )
        delete req;*/
}

void EBackupRemoto::cambiopestana( int pes ) {
    if( pes == 1 ) {
        // Busco los datos del sistema para ver cuales hay
        QUrl url( _host + "/backups/historial" );
        preferencias *p = preferencias::getInstancia();
        p->beginGroup( "Preferencias" );
        p->beginGroup( "BackupRemoto" );
        url.addQueryItem( "num_cliente", p->value( "numero_cliente", 1 ).toString() );
        url.addQueryItem( "id_servicio_backup", p->value( "id_servicio_backup", 2 ).toString() );
        url.addQueryItem( "driver", QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() );
        url.addQueryItem( "gestotux", "0" );
        p->endGroup(); p->endGroup(); p = 0;

        if( manager == 0 ) {
            manager = new QNetworkAccessManager( this );
            this->connect( manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( respuestaHistorial( QNetworkReply * ) ) );
        } else {
            this->disconnect( manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( respuestaInicio( QNetworkReply * ) ) );
            this->connect( manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( respuestaHistorial( QNetworkReply * ) ) );
        }


        QNetworkRequest req( url );
        req.setHeader( QNetworkRequest::ContentTypeHeader, "application/octet-stream" );
        manager->post( req, url.encodedQuery() );
    }
}

/*!
 *   \fn EBackupRemoto::iniciar()
 *	Realiza todo el proceso de creacion del backup.
 */
void EBackupRemoto::iniciar()
{
 // Ver en que pestaña esta
 if( Pestanas->currentIndex() == 0 )
 {
  generarBackup();
  return;
 }
 else if( Pestanas->currentIndex() == 1 )
 {
  restaurarBackup();
  return;
 }
 else
 {
  qWarning( "Pestaña desconocida" );
 }
}

/*!
    \fn EBackupRemoto::generar_db( bool estructura )
        Funcion que hace la obtencion de los datos de la base de datos y los prepara para la compresion.
        @param estructura Hacer backup de la estructura de la db
        @return Verdadero si no existieron errores, falso en caso contrario
 */
void EBackupRemoto::generar_db( bool /*estructura*/ )
{
 // Hago la conexion
 manager = new QNetworkAccessManager( this );
 connect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaInicio( QNetworkReply* ) ) );

 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "BackupRemoto" );
 QUrl url( p->value( "url_envio", _host + "/backups/envio" ).toString() );
 url.addQueryItem( "num_cliente", p->value( "numero_cliente", 1 ).toString() );
 url.addQueryItem( "id_servicio_backup", p->value( "id_servicio_backup", 2 ).toString() );
 url.addQueryItem( "driver", QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() );
 p->endGroup(); p->endGroup(); p = 0;

 // Envio el saludo de inicio
 QUrl *envio = new QUrl( url );
 envio->addQueryItem( "inicio", "0"  );

 req = new QNetworkRequest( *envio );
 req->setHeader( QNetworkRequest::ContentTypeHeader, "application/octet-stream" );
 lista.append( manager->post( *req, envio->encodedQuery() ) );
}

/*!
 * \brief EBackupRemoto::enviarColas
 * Funcion que envía el contenido de las colas de ejecución al servidor
 */
void EBackupRemoto::enviarColas() {
 QSqlDriver *db = QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driver();
 ////////////////////////////////////////////////////////////////////////
 QStringList tablas = QSqlDatabase::database().tables();
 int total = 0;
 QString tabla; QSqlQuery cola;
 foreach( tabla, tablas )
 {
       //Calculo para la barra
       cola.exec( QString("SELECT COUNT(*) FROM %1" ).arg( tabla ) );
       if( cola.next() )
       {
               total += cola.record().value(0).toInt();
       }
 }
 total += tablas.size();
 // Preparo todo para que pueda enviar datos
 PBProgreso->setRange( 0, total );
 PBEnviado->setRange( 0, total );

 // Desconecto la señal y pongo la que quiero realmente
 disconnect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaInicio( QNetworkReply* ) ) );
 connect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaColas( QNetworkReply* ) ) );

 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "BackupRemoto" );
 QUrl url( p->value( "url_envio", _host + "/backups/envio" ).toString() );
 url.addQueryItem( "ids", this->ids );
 p->endGroup(); p->endGroup(); p = 0;
 QNetworkRequest *req2 = new QNetworkRequest( url );
 req2->setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
 req2->setRawHeader( "X-Gestotux-Token", "token_seguridad_123456789" ); /// @TODO: modificar esto para que sea más seguro

 int pos = 0;
 _continuar = true;
 foreach( QString tabla, tablas )
 {
        PBProgreso->setValue( PBProgreso->value() + 1 );
        if( !cola.exec( QString( "SELECT * FROM %1" ).arg( tabla ) ) ) {
            qDebug() << "Error al seleccionar los datos de la tabla " << tabla;
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
        while( cola.next() && _continuar )
        {
            QUrl temp;
            temp.addEncodedQueryItem( "posicion", QUrl::toPercentEncoding( QString::number( pos ) ) );
            temp.addEncodedQueryItem( "cola", QUrl::toPercentEncoding( db->sqlStatement( QSqlDriver::InsertStatement, tabla, cola.record(), false ) ) );
            QByteArray data = temp.encodedQuery();

            lista.append( manager->post( *req2, data ) );

            QApplication::processEvents();

            PBProgreso->setValue( PBProgreso->value() + 1 );
            pos++;
        }
        if( _continuar == false ) {
            url.addQueryItem( "cancelar", "0" );
            QApplication::processEvents();
            req->setUrl( url );
            manager->post( *req, url.encodedQuery() );
            disconnect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaColas(QNetworkReply*) ) );
        }
  }
 //////////////////////////////////////////////////////////////////////////////////////////////////////////
 PBProgreso->setValue( PBProgreso->value() + 1 );
 LDebug->setText( "Listo backup de Base de datos.. Esperando envios..." );
 terminar = true;
}

void EBackupRemoto::respuestaColas( QNetworkReply *resp ) {
    if( resp->error() != QNetworkReply::NoError ) {
        QMessageBox::warning( this, "Error", resp->errorString().toLocal8Bit() );
        this->mostrarError( resp->error() );
    } else if( resp->isFinished() ) {
        QByteArray cont( resp->readAll() );
        QApplication::processEvents();
        bool ok = false;
        resp->deleteLater();
        if( cont.isEmpty() ) {
            return;
        }
        lista.removeOne( resp );
        QVariantMap mapa = Json::parse( cont, ok ).toMap();
        if( !ok ) {
            QMessageBox::warning( this, "Error", "Error de interpretación de los datos descargados. Intente nuevamente." );
            _continuar = false;
            return;
        }
        if( mapa["error"].toBool() ) {
            QMessageBox::warning( this, "Error", QString( "Error: %1" ).arg( mapa["mensaje"].toString() ).toLocal8Bit() );
            _continuar = false;
            emit cambiarDetener( false );
            return;
        } else {
            PBEnviado->setValue( PBEnviado->value() + 1 );
            qDebug() << "Cola recibida";
            if( lista.size() == 0 && terminar == true ) {
                 preferencias *p = preferencias::getInstancia();
                 p->beginGroup( "Preferencias" );
                 p->beginGroup( "BackupRemoto" );
                 QUrl url( p->value( "url_envio", _host + "/backups/envio" ).toString() );
                 url.addQueryItem( "num_cliente", p->value( "numero_cliente", 1 ).toString() );
                 url.addQueryItem( "id_servicio_backup", p->value( "id_servicio_backup", 2 ).toString() );
                 url.addQueryItem( "driver", QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() );
                 p->endGroup(); p->endGroup(); p = 0;
                 url.addQueryItem( "fin", "0" );
                 QNetworkRequest req( url );
                 req.setHeader( QNetworkRequest::ContentTypeHeader, "application/octet-stream" );
                 manager->post( req, url.encodedQuery() );
                 disconnect( manager, SIGNAL( finished( QNetworkReply* ) ),this, SLOT( respuestaColas( QNetworkReply* ) ) );
                 connect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaFin( QNetworkReply* ) ) );
                _continuar = false;
                ids = "";
                emit cambiarDetener( false );
            }
        }
    }
}

void EBackupRemoto::respuestaInicio( QNetworkReply *resp ) {
    if( resp->error() != QNetworkReply::NoError ) {
        QMessageBox::warning( this, "Error", resp->errorString().toLocal8Bit() );
        this->mostrarError( resp->error() );
    } else if( resp->isFinished() ) {
        QByteArray cont( resp->readAll() );
        QApplication::processEvents();
        bool ok = false;
        resp->deleteLater();
        if( cont.isEmpty() ) {
            return;
        }
        QVariantMap mapa = Json::parse( cont, ok ).toMap();
        if( !ok ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "Error de interpretación de los datos descargados. Intente nuevamente." ) );
            _continuar = false;
            return;
        }
        if( mapa["error"].toBool() ) {
            QMessageBox::warning( this, "Error", QString( "Error: %1" ).arg( mapa["mensaje"].toString() ).toLocal8Bit() );
            _continuar = false;
            emit cambiarDetener( false );
            return;
        } else {
            lista.removeAll( resp );
            PBEnviado->setValue( PBEnviado->value() + 1 );
            disconnect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaInicio( QNetworkReply* ) ) );
            if( mapa.contains("ids") ) {
                this->ids = mapa["ids"].toString();
            } else {
                qWarning( "No se encontro el identificador de backup" );
                abort();
            }
            enviarColas();
        }
    }
}

/*!
 *
 */
void EBackupRemoto::respuestaFin( QNetworkReply *resp ) {
    if( resp->error() != QNetworkReply::NoError ) {
        QMessageBox::warning( this, "Error", resp->errorString().toLocal8Bit() );
        this->mostrarError( resp->error() );
    } else if( resp->isFinished() ) {
        QByteArray cont( resp->readAll() );
        QApplication::processEvents();
        bool ok = false;
        resp->deleteLater();
        if( cont.isEmpty() ) {
            return;
        }
        QVariantMap mapa = Json::parse( cont, ok ).toMap();
        if( !ok ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "Error de interpretación de los datos descargados. Intente nuevamente." ) );
            _continuar = false;
            return;
        }
        if( mapa["error"].toBool() ) {
            QMessageBox::warning( this, "Error", QString( "Error: %1" ).arg( mapa["mensaje"].toString() ).toLocal8Bit() );
            _continuar = false;
            emit cambiarDetener( false );
            return;
        } else {
            lista.clear();
            PBEnviado->setValue( PBEnviado->value() + 1 );
            disconnect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuestaFin( QNetworkReply* ) ) );
            QMessageBox::information( this, "Correcto", "El backup ha sido generado y guardado correctamente." );
        }
    }
}

/*!
    \fn EBackupRemoto::detener()
     Slot llamado para detener la generacion del backup
 */
void EBackupRemoto::detener()
{  _continuar = false; }


/*!
    \fn EBackupRemoto::generarBackup()
 */
void EBackupRemoto::generarBackup()
{
  emit cambiarDetener( true );
 _continuar = true;
 /*if( !ChBBaseDatos->isChecked() && !ChBConfirugacion->isChecked() )
 {
  QMessageBox::information( this, "Seleccione opciones" , "Por favor, seleccione una opcion para iniciar la copia de seguridad" );
  emit cambiarDetener( false );
  return;
 }*/
 LDebug->setText( "Iniciando" );
 if( ChBBaseDatos->isChecked() )
 {
  LDebug->setText( "Generando backup de Base de datos" );
  generar_db( false );
  return;
 }
 /*if( ChBConfirugacion->isChecked() )
 {
  LDebug->setText( "Generando backup de Configuracion del programa" );
  if( !generar_config() )
  {
   qDebug( "Error al generar el backup de la config" );
   emit cambiarDetener( false );
   return;
  }
 }
 else
 {
  PBProgreso->setValue( PBProgreso->maximum() );
 }*/
 emit cambiarDetener( false );
}


/*!
    \fn EbackupRemoto::restaurarBackup()
 */
void EBackupRemoto::restaurarBackup()
{
 emit cambiarDetener( true );
 _continuar = true;
 return;
}

/*!
    \fn EbackupRemoto::ejecutarColas( QStringList colas )
    Metodo utilizado para ejecutar las consultas SQl definidas en @param colas en la base de datos.
    Todas las consultas se realizan dentro de una transacción, y de fallar cualquiera de las consultas, se realiza un rollback.
    @param colas Lista de QString con las consultas SQL dentro.
    @return Verdadero si todas las consultas fueron ejecutadas correctamente.
 */
bool EBackupRemoto::ejecutarColas( QStringList colas )
{
 bool estado = true;
 QSqlQuery *cola = new QSqlQuery();
 while( colas.size() > 0  && estado == true )
 {
  if( cola->exec( colas.at( 0 ) ) )
  {
    colas.removeFirst();
  }
  else
  {
        qWarning() << cola->lastError().text() << ". Cola: " << cola->lastQuery();
        estado = false;
  }
 }
 delete cola;
 if( estado == false )
 {
  QSqlDatabase::database().rollback();
 }
 else
 {
  QSqlDatabase::database().commit();
 }
 return estado;
}

/*!
 * \fn EBackupRemoto::respuestaHistorial( QNetworkReply * )
 * Slot llamado para cargar los datos del historial de backups
 */
void EBackupRemoto::respuestaHistorial( QNetworkReply *resp )
{
    if( resp->error() != QNetworkReply::NoError ) {
        // Notifico el error
        this->mostrarError( resp->error() );
        QMessageBox::information( this, "Error", "Hubo un error de comunicación con el servidor. Por favor intente nuevamente más tarde" );
        qDebug() << resp->readAll();
        return;
    } else {
        QByteArray cont( resp->readAll() );
        QApplication::processEvents();
        resp->deleteLater();
        bool ok = false;
        QVariantMap mapa = Json::parse( cont, ok ).toMap();
        if( !ok ) {
            QMessageBox::warning( this, "Error", "Error de interpretación de los datos descargados. Intente nuevamente." );
            qDebug() << cont;
            return;
        }
        if( mapa["error"].toBool() ) {
            QMessageBox::warning( this, "Error", QString( "Error: %1" ).arg( mapa["mensaje"].toString() ).toLocal8Bit() );
            return;
        }
        // Cargo los datos al modelo
        if( !mapa.contains("data") ) {
            QMessageBox::warning( this, "Error", "La cadena descargada no contiene el elemento data" );
            return;
        }
        // Cargo los datos al modelo del historial
        if( anteriores == 0 ) {
            anteriores = new MTempBackups( this );
        } else {
            anteriores->clear();
        }
        TVRestaurar->setModel( anteriores );
        qDebug() << mapa;
        /*foreach( QVariant arreglo, mapa ) {

        }*/

        //QVariant(QVariantMap, QMap(("Backup", QVariant(QVariantMap, QMap(("archivo_db", QVariant(QString, "/var/www/trsis/app/webroot/backups/1/QSQLITE/20130623180427.bkp") ) ( "archivo_est" ,  QVariant(, ) ) ( "archivo_pref" ,  QVariant(, ) ) ( "fecha" ,  QVariant(QString, "2013-06-23 18:04:27") ) ( "id_backup" ,  QVariant(QString, "12") ) ( "servicio_backup_id" ,  QVariant(QString, "2") ) ( "tamano" ,  QVariant(QString, "7122581") ) ( "usuario_id" ,  QVariant(QString, "1") ) )  ) ) ( "ServicioBackup" ,  QVariant(QVariantMap, QMap(("fecha_alta", QVariant(QString, "2012-05-22") ) ( "id_servicio" ,  QVariant(QString, "4") ) ( "id_servicio_backup" ,  QVariant(QString, "2") ) ( "limite_cantidad" ,  QVariant(QString, "2147483647") ) ( "limite_espacio" ,  QVariant(QString, "5368709120") ) )  ) ) ( "Usuario" ,  QVariant(QVariantMap, QMap(("cliente_id", QVariant(QString, "6") ) ( "contra" ,  QVariant(QString, "8807387a6325d5d3201faef03f7d1004fd4632fa") ) ( "id_usuario" ,  QVariant(QString, "1") ) ( "ultimo_acceso" ,  QVariant(QString, "2012-05-23 00:00:00") ) ( "ultimo_backup" ,  QVariant(QString, "2012-07-19 11:52:00") ) )  ) ) )  ) )  );
    }

}

void EBackupRemoto::mostrarError( QNetworkReply::NetworkError e ) {
    switch( e ) {
        case QNetworkReply::NoError:
        { qDebug( "Sin error" ); break; }
        case QNetworkReply::ConnectionRefusedError:
        { qDebug( "El servidor esta rechazando la conexión activamente." ); break; }
        case QNetworkReply::RemoteHostClosedError:
        { qDebug( "El servidor cerro la conexión abruptamente." ); break; }
        case QNetworkReply::HostNotFoundError:
        { qDebug( "El servidor especificado no fue encontrado." ); break; }
        case QNetworkReply::TimeoutError:
        { qDebug( "Tiempo de espera agotado para esta solicitud." ); break; }
        case QNetworkReply::OperationCanceledError:
        { qDebug( "Operación cancelada." ); break; }
        case QNetworkReply::SslHandshakeFailedError:
        { qDebug( "Error al establecer la conexión SSL." ); break; }
        case QNetworkReply::ProxyConnectionRefusedError:
        { qDebug( "El servidor proxy rechazó la conexión." ); break; }
        case QNetworkReply::ProxyNotFoundError:
        { qDebug( "El servidor proxy no fue encontrado." ); break; }
        case QNetworkReply::ProxyTimeoutError:
        { qDebug( "El servidor proxy tardó mucho en contestar." ); break; }
        case QNetworkReply::ProxyAuthenticationRequiredError:
        { qDebug( "El proxy solicita autentificación." ); break; }
        case QNetworkReply::ProxyConnectionClosedError:
        { qDebug( "El proxy cerro la conexión." ); break; }
        case QNetworkReply::ContentAccessDenied:
        { qDebug( "El contedo asociado ha sido denegado por el servidor." ); break; }
        case QNetworkReply::ContentOperationNotPermittedError:
        { qDebug( "Operación con el contenido no permitida" ); break; }
        case QNetworkReply::ContentNotFoundError:
        { qDebug( "El contenido buscado no fue encontrado." ); break; }
        case QNetworkReply::AuthenticationRequiredError:
        { qDebug( "Se requiere autentificaciòn para ingresar a este contenido." ); break; }
        case QNetworkReply::ContentReSendError:
        { qDebug( "Es necesario reenviar el conteido." ); break; }
        case QNetworkReply::ProtocolUnknownError:
        { qDebug( "Protocolo Desconocido." ); break; }
        case QNetworkReply::ProtocolInvalidOperationError:
        { qDebug( "Operacion del protocolo invalida." ); break; }
        case QNetworkReply::UnknownNetworkError:
        { qDebug( "Error de red desconocido." ); break; }
        case QNetworkReply::UnknownProxyError:
        { qDebug( "Error de proxy desconocido." ); break; }
        case QNetworkReply::UnknownContentError:
        { qDebug( "Contenido desconocido." ); break; }
        case QNetworkReply::ProtocolFailure:
        { qDebug( "Falla en el protocolo." ); break; }
#if QT_VERSION >= 0x040801
        case QNetworkReply::TemporaryNetworkFailureError:
        { qDebug( "Error de red temporal" ); break; }
#endif
    }

}
