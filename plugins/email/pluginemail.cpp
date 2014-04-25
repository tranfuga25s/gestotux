#include "pluginemail.h"

#include "formprefemail.h"
#include "eservidoremail.h"
/*#include "smtp.h"
#include "eemail.h"*/

#include <QSqlDatabase>
#include <QLabel>

bool PluginEmail::inicializar()
{
 // Iniciailizo el hilo de ejecucion que se conecta al servidor
 EServidorEmail *datosServer = new EServidorEmail();

 /*enviador = new Smtp(   datosServer->usuario(),
                        datosServer->password(),
                        datosServer->direccion(),
                        this,
                        QSqlDatabase::cloneDatabase( QSqlDatabase::database(), "correo" ) );*/
 delete datosServer;
 // La cola la mantiene el mismo objecto
 _etiqueta = new QLabel();
 // Conecto las señales para cambiar la etiqueta de estado
 //connect( enviador, SIGNAL( status( const QString& ) ), this, SLOT( cambioEstado( const QString& ) ) );
 //connect( EEmail::instancia(), SIGNAL( enviar( Mail * ) ), enviador, SLOT( queueMail( Mail * ) ) );
 return true;
}

bool PluginEmail::verificarTablas( QStringList lista)
{
 if( !lista.contains( "servidores_email" ) )
 { qWarning( "Error al buscar la tabla servidores_email" ); return false; }
 if( !lista.contains( "emails" ) )
 { qWarning( "Error al buscar la tabla emails" ); return false; }
 return true;
}

double PluginEmail::version() const
{
 return 0.1;
}

int PluginEmail::tipo() const
{
 return EPlugin::email;
}

QList< QActionGroup * > PluginEmail::accionesBarra()
{
 return QList<QActionGroup *>();
}

QString PluginEmail::nombre() const
{
 return "email";
}

QWidgetList PluginEmail::formsPreferencias()
{
 QWidgetList lista;
 lista.append( new FormPrefEmail() );
 return lista;
}

void PluginEmail::crearMenu(QMenuBar* m)
{}

void PluginEmail::crearToolBar(QToolBar* t)
{}

/*!
    \fn PluginEmail::seCierraGestotux()
 */
void PluginEmail::seCierraGestotux()
{
 /*disconnect( enviador, SIGNAL( status( const QString& ) ), this, SLOT( cambioEstado( const QString& ) ) );
 // matar el hilo de ejecucion....
 enviador->terminarEjecucion();
 enviador->terminate();
 qDebug( "Cerrado plugin email" );*/
 return;
}

/*!
    \fn PluginEmail::cambioEstado( const QString &texto )
    Cambia la etiqueta de la barra de estado donde se indica el estado de envio de emails.
    @param texto Texto a colocar en la etiqueta
 */
void PluginEmail::cambioEstado( const QString &texto )
{
 if( _etiqueta->isVisible() )
 {
  _etiqueta->setText( texto );
  _etiqueta->adjustSize();
 }
}

Q_EXPORT_PLUGIN2( email, PluginEmail )


/*!
    \fn PluginEmail::statusBarWidget()
    Devuelve la etiqueta que se va a colocar en la barra de estado al enviar emails.
    @return QLabel para agregar en la barra de estado.
 */
QWidget * PluginEmail::statusBarWidget()
{
    return _etiqueta;
}

/*!
 * \brief PluginEmail::botonPantallaInicial
 * \return
 */
QAction *PluginEmail::botonPantallaInicial()
{
    return new QAction( this );
}

/*!
 * \brief PluginEmail::crearAccionesGlobales
 */
void PluginEmail::crearAccionesGlobales(QMainWindow *)
{return;}
