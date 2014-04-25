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
#include "admincarcorr.h"

#include <QSqlQuery>
#include <QDir>
#include <QHash>

QHash<QString, EInformeInterface *> *AdminCarCorr::_plugins = 0;

QIcon AdminCarCorr::iconoPrograma() const
{
 return QIcon();
}

QString AdminCarCorr::directorioActualizaciones() const
{
 return "actualizaciones";
}

QString AdminCarCorr::directorioBackup() const
{
 return "admCarCorr";
}

QString AdminCarCorr::nombrePrograma() const
{
 return "Administracion de Caravanas";
}

QList<QActionGroup *> AdminCarCorr::accionesBarra()
{
 QList<QActionGroup*> lista;
 QActionGroup *GTris = new QActionGroup( this );
 GTris->setObjectName( "gtris" );
 GTris->setProperty( "icono", "" );
 GTris->setProperty( "titulo", "Manejo de TRIs" );
 GTris->addAction( ActAgregarCompra );
 GTris->addAction( ActAgregarMudanza );
 GTris->addAction( ActAgregarStock );
 GTris->addAction( ActAgregarVenta );
 lista.append( GTris );
 QActionGroup *resumenes = new QActionGroup( this );
 resumenes->setObjectName( "resumen" );
 resumenes->setProperty( "titulo", "Resumenes" );
 resumenes->setProperty( "icono", ":/imagenes/resumen.png" );
 if( !plugins().isEmpty() )
 {
 	foreach( EInformeInterface *p, plugins() )
	{
		if( !p->accionesBarra().isEmpty() )
		{
			QAction *ac;
			foreach( ac, p->accionesBarra() )
			{
				resumenes->addAction( ac );
			}
		}

	}
 }
 lista.append( resumenes );
 return lista;
}

QString AdminCarCorr::nombre() const
{
 return "admincaravanascorrientes";
}

#include "formprefcaravanas.h"

QWidgetList AdminCarCorr::formsPreferencias()
{
 QWidgetList _listaFormPref;
 _listaFormPref.append( new FormPrefCaravanas() );
 return _listaFormPref;
}

#include <QApplication>
#include <QDate>
#include "preferencias.h"

bool AdminCarCorr::inicializar()
{
 _acciones.clear();
 // Verifico el uso
 /*if( !preferencias::getInstancia()->contains( "comprado" ) )
 {
  preferencias::getInstancia()->setValue( "comprado", true );
  preferencias::getInstancia()->setValue( "fechas_informes", QDate::currentDate() );
 }
 else
 {
  QDate fecha = preferencias::getInstancia()->value( "fechas_informes", QDate() ).toDate();
  if( !fecha.isValid() || fecha.daysTo( QDate::currentDate() ) > 29 )
  {
    qWarning( "Error de validación de licencia. Su tiempo de prueba ha expirado" );
    return false;
  }
  else
  {
   qWarning( QString( "Le quedan %1 dias para pruebas" ).arg( fecha.daysTo( QDate::currentDate() ) ).toLocal8Bit() );
  }
 }*/

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

 ActModificarTri = new QAction( "Modificar Tri", this );
 ActModificarTri->setToolTip( "Permite modificar un numero especifico de tri" );
 connect( ActModificarTri, SIGNAL( triggered() ), this, SLOT( modificarTri() ) );

 ActEliminarTri = new QAction( "Eliminar Tri", this );
 ActEliminarTri->setToolTip( "Elimina un tri especifico" );
 connect( ActEliminarTri, SIGNAL( triggered() ), this, SLOT( eliminarTri() ) );

 _acciones.append( ActAgregarVenta );
 _acciones.append( ActAgregarCompra );
 _acciones.append( ActAgregarMudanza );
 _acciones.append( ActAgregarStock );

 cargarPluginsInformes();
 return true;
}

