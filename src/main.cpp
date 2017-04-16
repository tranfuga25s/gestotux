/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
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

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSqlDatabase>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QIcon>
#include <QApplication>
#include <QMessageBox>
#include <QPluginLoader>
#include <QDate>
#include <QDebug>

#include "gestotux.h"
#include "preferencias.h"
#include "esplash.h"
#include "emysql.h"
#include "eplugin.h"
#include "einfoprogramainterface.h"
#include "formulariocentral.h"
#include "eregistroplugins.h"
#include "EReporte.h"
#include "mproductostotales.h"

FILE *debug;
/*!
 * /fn myMessageOutput( QtMsgType type, const char *msg )
 * Función de salida personalizada para redireccionar la salida de la aplicación al archivo debug.txt y mantener un registro de todas las cosas que suceden dentro del programa.
 * @param type Tipo de salida.
 * @param msg Mensaje de salida.
 */
 void myMessageOutput(QtMsgType type, const char *msg)
 {
     const char *time = QDateTime::currentDateTime().toString(Qt::ISODate).toAscii().constData();
     switch (type) {
         case QtDebugMsg:
            fprintf(debug, "%s: Debug: %s\n", time, msg );
            fflush(debug);
            break;
         case QtWarningMsg:
            #ifdef GESTOTUX_DESARROLLO
            QMessageBox::warning( 0, "Warning de aplicacion", msg );
            #endif
            fprintf(debug, "%s: warning: %s\n", time, msg );
            fflush(debug);
            break;
         case QtCriticalMsg:
            QMessageBox::critical( 0, "Error Critico", msg );
            fprintf(debug, "%s: critico: %s\n", time, msg );
            fflush(debug);
            break;
         case QtFatalMsg:
            fprintf( debug, "%s: Fatal: %s\n", time, msg );
            fflush(debug);
            QMessageBox::critical( 0, "¡¡¡¡¡¡FATAL!!!!!!", msg );
            abort();
     }
 }


/*!
 * \fn hacerTablas( QString nombrePlug )
 * Función auxiliar que irve para generar las tablas al inicializar la aplicacion o cuando se inicializa un nuevo plugin.
 * Busca dentro los recursos embebidos, dentro de la carpeta sql, el archivo <plugin>.<driversql>.sql
 * @param nombrePlug Nombre del plugin que se intenta inicializar sus datos.
 * @return si se pudo crear la tabla o si existio algun error al intentar ejecutar la cola de creación.
 */
bool hacerTablas( QString nombrePlug )
{
 if( QFile::exists( ":/sql/"+nombrePlug+"."+QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName()+".sql" ) )
 {
        QFile archivo( ":/sql/"+nombrePlug+"."+QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName()+".sql" );
        if( archivo.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
                QStringList cadenas = QString( archivo.readAll() ).split( ";" );
                QString cadena; QSqlQuery cola;
                foreach( cadena, cadenas )
                {
                        //qDebug() << cadena;
                        if( cadena.isEmpty() || cadena.isNull() ) {
                                qDebug() << "Cadena vacia, salteandola...";
                            } else {
                                if( !cola.exec( cadena ) )
                                {
                                        qDebug() << cadena;
                                        qDebug() << " Fallo...." << cola.lastError().text();
                                        return false;
                                }
                                else
                                { qDebug( "Ok" ); }
                            }
                }
                return true;
        }
        else
        {
                qWarning() << "Error al abrir el archivo: :/sql/" << nombrePlug << "." << QSqlDatabase::database().driverName() << ".sql";
                return false;
        }
 }
 else
 {
  qWarning() << "No se pudo generar las tablas del plugin " << nombrePlug << ". No se encontro el archivo: :/sql/" << nombrePlug << "." << QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() << ".sql";
  return false;
 }
}

/*!
 * \fn generarInterconexiones()
 * Genera las interconexiones entre plugins
 */
