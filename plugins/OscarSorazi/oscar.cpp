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
#include "oscar.h"

#include "../aCarCorr/einformeinterface.h"
#include <QSqlQuery>

QSettings *Oscar::_pref = 0;
QHash<QString, EInformeInterface *> *Oscar::_plugins = 0;


QIcon Oscar::iconoPrograma() const
{
 return QIcon();
}

QString Oscar::companeros()
{
 return "Exequiel Saavedra <gandhalf@gmail.com> \n Pruebas y graficos";
}

QString Oscar::directorioActualizaciones() const
{
 return "oscar";
}

QString Oscar::directorioBackup() const
{
 return "oscar";
}

QString Oscar::empresa() const
{
 return "Oscar Sorazi";
}

QString Oscar::nombrePrograma() const
{
 return "Oscar Sorazi";
}

QList< QAction * > Oscar::accionesBarra() const
{
  return _acciones;
}

QString Oscar::nombre() const
{
 return "oscarsoraiz";
}

QWidgetList Oscar::formsPreferencias()
{
 return QWidgetList();
}

bool Oscar::inicializar( QSettings* pref)
{
 _pref = pref;
 _acciones.clear();

 ActCategoria = new QAction( "Ver Categorias", this );
 ActCategoria->setIcon( QIcon( ":/imagenes/categoria.png" ) );
 ActCategoria->setStatusTip( "Muestra el listado de categorias de caravanas" );
 ActCategoria->setToolTip( "Muestra las categorias ( Ctrl + u )" );
 ActCategoria->setShortcut( QKeySequence( "Ctrl+u" ) );
 connect( ActCategoria, SIGNAL( triggered() ), this, SLOT( verCategorias() ) );

 ActEstablecimiento = new QAction( "Ver Establecimientos", this );
 ActEstablecimiento->setIcon( QIcon( ":/imagenes/establecimientos.png" ) );
 ActEstablecimiento->setStatusTip( "Muestra el listado de establecimientos" );
 ActEstablecimiento->setToolTip( "Muestra los establecimientos ( Ctrl + e )" );
 ActEstablecimiento->setShortcut( QKeySequence( "Ctrl+e" ) );
 connect( ActEstablecimiento, SIGNAL( triggered() ), this, SLOT( verEstablecimientos() ) );

 ActAgregarCompra = new QAction( "Agregar compra", this );
 ActAgregarCompra->setIcon( QIcon( ":/imagenes/add.png" ) );
 ActAgregarCompra->setStatusTip( "Agrega una nueva compra" );
 ActAgregarCompra->setToolTip( "Agrega una nueva compra" );
 connect( ActAgregarCompra, SIGNAL( triggered() ), this, SLOT( agregarCompra() ) );

 ActAgregarVenta = new QAction( "Nueva venta", this );
 ActAgregarVenta->setIcon( QIcon( ":/imagenes/add.png" ) );
 ActAgregarVenta->setToolTip( "Agregar nueva venta de animales" );
 connect( ActAgregarVenta, SIGNAL( triggered() ), this, SLOT( hacerVenta() ) );

 ActAgregarMudanza = new QAction( "Nuevo Traslado", this );
 ActAgregarMudanza->setIcon( QIcon( ":/imagenes/add.png" ) );
 ActAgregarMudanza->setToolTip( "Agrega un nuevo tri de mudanza" );
 connect( ActAgregarMudanza, SIGNAL( triggered() ), this, SLOT( hacerMudanza() ) );

 ActAgregarStock = new QAction( "Ingreso x stock", this );
 ActAgregarStock->setIcon( QIcon( ":/imagenes/add.png" ) );
 ActAgregarStock->setToolTip( "Agregar Caravanas a la existencia" );
 connect( ActAgregarStock, SIGNAL( triggered() ), this, SLOT( agregarStock() ) );

 ActDuenos = new QAction( "Dueños", this );
 ActDuenos->setToolTip( "Muestra el listado de personas que estan como dueños" );
 connect( ActDuenos, SIGNAL( triggered() ), this, SLOT( mostrarDuenos() ) );

 ActModificarTri = new QAction( "Modificar Tri", this );
 ActModificarTri->setToolTip( "Permite modificar un numero especifico de tri" );
 connect( ActModificarTri, SIGNAL( triggered() ), this, SLOT( modificarTri() ) );

 ActEliminarTri = new QAction( "Modificar Tri", this );
 ActEliminarTri->setToolTip( "Elimina un tri de la lista" );
 connect( ActEliminarTri, SIGNAL( triggered() ), this, SLOT( eliminarTri() ) );

 _acciones.append( ActAgregarVenta );
 _acciones.append( ActAgregarCompra );
 _acciones.append( ActAgregarMudanza );
 _acciones.append( ActAgregarStock );
 _acciones.append( ActDuenos );

 cargarPluginsInformes();
 return verificarTablas();
}

