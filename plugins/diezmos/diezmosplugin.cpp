#include "diezmosplugin.h"

Q_EXPORT_PLUGIN2( diezmos, DiezmosPlugin )

bool DiezmosPlugin::inicializar()
{
 Q_INIT_RESOURCE(diezmos);
 /////////////////////////////////////
 // Muestra los gastos
 /////////////////////////////////////
 ActDiezmos = new QAction( "Diezmos", this );
 ActDiezmos->setStatusTip( "Ver la cuenta corriente de diezmos" );
 ActDiezmos->setIcon( QIcon( ":/imagenes/diezmo.jpg" ) );
 connect( ActDiezmos, SIGNAL( triggered() ), this, SLOT( ver_diezmos() ) );
 /////////////////////////////////////
 // Muestra el agregar un gasto
 /////////////////////////////////////
 ActDarDiezmo = new QAction( "Agregar Gasto", this );
 ActDarDiezmo->setStatusTip( "Agregar nuevo gasto" );
 ActDarDiezmo->setIcon( QIcon( ":/imagenes/gasto.jpg" ) );
 connect( ActDarDiezmo, SIGNAL( triggered() ), this, SLOT( darDiezmo() ) );

 return true;
}

bool DiezmosPlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "diezmos" ) )
 { qWarning( "DiezmosPlugin::Error al buscar la tabla diezmos" ); return false; }
 return true;
}

double DiezmosPlugin::version() const
{
 return 0.1;
}

int DiezmosPlugin::tipo() const
{
 return EPlugin::comun;
}

QList< QActionGroup * > DiezmosPlugin::accionesBarra()
{
 /*QList<QActionGroup *> lista;
 QActionGroup *gastos = new QActionGroup( this );
 gastos->setObjectName( "Gastos" );
 gastos->addAction( ActAgregarGasto );
 lista.append( Gastos );
 return lista;*/
 return QList<QActionGroup *>();
}

QString DiezmosPlugin::nombre() const
{
 return "diezmos";
}

QWidgetList DiezmosPlugin::formsPreferencias()
{
 return QWidgetList();
}

void DiezmosPlugin::crearMenu(QMenuBar* m)
{
 QMenu *menuHerramientas = m->findChild<QMenu *>( "menuHerramientas" );
 if( menuHerramientas == 0 )
 {
  qDebug( "Error en las baras de menu" );
 }
 else
 {
  QMenu *menuDiezmos = menuHerramientas->addMenu( "Diezmos" );
  menuDiezmos->addAction( ActDarDiezmo );
  menuDiezmos->addAction( ActDiezmos );
 }
}

void DiezmosPlugin::crearToolBar(QToolBar*)
{}

#include "ddardiezmo.h"
/*!
    \fn DiezmosPlugin::darDiezmo()
 */
void DiezmosPlugin::darDiezmo()
{
    DDarDiezmo *d =  new DDarDiezmo();
    d->exec();
}

#include "vdiezmos.h"
/*!
    \fn DiezmosPlugin::ver_diezmos()
    Muestra el listado de gastos que haya cargado
 */
void DiezmosPlugin::ver_diezmos()
{ emit agregarVentana( new VDiezmos() ); }


/*!
    \fn DiezmosPlugin::seCierraGestotux()
 */
void DiezmosPlugin::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(diezmos); return; }