void generarInterconexiones( QMainWindow *ventana_principal )
{
    ERegistroPlugins *egp = ERegistroPlugins::getInstancia();
    if( egp->existePlugin( "presupuesto" ) && egp->existePlugin( "ventas" ) ) {
       QObject::connect( egp->pluginQObject( "presupuesto" ),
                          SIGNAL( emitirFactura( int, QDate, MProductosTotales * ) ),
                          egp->pluginQObject( "ventas" ),
                          SLOT( agregarFactura( int, QDate, MProductosTotales * ) ) );
    }
    if( egp->existePlugin( "ventas" ) && egp->existePlugin( "pagos" ) ) {
       QObject::connect( egp->pluginQObject( "ventas" ),
                         SIGNAL( emitirRecibo( int, QString, double ) ),
                         egp->pluginQObject( "pagos" ),
                         SLOT( agregarRecibo( int, QString, double ) ) );
    }
    if( egp->existePlugin( "ctacte" ) && egp->existePlugin( "pagos" ) ) {
       QObject::connect( egp->pluginQObject( "ctacte"  ),
                         SIGNAL( emitirRecibo( int, QString, double ) ),
                         egp->pluginQObject( "pagos" ),
                         SLOT( agregarRecibo( int, QString, double ) ) );
       QObject::connect( egp->pluginQObject( "ctacte" ),
                         SIGNAL( mostrarRecibo( int ) ),
                         egp->pluginQObject( "pagos" ),
                         SLOT( mostrarRecibo( int ) ) );
    }
    // Integracion de cuentas corrientes con facturas
    if( egp->existePlugin( "ctacte" ) && egp->existePlugin( "ventas" ) ) {
       QObject::connect( egp->pluginQObject( "ctacte" ),
                         SIGNAL( mostrarFactura( int ) ),
                         egp->pluginQObject( "ventas" ),
                         SLOT( mostrarFactura( int ) ) );
    }
    // Integracion de cuentas corrientes con remitos
    if( egp->existePlugin( "ctacte" ) && egp->existePlugin( "remitos" ) ) {
       QObject::connect( egp->pluginQObject( "ctacte" ),
                         SIGNAL( mostrarRemito( int ) ),
                         egp->pluginQObject( "remitos" ),
                         SLOT( mostrarRemito( int ) ) );
    }
    // Estructura de señales para integración de cuotas con ventas
    if( egp->existePlugin( "ventas" ) && egp->existePlugin( "cuotas" ) ) {
       QObject::connect( egp->pluginQObject( "ventas"  ),
                         SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ),
                         egp->pluginQObject( "cuotas" ),
                         SLOT( generarPlanCuotas( int, double, MPlanCuota::TipoComprobante ) ) );
       QObject::connect( egp->pluginQObject( "cuotas" ),
                         SIGNAL( emitirPlanCuotaId( int ) ),
                         egp->pluginQObject( "ventas"  ),
                         SIGNAL( planCuotaSetearIdCuota( int ) ) );
       QObject::connect( egp->pluginQObject( "ventas"  ),
                         SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ),
                         egp->pluginQObject( "cuotas" ),
                         SLOT( planCuotasSetearIdFactura( int, int ) ) );
    }
    // Estructura de señales para integración de cuotas con remitos
    if( egp->existePlugin( "remitos" ) && egp->existePlugin( "cuotas" ) ) {
       QObject::connect( egp->pluginQObject( "remitos"  ),
                         SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ),
                         egp->pluginQObject( "cuotas" ),
                         SLOT( generarPlanCuotas( int, double, MPlanCuota::TipoComprobante ) ) );
       QObject::connect( egp->pluginQObject( "cuotas" ),
                         SIGNAL( emitirPlanCuotaId( int ) ),
                         egp->pluginQObject( "remitos"  ),
                         SIGNAL( planCuotaSetearIdCuota( int ) ) );
       QObject::connect( egp->pluginQObject( "remitos"  ),
                         SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ),
                         egp->pluginQObject( "cuotas" ),
                         SLOT( planCuotasSetearIdFactura( int, int ) ) );
    }
    if( egp->existePlugin( "servicios" ) ) {
        QObject::connect( egp->pluginQObject( "servicios" ),
                          SIGNAL( editarCliente( int ) ),
                          ventana_principal,
                          SLOT( editarCliente( int ) ) );
    }
    // Estructura de señales para integración de ventas con garantias
    if( egp->existePlugin( "ventas" ) && egp->existePlugin( "garantias" ) ) {
       QObject::connect( egp->pluginQObject( "ventas"  ),
                         SIGNAL( emitirGarantia( int, int, QString, int ) ),
                         egp->pluginQObject( "garantias" ),
                         SLOT( agregarGarantiaProducto( int, int, QString, int ) ) );
    }
    if( egp->existePlugin( "remitos" ) && egp->existePlugin( "garantias" ) ) {
      QObject::connect( egp->pluginQObject( "remitos"  ),
                        SIGNAL( emitirGarantia( int, int, QString, int ) ),
                        egp->pluginQObject( "garantias" ),
                        SLOT( agregarGarantiaProducto( int, int, QString, int ) ) );
    }
}

