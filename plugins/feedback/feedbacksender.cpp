#include "feedbacksender.h"

#include "preferencias.h"
#include <QFile>
#include <QApplication>
#include <QDir>
#include <QMessageBox>

FeedbackSender::FeedbackSender(QObject *parent) :
QObject( parent )
{
    reintentos = 4;
}

/**
 * @brief FeedbackSender::verificarEnvío
 * Codigo que verifica la existencia de un error previo y envía el error al servidor de gestotux
 * @author Esteban Zeller
 */
void FeedbackSender::verificarEnvio()
{
    if( reintentos == 0 ) {
        qDebug( "Feedback: Numero de reintentos maximo alcanzados" );
        emit terminar();
        return;
    }

    preferencias *p = preferencias::getInstancia();
    p->inicio();
    bool ultimo_error = p->value( "lastError", false ).toBool();

    if( ultimo_error == false ) {
        //qDebug( "Feedback: No hubo error en el ultimo cierre.");
        emit terminar(); // No hubo error en el ultimo cierre
    }
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Feedback" );
    bool enviar = p->value( "enviar_reporte", true ).toBool();

    if( !enviar ) {
        emit terminar(); // No quiere enviar los datos
    }

    // Abro el archivo y genero el encabezado para que llegue al servidor
    if( !QFile::exists(  QApplication::applicationDirPath().append( QDir::separator() ).append( "debugOld.txt" ) ) ) {
       // qDebug( "Feedback: No se pudo enviar el archivo de errores ya que no existe la copia anterior" );
        emit terminar();
        return;
    }

    f = new QFile( QApplication::applicationDirPath().append( QDir::separator() ).append( "debugOld.txt" ) );
    if( ! f->open( QIODevice::ReadOnly ) ) {
        //qDebug( "Feedback: Error al intentar abrir el archivo debugOld.txt como solo lectura" );
        emit terminar();
        return;
    }

    manager = new QNetworkAccessManager( this );
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( respuesta( QNetworkReply* ) ) );

    QUrl url( p->value( "url_envio", "http://www.gestotux.com.ar/feedbacks/enviar" ).toString() );

    p->inicio();
    p->beginGroup( "carga" );
    url.addQueryItem( "cliente", p->value( "pluginInfo", "default" ).toString() );
    p->endGroup(); p = 0;

    url.addQueryItem( "data", f->readAll().replace( '\n', "<br />" ) );

    req = new QNetworkRequest( url );

    // Envio el archivo
    lista = manager->post( *req, url.encodedQuery() );

}

/**
 * \fn FeedbackSender::respuesta( QNetworkReply *resp )
 * \param resp Respuesta recibida del servidor
 * Slot llamado cuando se obtiene la respuesta
 **/
void FeedbackSender::respuesta( QNetworkReply *resp )
{
    if( resp->error() != QNetworkReply::NoError ) {
        //qDebug( "Feedback: Error de red al enviar informe de errores" );
        //qDebug( resp->errorString().toLocal8Bit() );
        // Intento nuevamente más tarde hasta n veces
        reintentos--;
        //qDebug( QString( "Feedback: Numero de reintentos faltantes: %1" ).arg( reintentos ).toLocal8Bit() );
        emit pausar( 10 ); // Este sleep es en segundos
    } else if( resp->isFinished() ) {
        QApplication::processEvents();
        resp->deleteLater();
        //qDebug( "Feedback: Envio de error completado." );
        f->close();
        f->remove();
        emit terminar();
   }
}