bool AdminCarCorr::verificarTablas()
{
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_categorias" ) )
 { qWarning( "Error al buscar la tabla car_categorias" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_establecimientos" ) )
 { qWarning( "Error al buscar la tabla car_establecimientos" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_caravana" ) )
 { qWarning( "Error al buscar la tabla car_caravana" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_caravana" ) )
 { qWarning( "Error al buscar la tabla car_caravana" );	return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_tri" ) )
 { qWarning( "Error al buscar la tabla car_tri" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_carv_tri" ) )
 { qWarning( "Error al buscar la tabla car_carv_tri" ); return false; }
 if( !QSqlDatabase::database().tables( QSql::Tables ).contains( "car_carv_duenos" ) )
 { qWarning( "Error al buscar la tabla car_carv_duenos" ); return false; }
 return true;
}

double AdminCarCorr::version() const
{
 return 0.11;
}

int AdminCarCorr::tipo() const
{
 return EPlugin::info;
}

void AdminCarCorr::crearMenu( QMenuBar* m )
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
  menuHer->addSeparator();
  menuHer->addAction( ActAgregarCompra );
  menuHer->addAction( ActAgregarMudanza );
  menuHer->addAction( ActAgregarVenta );
  menuHer->addAction( ActAgregarStock );
  menuHer->addSeparator();
  menuHer->addAction( ActModificarTri );
  menuHer->addAction( ActEliminarTri );
 }
 // Creo el menu de informes
 if( !plugins().isEmpty() )
 {
 	QMenu *menuInformes = m->addMenu( "Informes" );
 	menuInformes->setObjectName( "menuInformes" );
	foreach( EInformeInterface *p, plugins() )
	{
		p->crearMenu( menuInformes );
	}
 }
}

Q_EXPORT_PLUGIN2(admincaravanascorrientes, AdminCarCorr );

#include "vcategorias.h"
/*!
    \fn AdminCarCorr::verCategorias()
 */
void AdminCarCorr::verCategorias()
{
 emit agregarVentana( new VCategorias() );
}


#include "vestablecimiento.h"
/*!
    \fn AdminCarCorr::verEstablecimientos()
 */
void AdminCarCorr::verEstablecimientos()
{
 emit agregarVentana( new VEstablecimiento() );
}

#include "formagregar.h"
/*!
    \fn AdminCarCorr::agregarCompra()
 */
void AdminCarCorr::agregarCompra()
{
 emit agregarVentana( new FormAgregar() );
}

#include "formmudanza.h"
/*!
    \fn AdminCarCorr::hacerMovimiento()
 */
void AdminCarCorr::hacerMovimiento()
{
 emit agregarVentana( new FormMudanza() );
}

#include "formventa.h"
/*!
    \fn AdminCarCorr::hacerVenta()
 */
void AdminCarCorr::hacerVenta()
{
 emit agregarVentana( new FormVenta() );
}


#include "formmudanza.h"
/*!
    \fn AdminCarCorr::hacerMudanza()
 */
void AdminCarCorr::hacerMudanza()
{
 emit agregarVentana( new FormMudanza() );
}


/*!
    \fn AdminCarCorr::cargarPluginsInformes()
	Carga los plugins de informes que se encuentren activos y se verifiquen
 */
void AdminCarCorr::cargarPluginsInformes()
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
    \fn AdminCarCorr::plugins()
	Devuelve una lista de punteros a todos los plugins que tiene cargados
	@return QList de EInformeInterface * de todos los plugins
 */
QList<EInformeInterface *> AdminCarCorr::plugins()
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


/*!
    \fn AdminCarCorr::empresa() const
	Devuleve el nombre de la empresa para el dialogo de Acerca de
	@return Nombre de la empresa
 */
QString AdminCarCorr::empresa() const
{
 return "Administracion Corrientes";
}

#include "formstock.h"
void AdminCarCorr::agregarStock()
{
 emit agregarVentana( new FormStock() );
}


/*!
    \fn AdminCarCorr::companeros()
 */
QString AdminCarCorr::companeros()
{
 return "Exequiel Saavedra <gandhalf@gmail.com> \n Pruebas y graficos";
}

#include <QSqlRecord>
#include <QInputDialog>
#include "formmodificartri.h"
/*!
   \fn AdminCarCorr::modificarTri()
	Slot para modificar un tri.
        Muestra una ventana para elegir el numero de tri, y muestra luego el formulario de modificacion
*/
void AdminCarCorr::modificarTri()
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
    \fn AdminCarCorr::eliminarTri()
	Slot de eliminacion de tri, muestra la ventana para elegir cual eliminar.
 */
void AdminCarCorr::eliminarTri()
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


/*!
    \fn AdminCarCorr::crearToolBar( QToolBar *t )
 */
void AdminCarCorr::crearToolBar( QToolBar *t )
{
 t->addActions( _acciones );
}


/*!
    \fn AdminCarCorr::seCierraGestotux()
 */
void AdminCarCorr::seCierraGestotux()
{
 qDebug( "Cerrando ACarCorr" );
}