/**
 * \mainpage Documentacion interna de Gestotux
 *
 * Esta es la documentacion interna de gestotux respecto a su estructura de codigo. Si desea conocer mas datos hacerca de el modo de funcionamiento del programa por favor dirijase a http://gestotux.googlecode.com/ para acceder al wiki de el programa.
 */
int main(int argc, char *argv[])
{
      // Inicializa imagenes y archivos internos
      Q_INIT_RESOURCE(gestotux);
      QApplication app(argc, argv);
      // Maneja la salida del programa
      // Elimino el archivo anterior si existe para que el rename no falle
      QString dir1 = QApplication::applicationDirPath().append( QDir::separator() );
      QString dir2 = dir1;
      dir2.append( "debug.txt" );
      QFile::rename( dir1 + "debugOld.txt", dir1 + "debug" + QDateTime::currentDateTime().toString( "ddMyyhms" ) + ".txt" );
      QFile::copy( dir2, dir1.append( "debugOld.txt") );
      debug = fopen( QApplication::applicationDirPath().append( QDir::separator() ).append( "debug.txt" ).toLocal8Bit(), "w" );
      fseek( debug, 0, 0 );
      qInstallMsgHandler(myMessageOutput);
      // Muestro el splash
      ESplash splash;
      splash.show();
      splash.showMessage( "Cargando propiedades locales" );
      // Permite que el programa tenga el Look & Feel del escritorio actual
      app.setDesktopSettingsAware( true );
      preferencias *p = preferencias::getInstancia();
      // Valor de referencia para el plugin de retroalimentacion
      p->setValue( "error_cierre", true );
      p->sync();
      // Busco las preferencias de estilo
      p->beginGroup( "Preferencias" );
      p->beginGroup( "General" );
      QApplication::setStyle( QStyleFactory::create( p->value( "estilo", "float" ).toString() ) );
      app.setEffectEnabled( Qt::UI_AnimateMenu, true );
      app.setEffectEnabled( Qt::UI_AnimateCombo, true );
      app.setEffectEnabled( Qt::UI_FadeTooltip, true );
      app.setEffectEnabled( Qt::UI_FadeMenu, true );
      if( p->value( "sobreestilo", false ).toBool() )
      {
                QDir dir( QCoreApplication::applicationDirPath() );
                dir.cd( "sobreestilos" );
                dir.cd( p->value( "sobreestilonombre", "" ).toString() );
                QFile file( dir.absoluteFilePath( p->value( "sobreestilonombre", "" ).toString().append( ".qss" ) ) );
                file.open(QFile::ReadOnly);
                QString styleSheet = QLatin1String(file.readAll());
                qApp->setStyleSheet(styleSheet);
      }
      // Preferencias Idiomaticas
      QLocale locale( QLocale::Spanish, QLocale::Argentina );
      QLocale::setDefault( locale );
      splash.showMessage( "Cargando Traduccion" );
      // Cargo las traducciones
      QDir *directorio = new QDir( QCoreApplication::applicationDirPath() );
      directorio->cd( "traducciones" );
      QTranslator tran_qt;
      if( tran_qt.load( directorio->absoluteFilePath( "qt_es" ) ) )
      { app.installTranslator(&tran_qt); } else { qDebug( "Fallo al cargar la traduccion de qt" ); }
      QTranslator tran_nc;
      if( tran_nc.load( directorio->absoluteFilePath( "ncreport_es" ) ) )
      { QCoreApplication::instance()->installTranslator(&tran_nc); } else { qDebug( "Fallo al cargar la traduccion del reporte" ); }
      delete directorio;
      directorio = 0;
      splash.showMessage( "Cargando Base de datos" );
      qDebug() << "Drivers disponiles: " << QSqlDatabase::drivers();
      // Chequeo la Base de Datos
      bool fallosql = false;
      if( ( QSqlDatabase::isDriverAvailable( "QMYSQL" ) == true && p->value( "dbExterna", false ).toBool() ) || !p->value( "noForzarMysql", true ).toBool() )
      {
         //qWarning( "Usando mysql" );
         EMysql dialogo;
         int ret = dialogo.exec();
         switch( ret )
         {
                case EMysql::Conectado:
                {
                        qDebug( "Base de datos abierta correctamente" );
                        fallosql = false;
                        break;
                }
                case EMysql::Cancelado:
                {
                        qWarning( "No se puede continuar sin la base de datos. Se saldra del programa" );
                        exit(0);
                        break;
                }
                case EMysql::Interna:
                {
                        fallosql = true;
                        break;
                }
                default:
                {
                        qWarning() << "Retorno desconocido: " << QString::number( ret );
                        abort();
                        break;
                }
        }
      } else { fallosql = true; }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Cargo el driver que este disponible, usando db interna y no se fuerza a usar mysql
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      if( QSqlDatabase::isDriverAvailable( "QSQLITE" ) && fallosql == true )
      {
       QFile *base = new QFile( QApplication::applicationDirPath().append( QDir::separator() ).append( "gestotux.database" ).toLocal8Bit() );
       if( !base->open( QIODevice::ReadOnly ) )
       {
         qDebug( "-------------------------------------------------" );
         qDebug( "El archivo de Base de datos no existe!");
         qDebug( "-------------------------------------------------" );
                QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
                DB.setDatabaseName( QApplication::applicationDirPath().append( QDir::separator() ).append( "gestotux.database" ) );
                if( !DB.open() )
                {
                        qDebug() << "Ultimo error: " << DB.lastError().text();
                        abort();
                }
        }
        else
        {
                // Aunque exista chequeo que no sea de tam 0
                if( base->size() <= 0 )
                {
                        qFatal( "Error! El archivo de db tiene menos o es igual a 0 bytes " );
                }
        }
        delete base;
        QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
        DB.setDatabaseName( QApplication::applicationDirPath().append( QDir::separator() ).append( "gestotux.database" ) );
        if( !DB.open() )
        {
                qDebug() << "Ultimo error: " << DB.lastError().text();
                abort();
        } else { qDebug( "Base de datos SQLite abierta correctamente" ); }
        /// FIN SQLITE
       }
       else if( fallosql == true || !QSqlDatabase::database().isValid() )
       {
        // No se puede usar sqlite para el programa
        qDebug( "No se puede encontrar el plug-in para la Base de Datos" );
        qDebug() << "Lista de Drivers Soportados:" << QSqlDatabase::drivers();
        abort();
       }
       ////////////////////////////////////////////////////////////////////////////////////////////////////
       // Inicia codigo general
       ////////////////////////////////////////////////////////////////////////////////////////////////////
       // Chequeo si existen las tablas, llegado este punto la base de datos debe estar abierta
       QStringList tablas = QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).tables( QSql::Tables );
       tablas.append( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).tables( QSql::Views ) );
       if( tablas.isEmpty() )
       {
                // Es la primera vez que se arranca el programa
                qDebug( "No existen tablas en la base de datos." );
                hacerTablas( "tablas" );
                // Cada plugin debe inicializar sus propias tablas
       }
       // Fin de arranque de la base de datos
        splash.showMessage( "Base de datos Abierta correctamente" );
        /////////////////////////////////////////////////////////////////////////////////////////////////
        // Cargo los plugins aca
        // Salgo de la preferencia General, porque no quiero ahi datos de la carga de los plugins
        p->endGroup();
        /////////////////////////////////////////////////////////////////////////////////////////////////
        // Necesito el formulario principal para algunas cosas
        splash.showMessage( "Cargando Ventana Principal" );
        gestotux * mw = new gestotux();
        ////////////////////////////////////////////////////////
        splash.showMessage( "Cargando plugins" );
        QPluginLoader loader;
        loader.setLoadHints( QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint );
        QDir pluginsDir = QDir(qApp->applicationDirPath());

         #if defined(Q_OS_WIN)
             if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
                 pluginsDir.cdUp();
         #elif defined(Q_OS_MAC)
             if (pluginsDir.dirName() == "MacOS") {
                 pluginsDir.cdUp();
                 pluginsDir.cdUp();
                 pluginsDir.cdUp();
             }
         #endif
             pluginsDir.cd("plugins");
                QStringList filtro;
        #ifdef Q_WS_WIN32
                filtro.append( "*.dll" );
        #endif
        #ifdef Q_WS_X11
                filtro.append( "*.so" );
        #endif
             foreach( QString fileName, pluginsDir.entryList( filtro, QDir::Files ) )
             {
                loader.setFileName(  pluginsDir.absoluteFilePath( fileName )  );
                 if( loader.load() )
                 {
                        QObject *obj = loader.instance();
                        EPlugin *plug = qobject_cast<EPlugin *>( obj );
                        if( plug->inicializar() )
                        {
                                QObject::connect( obj, SIGNAL( agregarVentana( QWidget * ) ), mw->formCen(), SLOT( agregarForm( QWidget * ) ) );
                                QObject::connect( obj, SIGNAL( agregarDockWidget( Qt::DockWidgetArea, QDockWidget * ) ), mw, SLOT( agregarDock( Qt::DockWidgetArea, QDockWidget * ) ) );
                                QObject::connect( mw, SIGNAL( saliendoGestotux() ), obj, SLOT( seCierraGestotux() ) );
                                //Verifico sus tablas
                                if( plug->verificarTablas( tablas ) != true )
                                {
                                        // recordar inicailizar los resources en la funcion inicializar para qe esten disponibles los archivos sql.
                                        if( hacerTablas( plug->nombre() ) )
                                        {
                                                // todo ok
                                                qDebug( "Tablas creadas correctamente" );
                                                // Actualizo la lista de tablas para que incluya las ultimas agregadas
                                                tablas.append( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).tables( QSql::Tables ) );
                                                tablas.removeDuplicates();
                                                tablas.append( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).tables( QSql::Views ) );
                                                tablas.removeDuplicates();
                                        }
                                        else
                                        {
                                                // No se pudieron cargar las tablas
                                                qWarning( "No se pudo crear la tabla" );
                                                continue;
                                        }
                                }
                                ERegistroPlugins::getInstancia()->agregarPlugin( plug );
                                ERegistroPlugins::getInstancia()->agregarPlugin( plug->nombre(), obj );
                                splash.showMessage( "Cargando plugin "+ plug->nombre() );
                                //////////////////////////////////////////////////////////////////////////////////////
                                /*if( !tran.load(QApplication::applicationDirPath() + QDir::separator() + "traducciones" + QDir::separator() + plug->nombre() ) )
                                {  qDebug( qPrintable( "Error al cargar la traduccion de " + plug->nombre()  ) ); }
                                else {  QCoreApplication::instance()->installTranslator( &tran ); }*/
                                qDebug() << "Cargando Plugin: " << pluginsDir.absoluteFilePath( fileName );
                                // veo que tipo es para que al inicializar y cargar plugins dependientes, pueda usarse el valor
                                if( plug->tipo() == EPlugin::info && !ERegistroPlugins::getInstancia()->pluginInfoSeteado() )
                                {
                                        EInfoProgramaInterface *e = qobject_cast<EInfoProgramaInterface *>(obj);
                                        ERegistroPlugins::getInstancia()->setPluginInfo( e );
                                        preferencias *p = preferencias::getInstancia();
                                        p->setValue( "pluginInfo", plug->nombre() );
                                        p->beginGroup( "Reportes" );
                                        p->setValue( "Recibos", e->reporte( EReporte::Recibo ) );
                                        p->setValue( "Factura", e->reporte( EReporte::Factura ) );
                                        p->setValue( "Presupuesto", e->reporte( EReporte::Presupuesto ) );
                                        p->setValue( "AnulacionFactura", e->reporte( EReporte::AnulacionFactura ) );
                                        p->setValue( "Remitos", e->reporte( EReporte::Remito ) );
                                        p->endGroup();
                                } else if( plug->tipo() == EPlugin::email ) {
                                        ERegistroPlugins::getInstancia()->setearPluginEmail( qobject_cast<EInterfazEmail *>(obj) );
                                        preferencias *p = preferencias::getInstancia();
                                        p->setValue( "pluginEmail", plug->nombre() );
                                        p=0;

                                }
                        }
                        else
                        {
                                qWarning() << "Error de inicializacion en el plugin " << plug->nombre();
                        }
                 }
                 else
                 {
                        qWarning() << "Error al cargar el plugin";
                        qWarning() << loader.errorString();
                 }
             }
        /////////////////////////////////////////////////////////////////////////////////////////////////
        p->beginGroup( "General" );
        if ( !p->value( "splash", false ).toBool() )
        { splash.finish( mw ); }
        bool maximizar = p->value( "maximizado", true ).toBool();
        p->endGroup();
        p->endGroup();
        p = 0;
        // Con esto las preferencias quedan en la raiz
        mw->show();
        // Salir del programa cuando se cierren todas las ventanas
        app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );
        splash.showMessage( "Listo." );

        mw->inicializar();
        if( maximizar ) { mw->showMaximized(); }
        generarInterconexiones( mw );
        int ret = app.exec();
        QStringList list = QSqlDatabase::connectionNames();
        for(int i = 0; i < list.count(); ++i) {
            QSqlDatabase::removeDatabase(list[i]);
        }
        p = preferencias::getInstancia();
        p->inicio();
        p->setValue( "error_cierre", false );
        p->sync();
        p = 0;
        return ret;
}
