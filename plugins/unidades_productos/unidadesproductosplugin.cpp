#include "unidadesproductosplugin.h"

/*!
    \fn UnidadesProductosPlugin::accionesBarra()
 */
QList<QActionGroup *> UnidadesProductosPlugin::accionesBarra()
{
 QList<QActionGroup *> lista;
 return lista;
}


/*!
    \fn UnidadesProductosPlugin::nombre() const
 */
QString UnidadesProductosPlugin::nombre() const
{
  return "unidades_productos";
}

#include "formprefunidadesproductos.h"
/*!
    \fn UnidadesProductosPlugin::formsPreferencias()
 */
QWidgetList UnidadesProductosPlugin::formsPreferencias()
{
    QWidgetList lista;
    lista.append( new FormPrefUnidadesProductos() );
    return lista;
}


/*!
    \fn UnidadesProductosPlugin::inicializar()
 */
bool UnidadesProductosPlugin::inicializar()
{
 Q_INIT_RESOURCE(unidades_productos);

 /*ActAgregarRemito = new QAction( "Agregar remito", this );
 ActAgregarRemito->setIcon( QIcon( ":/imagenes/remito-nuevo.png" ) );
 ActAgregarRemito->setStatusTip( "Muestra el listado de remitos emitidos" );
 connect( ActAgregarRemito, SIGNAL( triggered() ), this, SLOT( agregarNuevoRemito() ) );

 ActVerRemitos = new QAction( "Listado de remitos", this );
 ActVerRemitos->setIcon( QIcon( ":/imagenes/remito.png" ) );
 ActVerRemitos->setStatusTip( "Permite ver las facturas emitidas y anularlas." );
 connect( ActVerRemitos, SIGNAL( triggered() ), this, SLOT( verRemitos() ) );*/

 return true;
}


/*!
    \fn UnidadesProductosPlugin::verificarTablas( QStringList tablas )
 */
bool UnidadesProductosPlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "unidades_productos" ) )
 { qDebug( "UnidadesProductosPlugin::Error al buscar la tabla unidades_productos" ); return false; }
 if( !tablas.contains( "productos_unidades" ) )
 { qDebug( "UnidadesProductosPlugin::Error al buscar la tabla productos_unidades" ); return false; }
 /*if( !tablas.contains( "remito" ) )
 { qDebug( "UnidadesProductosPlugin::Error al buscar la tabla remito" ); return false; }
 if( !tablas.contains( "item_remito" ) )
 { qDebug( "UnidadesProductosPlugin::Error al buscar la tabla item_remito" ); return false; }
 if( !tablas.contains( "v_remito" ) )
 { qDebug( "UnidadesProductosPlugin::Error al buscar la vista item_remito" ); return false; }*/
 return true;
}


/*!
    \fn UnidadesProductosPlugin::tipo() const
 */
int UnidadesProductosPlugin::tipo() const
{
 return EPlugin::comun;
}


/*!
    \fn UnidadesProductosPlugin::crearMenu( QMenuBar *m )
 */
void UnidadesProductosPlugin::crearMenu( QMenuBar *m )
{
 /*QMenu *mVentas = m->addMenu( "Remitos" );
 mVentas->addAction( ActAgregarRemito );
 mVentas->addAction( ActVerRemitos );*/
}


/*!
    \fn UnidadesProductosPlugin::version() const
 */
double UnidadesProductosPlugin::version() const
{
 return 0.02;
}

void UnidadesProductosPlugin::crearToolBar( QToolBar */*t*/ )
{}


/*!
    \fn UnidadesProductosPlugin::seCierraGestotux()
 */
void UnidadesProductosPlugin::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(unidades_productos);
 return;
}

Q_EXPORT_PLUGIN2( unidades_productos, UnidadesProductosPlugin )