bool Oscar::verificarTablas()
{
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_categorias" ) )
 { qWarning( "Error al buscar la tabla car_categorias" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_establecimientos" ) )
 { qWarning( "Error al buscar la tabla car_establecimientos" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_caravana" ) )
 { qWarning( "Error al buscar la tabla car_caravana" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_tri" ) )
 { qWarning( "Error al buscar la tabla car_tri" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_carv_tri" ) )
 { qWarning( "Error al buscar la tabla car_carv_tri" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_carv_duenos" ) )
 { qWarning( "Error al buscar la tabla car_carv_duenos" ); return false; }
 return true;
}

double Oscar::version() const
{
 return 0.1;
}

int Oscar::tipo() const
{
 return EPlugin::info;
}

void Oscar::crearMenu(QMenuBar* m)
{
  QMenu *menuHer = m->findChild<QMenu *>( "menuHerramientas" );
 if( menuHer == 0 )
 {
  qDebug( "Error en las baras de menu" );
 }
 else
 {
  menuHer->addAction( ActEstablecimiento );
  menuHer->addAction( ActCategoria );
  menuHer->addAction( ActDuenos );
  menuHer->addSeparator();
  menuHer->addAction( ActAgregarCompra );
  menuHer->addAction( ActAgregarMudanza );
  menuHer->addAction( ActAgregarVenta );
  menuHer->addAction( ActAgregarStock );
  menuHer->addSeparator();
  menuHer->addAction( ActEliminarTri );
 }
 if( !plugins().isEmpty() )
 {
	// Creo el menu de informes
	QMenu *menuInformes = m->addMenu( "Informes" );
	menuInformes->setObjectName( "menuInformes" );
	foreach( EInformeInterface *p, plugins() )
	{
		p->crearMenu( menuInformes );
	}
 }
}

Q_EXPORT_PLUGIN2( oscarsoraiz , Oscar );

#include "../aCarCorr/vcategorias.h"
void Oscar::verCategorias()
{ emit agregarVentana( new VCategorias() ); }


#include "../aCarCorr/vestablecimiento.h"
/*!
    \fn Oscar::verEstablecimientos()
 */
void Oscar::verEstablecimientos()
{ emit agregarVentana( new VEstablecimiento() ); }

#include "../aCarCorr/formagregar.h"
/*!
    \fn Oscar::agregarCompra()
 */
void Oscar::agregarCompra()
{ emit agregarVentana( new FormAgregar() ); }

#include "../aCarCorr/formmudanza.h"
/*!
    \fn Oscar::hacerMovimiento()
 */
void Oscar::hacerMovimiento()
{ emit agregarVentana( new FormMudanza() ); }

#include "../aCarCorr/formventa.h"
/*!
    \fn Oscar::hacerVenta()
 */
void Oscar::hacerVenta()
{ emit agregarVentana( new FormVenta() ); }


#include "../aCarCorr/formmudanza.h"
/*!
    \fn Oscar::hacerMudanza()
 */
void Oscar::hacerMudanza()
{ emit agregarVentana( new FormMudanza() ); }


#include <QDir>
void Oscar::cargarPluginsInformes()
{
loader = new QPluginLoader( this );
 // Busco los plugins de presupuestos
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
     if( !pluginsDir.exists( "informes" ) ) {  return; }
     pluginsDir.cd("informes");
     if( !pluginsDir.exists() ) { return; }

	_plugins = new QHash<QString, EInformeInterface *>();
	QStringList filtro;
#ifdef Q_WS_WIN32
	filtro.append( "*.dll" );
#endif
#ifdef Q_WS_X11
	filtro.append( "*so" );
#endif
	if( pluginsDir.entryList( filtro, QDir::Files ).isEmpty() ) { return; }
     foreach( QString fileName, pluginsDir.entryList( filtro, QDir::Files ) )
     {

	loader->setFileName( pluginsDir.absoluteFilePath( fileName ) );
        if( loader->load() )
        {
		QObject *obj = loader->instance();
		EInformeInterface *plug = qobject_cast<EInformeInterface *>( obj );
		// Genero ahora el visor de informe
		if( plug->inicializar() )
		{
			connect( obj, SIGNAL( agregarVentana( QWidget * ) ), this, SIGNAL( agregarVentana( QWidget * ) ) );
			_plugins->insert( plug->nombre(), plug );
			qDebug( QString( "Cargando Plugin: %1" ).arg( pluginsDir.absoluteFilePath( fileName )).toLocal8Bit() );
		}
		else
		{
			qWarning( QString( "Error de inicializacion en el plug in %1" ).arg( plug->nombre() ).toLocal8Bit() );
		}
	}
	else
	{
		qWarning( QString( "Error al cargar el plugin. Error: %1" ).arg( loader->errorString() ).toLocal8Bit() );
	}
	// Fin de la carga del plugin
  }
}


/*!
    \fn Oscar::plugins()
 */
QList<EInformeInterface *> Oscar::plugins()
{
 if( _plugins == 0 )
 {
  QList<EInformeInterface *> lista;
  return lista;
 }
 else
 {
  return _plugins->values();
 }
}


#include "vduenos.h"
void Oscar::mostrarDuenos()
{ emit agregarVentana( new VDuenos() ); }

#include "../aCarCorr/formstock.h"
void Oscar::agregarStock()
{emit agregarVentana( new FormStock( ) ); }


#include <QSqlRecord>
#include <QInputDialog>
#include "../aCarCorr/formmodificartri.h"
void Oscar::modificarTri()
{
// Pregunto que numero de tri quiere modificar
 bool ok;
 QStringList lista;
 QSqlQuery cola( "SELECT id_tri FROM car_tri" );
 while( cola.next() )
 {
  lista.append( cola.record().value(0).toString() );
 }
 QString id_tri = QInputDialog::getItem( 0, tr("Elija el tri"), tr("# Tri :"), lista, 0, false, &ok );
 if( !id_tri.isNull() && ok )
 {
  // Cargo el formulario con el tri que corresponda
  if( cola.exec( QString( "SELECT razon FROM car_tri WHERE id_tri = '%1'" ).arg( id_tri ) ) )
  {
   if( cola.next() )
   {
     emit agregarVentana( new FormModificarTri( 0, cola.record().value("razon").toInt(), id_tri.toInt() )  );
     return;
   }
   else
   {
     qWarning( "Error al buscar el tipo de movimiento" );
     return;
   }
  }
  else
  {  qWarning( "Error al ejecutar la cola de busqueda de tipo de movimiento" ); }
 }
 else
 {
  return;
 }
}


#include <QMessageBox>

/*!
    \fn Oscar::eliminarTri()
 */
void Oscar::eliminarTri()
{
  // Pregunto que numero de tri quiere modificar
 bool ok;
 QStringList lista;
 QSqlQuery cola( "SELECT id_tri FROM car_tri" );
 while( cola.next() )
 {
  lista.append( cola.record().value(0).toString() );
 }
 QString id_tri = QInputDialog::getItem( 0, tr("Elija el tri"), tr("# Tri :"), lista, 0, false, &ok );
 if( !id_tri.isNull() && ok )
 {
	EMovimiento *movimiento = new EMovimiento();
	if( movimiento->eliminarTRI( id_tri.toInt() ) )
	{
		QMessageBox::information( 0, "TRI eliminado", "El tri seleccionado ha sido eliminado" );
	}
	else
	{
		qWarning( "No se pudo eliminar el tri." );
	}
	delete movimiento;
 }
}
