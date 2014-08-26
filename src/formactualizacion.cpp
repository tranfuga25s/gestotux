/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   *
 *   juiraze@yahoo.com.ar   *
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

#include <QFtp>
#include <QSettings>
#include "formactualizacion.h"
#include "eregistroplugins.h"
#include "preferencias.h"
#include "version.h"
#include "eplugin.h"
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QTextEdit>
#include <QDomNodeList>
#include <QDir>
#include <QDebug>

FormActualizacion::FormActualizacion(QWidget* parent, Qt::WFlags fl)
: EVentana( parent, fl ), Ui::FormActualizacionBase()
{
        setupUi(this);
        this->setAttribute( Qt::WA_DeleteOnClose );
        this->setObjectName( "actualizacion" );

        ActCerrar = new QAction( "Cerrar", this );
        ActCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
        ActCerrar->setShortcut( QKeySequence( "Ctrl+c" ) );
        ActCerrar->setToolTip( "Cierra la ventana de actualizacion ( Ctrl + c ) " );
        ActCerrar->setStatusTip( "Cierra la ventana de actualizaciones" );
        connect( ActCerrar, SIGNAL( triggered() ), this, SLOT( close() ) );

        ActIniciar = new QAction( "Iniciar", this );
        ActIniciar->setStatusTip( "Inicia la actualizacion" );
        ActIniciar->setShortcut( QKeySequence( "Ctrl+i" ) );
        ActIniciar->setIcon( QIcon( ":/imagenes/next.png" ) );
        connect( ActIniciar, SIGNAL( triggered() ), this, SLOT( iniciar() ) );

        ActDetener = new QAction( "Detener", this );
        ActDetener->setIcon( QIcon( ":/imagenes/stop.png" ) );
        ActDetener->setStatusTip( "Detiene la actualizacion" );
        ActDetener->setToolTip( "Detiene la actualizacion ( Ctrl +d )" );
        ActDetener->setShortcut( QKeySequence( "Ctrl+d" ) );
        ActDetener->setEnabled( false );
        connect( ActDetener, SIGNAL( triggered()), this, SLOT( detener() ) );


        addAction( ActIniciar );
        addAction( ActDetener );
        addAction( ActCerrar );

        this->setWindowTitle( "Actualizar el programa" );
        this->setWindowIcon( QIcon( ":/imagenes/actualizar.png" ) );
}

FormActualizacion::~FormActualizacion()
{
}


/*!
    \fn FormActualizacion::iniciar()
        Conecta las se?ales, inicia la comunicacion ftp y coloca el primer comando
 */
void FormActualizacion::iniciar()
{
 _continuar_actualizando = true;

  ftp = new QFtp( this );
  connect( ftp, SIGNAL( commandFinished( int, bool ) ), this, SLOT( terminado( int, bool ) ) );
  //connect( ftp, SIGNAL( commandStarted( int ) ), this, SLOT( inicio( int ) ) );
  connect( ftp, SIGNAL( stateChanged( int ) ), this, SLOT( cambioEstado( int ) ) );
  connect( ftp, SIGNAL( dataTransferProgress( qint64 , qint64 ) ), this, SLOT( transferencia( qint64, qint64 ) ) );
  //connect( ftp, SIGNAL( readyRead() ), this, SLOT( datosListos() ) );

  //Inicio la verificacion
  ActIniciar->setEnabled( false );
  ActDetener->setEnabled( true );
  // Busco los datos desde el registro para el host y puerto
  QSettings *p = preferencias::getInstancia();
  //QString host = p->value( "actualizaciones/host", "tranfuga.no-ip.org" ).toString();
  QString host = p->value( "Preferencias/Actualizaciones/servidor", "trafu.no-ip.org" ).toString();
  quint16 puerto = p->value( "Preferencias/Actualizaciones/puerto", 21 ).toInt();
  ftp->connectToHost( host, puerto );
}


/*!
    \fn FormActualizacion::detener()
        Coloca la variable _continuar_actualizando en falso, limpia las acciones ftp, detiene las acciones actuales y cambia los botones
 */
void FormActualizacion::detener()
{
  _continuar_actualizando = false;
  ftp->clearPendingCommands();
  ftp->close();
  ActDetener->setEnabled( false );
  ActIniciar->setEnabled( true );
  transferencia( 0, 0 );
  TELog->append( "Cancelado" );
}


/*!
    \fn FormActualizacion::finComando( int comando, bool error )
 */
void FormActualizacion::finComando( int comando, bool error )
{
 if( error )
 {
  TELog->append( QString( "Error al ejecutar el comando : %1, \n error: %2" ).arg( comando ).arg( ftp->errorString() ) );
 }
}


/*!
    \fn FormActualizacion::cambioEstado( int estado )
        Funcion llamad cada vez que la conexion ftp cambia de estado
        @param estado id de estado para colocar el mensaje
 */
void FormActualizacion::cambioEstado( int estado )
{
 switch( estado )
 {
  case QFtp::Unconnected:
  {
        TELog->append( "Se desconecto del servidor" );
        ActIniciar->setEnabled( true );
        ActDetener->setEnabled( false );
        break;
  }
  case QFtp::HostLookup:
  {
        TELog->append( "Buscando el servidor de actualizaciones" );
        break;
  }
  case QFtp::Connecting:
  {
        TELog->append( "Conectando al servidor" );
        break;
  }
  case QFtp::Connected:
  {
        TELog->append( "Conectado... ");
        break;
  }
  case QFtp::LoggedIn:
  {
        TELog->append( "Autenticando" );
        break;
  }
  case QFtp::Closing:
  {
        TELog->append( "Cerrando conexion... " );
        break;
  }
  default:
  {
        TELog->append( "Cambio al estado por default" );
        break;
  }
 }
}


/*!
    \fn FormActualizacion::inicio( int )
        Funcion llamada cada vez que se inicia un comando.
        @param id numero de comando
 */
void FormActualizacion::inicio( int id )
{
  TELog->append( " Inicio del comando " + id );
}

#include <QTemporaryFile>
/*!
    \fn FormActualizacion::terminado( int comando, bool  error )
        Funcion llamada cada vez que se finaliza un comando.
        Esta funcion ademas es la encargada de dirijir los comandos siguientes y de copiar los archivos a su ubicacion.
        @param comando numero de comando ftp
        @param error indica si el comando termino con error
 */
void FormActualizacion::terminado( int comando, bool  error )
{
  //TELog->append( QString( "Termino comando %1, error %2" ).arg( comando ).arg( error ) );
  switch( comando )
  {
   // Coneccion
   case 1:
   {
        if( !error )
        {
            if( ! preferencias::getInstancia()->value( "Preferencias/Actualizaciones/anonimo" ).toBool() ) {
                QString usuario = preferencias::getInstancia()->value( "Preferencias/Actualizaciones/usuario", "" ).toString();
                QString contra = preferencias::getInstancia()->value( "Preferencias/Actualizaciones/contra", "" ).toString();
                ftp->login( usuario, contra );
            } else {
                ftp->login();
            }
        }
        break;
   }
   // Login
   case 2:
   {
        if( !error )
        {
          QString ruta = preferencias::getInstancia()->value( "Preferencias/Actualizaciones/ruta", "actualizaciones" ).toString();
          ftp->cd( ruta );
        }
        break;
   }
   case 3:
   {
        if( !error )
        {
                ftp->get( "actualizacion.xml" );
                TELog->append( "Descargando indice" );
        }
        break;
   }
   case 4:
   {
        // Analizar el archivo
        TELog->append( "Analizando Indice...");
        analizarGeneral();
        break;
   }
   default:
   {
        if( _arch_dest.find( comando ) != _arch_dest.end() )
        {
                // Se termino de descargar el archivo o tuvo error
                QPair<QString,QString> par = _arch_dest.value( comando );
                _arch_dest.remove( comando );
                if( error )
                {
                        TELog->append( "Error al descargar el archivo " + par.first );
                        return;
                }
                TELog->append( "Se termino de descargar el archivo: " + par.first );
                QTemporaryFile archivoRecibido( this );
                archivoRecibido.open();
                archivoRecibido.write( ftp->readAll() );
                qDebug() << "Creado archivo temporal: " << archivoRecibido.fileName();
                // lo coloco en la ubicacion necesaria
                QDir dir( QApplication::applicationDirPath() );
                dir.cd( par.second );
                if( QFile::exists( dir.filePath( par.first ) ) )
                {
                        QFile::remove( dir.filePath( par.first ) );
                }
                if( QFile::copy( QDir::temp().filePath( archivoRecibido.fileName() ), dir.filePath( par.first ) ) )
                {
                        TELog->append( "Archivo actualizado correctamente" );
                }
                else
                {
                        qDebug() << "Error al copiar el archivo";
                }
        }
        else
        {
                qDebug() << "Comando desconocido: " << comando;
        }
    break;
   }
  }
}


#include <QSqlQuery>
#include <QSqlError>
/*!
    \fn FormActualizacion::analizarGeneral()
        Funcion que analiza el archivo xml de actualizaciones para comprobar las descargas
 */
void FormActualizacion::analizarGeneral()
{
  QDomDocument *docxml = new QDomDocument();
  int linea,columna; QString mensaje;
  if( !docxml->setContent( ftp->readAll(), false, &mensaje, &linea, &columna ) )
  {
   TELog->append( "Error al cargar el contenido del archivo de actualizaciones" );
   qDebug() << "Error: f:" << linea <<",c:" << columna <<"; m=" << mensaje;
   _continuar_actualizando = false;
   return;
  }
  else
  {
   TELog->append( "Descarga correcta." );
   qDebug( "Descarga Correcta" );
  }

  TELog->append( "Analizando actualizaciones disponibles" );
  QDomElement docElem = docxml->documentElement();
  qDebug() << "Primer hijo: " << docElem.tagName();
  if( docElem.tagName() == "actualizacion" )
  {
        qDebug() << "Encontrado nodo de actualizacion";
  }
  if( docElem.attribute( "version", 0 ).toDouble() > VERSION_PROGRAMA )
  {
        TELog->append( "No existen actualizaciones para esta version de Gestotux. Por Favor actualize el programa a una version superior" );
        ftp->clearPendingCommands();
        ftp->close();
        _continuar_actualizando = false;
        return;
  }
  else
  {
        TELog->append( "No se necesita actualizar el programa general." );
        //Ingreso al directorio de la version del programa
        ftp->cd( QString::number( docElem.attribute( "version", 0 ).toDouble() ) );
        qDebug() << "Entrando en: " << docElem.attribute( "version", 0 ).toDouble();

        // Busco si hay algo dentro de archivos
        /*QDomNode nodo_archivos = docxml->elementsByTagName( "archivos" ).item(0);
        if( nodo_archivos.hasChildNodes() )
        {
                qDebug( "Encontrada etiqueta de archivos generales" );
                ///\todo Esto todavia no defini como lo voy a hacer
        }*/
        qDebug() << "Encontrada version: " <<  docElem.attribute( "version", 0 );
        // Busco los plugins
        while( docElem.hasChildNodes() )
        {
                if( !_continuar_actualizando )
                { return; }
                QDomNode nodoA = docElem.firstChild();
                if( nodoA.toElement().tagName() == "plugin" )
                {
                        // Tengo instalado el plugin??
                        qDebug() << "Encontrado plugin " << nodoA.toElement().attribute( "nombre" );
                        QString nombre = nodoA.toElement().attribute( "nombre" );
                        if( ERegistroPlugins::getInstancia()->pluginsHash()->find( nombre ) == ERegistroPlugins::getInstancia()->pluginsHash()->end() )
                        {
                                qDebug() << "El plugin " <<  nombre << " no se encuentra en este sistema, no se descargara ni actualizar";
                                docElem.removeChild( nodoA );
                                continue;
                        }
                        // ingreso a la carpeta del plugin
                        ftp->cd( nombre );
                        qDebug() << "Entrando en la carpeta: " << nombre;
                        QMap<double,QDomNode> versiones;
                        // Este nodo debe tener tantos nodos como versiones disponibles
                        while( nodoA.hasChildNodes() )
                        {
                                QDomNode nodoVersion = nodoA.firstChild();
                                if( nodoVersion.toElement().tagName() == "version" )
                                {
                                        //veo que numero de version es
                                        double version  = nodoVersion.toElement().attribute( "numero" ).toDouble();
                                        qDebug() << "Encontrada version " << version;
                                        if( version >= ERegistroPlugins::getInstancia()->pluginsHash()->value( nombre )->version() )
                                        {
                                                // Lo ingreso a la lista de actualizaciones de forma ordenanda
                                                qDebug( "Version agregada" );
                                                versiones.insert( version, nodoVersion );
                                                nodoA.removeChild( nodoVersion );
                                        }
                                        else
                                        {
                                                // actualizacion vieja, la elimino del arbol
                                                nodoA.removeChild( nodoVersion );
                                                continue;
                                        }
                                }
                                else
                                {
                                        // No puede haber de otro tipo, lo elimino
                                        qDebug( "Encontrado nodo que no es version" );
                                        nodoA.removeChild( nodoVersion );
                                }
                        }
                        // Ejecuto las actualizaciones de forma ordenada
                        qDebug( "Ordenando versiones" );
                        QList<double> lista = versiones.keys();
                        qStableSort( lista.begin(), lista.end() );
                        if( lista.size() == 0 )
                        {
                                qDebug( "La lista de actualizaciones esta vacia" );
                        }
                        while( lista.size() > 0 )
                        {
                                QDomNode nodoB = versiones[lista.first()];
                                // Trabajo con el nodo
                                // Busco los hijos que son archivos
                                TELog->append( QString( "Actualizando plugin %1..." ).arg( nombre ) );
                                // Ingreso al directorio de la version del plugin
                                ftp->cd( QString::number( lista.first() ) );
                                #ifdef Q_WS_WIN32
                                QString nombre_os = "windows";
                                #endif
                                #ifdef Q_WS_X11
                                QString nombre_os = "linux";
                                #endif
                                QDomNode nodo_os = nodoB.toElement().elementsByTagName( nombre_os ).item(0);
                                qDebug() << "Nodo OS: " << nodo_os.nodeName();
                                QDomNodeList nodos_archivos = nodo_os.toElement().elementsByTagName( "archivo" );
                                unsigned int posNodo = 0;
                                qDebug() << "Encontrado " << nodos_archivos.length() << " nodos ";
                                while( posNodo < nodos_archivos.length() && _continuar_actualizando )
                                {
                                        QDomNode nodo_archivo = nodos_archivos.item(posNodo);
                                        QPair<QString,QString> tmp;
                                        tmp.first = nodo_archivo.toElement().attribute( "nombre" );
                                        tmp.second = nodo_archivo.toElement().attribute( "directorio_destino" );
                                        qDebug( QString( "Encontrado archivo %1, dir %2" ).arg( tmp.first ).arg( tmp.second ).toLocal8Bit() );
                                        TELog->append( QString( "Descargando archivo %1..." ).arg( tmp.first ) );
                                        int pos = ftp->get( tmp.first );
                                        _arch_dest.insert( pos, tmp );
                                        posNodo++;
                                }
                                //Veo si hay actualizaciones de la base de datos
                                qDebug( "Actualizaciones de base de datos" );
                                QDomNodeList nodos_db = nodoB.toElement().elementsByTagName( "db" );
                                if( nodos_db.length() > 0 && _continuar_actualizando )
                                {
                                        for( unsigned int i=0; i<nodos_db.length(); i++ )
                                        {
                                                if( !_continuar_actualizando )
                                                { return; }
                                                QDomNode nodo = nodos_db.item(i);
                                                // Busco todos los hijos
                                                QDomNodeList nodos_colas = nodo.toElement().elementsByTagName( "cola" );
                                                if( nodos_colas.length() > 0 && _continuar_actualizando )
                                                {
                                                        for( unsigned int j=0; j < nodos_colas.length(); j++ )
                                                        {
                                                                if( !_continuar_actualizando )
                                                                { return;}
                                                                QDomNode nCola = nodos_colas.item(j);
                                                                if( nCola.nodeName() == "cola" )
                                                                {
                                                                        QSqlQuery cola;
                                                                        if( cola.exec( nCola.firstChild().toText().data() ) )
                                                                        {
                                                                                qDebug( QString( "Cola ejecutada correctamente: %1" ).arg( cola.executedQuery() ).toLocal8Bit() );
                                                                        }
                                                                        else
                                                                        {
                                                                                qWarning( QString( "La ejecucion de la actualizacion no fue correcta. Cola: %1" ).arg( cola.executedQuery() ).toLocal8Bit() );
                                                                                qDebug( QString( "Error: %1.\n Cola: %2" ).arg( cola.lastError().text() ).arg( cola.executedQuery() ).toLocal8Bit() );
                                                                        }
                                                                }
                                                                else
                                                                {
                                                                        qDebug( QString("Nodo encontrado: %1").arg(nodo.nodeName() ).toLocal8Bit() );
                                                                }
                                                        } // Fin for colas
                                                }// Fin if nodos_colas
                                        }// Fin for dbs
                                }
                                else
                                {
                                        qDebug( "No hay actualizaciones para la base de datos" );
                                }
                                //////////////////////// Fin de trabajar con el nodo
                                versiones.remove(lista.first());
                                lista.removeFirst();
                                // Salgo del directorio de la version y quedo en el directorio del plugin
                                ftp->cd("..");
                        }
                        qDebug( "Fin bucle Versiones" );
                        // Termino de actualizar el plugin y sus versiones -> salgo al directorio de la version del programa
                        ftp->cd("..");
                        docElem.removeChild( nodoA );
                }
                else if( nodoA.toElement().tagName() == "libreria" )
                {
                        ftp->cd( "librerias" );
                        // Veo el numero de secuencia
                        int num_seq  = preferencias::getInstancia()->value( "Preferencias/General/"+nodoA.toElement().attribute("nombre" ) + "/numseq", 0 ).toInt();
                        int num_nuevo = nodoA.toElement().attribute( "numerosecuencia" ).toInt();
                        if( num_seq <= num_nuevo )
                        {
                                ftp->cd( QString::number( num_nuevo ) );
                                #ifdef Q_WS_WIN32
                                QString nombre_os = "windows";
                                #endif
                                #ifdef Q_WS_X11
                                QString nombre_os = "linux";
                                #endif
                                QDomNode nodo_os = nodoA.toElement().elementsByTagName( nombre_os ).item(0);
                                qDebug( QString( "Nodo OS: %1" ).arg( nodo_os.nodeName() ).toLocal8Bit() );
                                QDomNodeList nodos_archivos = nodo_os.toElement().elementsByTagName( "archivo" );
                                unsigned int posNodo = 0;
                                qDebug( QString( "Encontrado %1 nodos").arg( nodos_archivos.length() ).toLocal8Bit() );
                                while( posNodo < nodos_archivos.length() && _continuar_actualizando )
                                {
                                        QDomNode nodo_archivo = nodos_archivos.item(posNodo);
                                        QPair<QString,QString> tmp;
                                        tmp.first = nodo_archivo.toElement().attribute( "nombre" );
                                        tmp.second = nodo_archivo.toElement().attribute( "directorio_destino" );
                                        qDebug( QString( "Encontrado archivo %1, dir %2" ).arg( tmp.first ).arg( tmp.second ).toLocal8Bit() );
                                        TELog->append( QString( "Descargando archivo %1..." ).arg( tmp.first ) );
                                        int pos = ftp->get( tmp.first );
                                        _arch_dest.insert( pos, tmp );
                                        posNodo++;
                                }
                        }
                        else
                        {
                                // La libreria no necesita actualizacion
                        }
                        ftp->cd("..");
                        //Fin de actualizar la libreria -> regreso a la carpeta de version del programa
                }
                else
                {
                        // El nodo no es plugin
                        /// \todo ver que hacer aca
                        qDebug( QString( "Tipo de nodo desconocido: %1" ).arg( nodoA.toElement().tagName() ).toLocal8Bit() );
                        docElem.removeChild( nodoA );
                }
        } // fin de si actualizacion tiene nodos
  }
 ftp->close();
 TELog->append( "Lista el Analisis" );
 transferencia( 100, 100 );
 qDebug( "Fin" );
}


/*!
    \fn FormActualizacion::transferencia( qint64 echo, qint64 total )
        Funcion llamada cada vez que se actualiza la barra de progreso
        @param echo cantidad de datos transmitida
        @param total cantidad total de datos a transmitir
 */
void FormActualizacion::transferencia( qint64 echo, qint64 total )
{
 //qDebug( QString("transferencia %1 de %2 ").arg(echo).arg(total).toLocal8Bit() );
 PBProgreso->setRange( 0, total );
 PBProgreso->setValue( echo );
}
