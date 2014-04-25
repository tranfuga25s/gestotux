#include "credencialesplugin.h"
bool credencialesplugin::inicializar()
{
 Q_INIT_RESOURCE(credenciales);

    ActNuevaCredencial = new QAction( this );
    ActNuevaCredencial->setText( "Nueva credencial" );
    connect( ActNuevaCredencial, SIGNAL( triggered() ), this, SLOT( crearcredencial() ) );

    ActEquipos = new QAction( this );
    ActEquipos->setText( "Equipos" );
    connect( ActEquipos, SIGNAL( triggered() ), this, SLOT( verEquipos() ) );

    ActCredenciales = new QAction( this );
    ActCredenciales->setText( "Credenciales" );
    connect( ActCredenciales, SIGNAL( triggered() ), this, SLOT( verCredenciales() ) );

 return true;
}

bool credencialesplugin::verificarTablas( QStringList tablas )
{
    if( !tablas.contains( "equipos" ) ) {
        qDebug( "Tabla de equipos no encontradas" );  return false;
    }
    if( !tablas.contains( "credenciales" ) ) {
        qDebug( "Tabla de credenciales no encontradas" ); return false;
    }
    return true;
}

double credencialesplugin::version() const
{ return 0.1; }

int credencialesplugin::tipo() const
{
 return EPlugin::info;
}

QList< QActionGroup * > credencialesplugin::accionesBarra()
{ return QList<QActionGroup*>(); }

QString credencialesplugin::nombre() const
{ return "credencialesplugin"; }

QWidgetList credencialesplugin::formsPreferencias()
{ return QWidgetList(); }

void credencialesplugin::crearMenu( QMenuBar* m )
{
   QMenu *menuHer = m->findChild<QMenu *>( "menuHerramientas" );
   menuHer->addAction( ActNuevaCredencial );
   menuHer->addAction( ActEquipos );
   menuHer->addAction( ActCredenciales );
}

void credencialesplugin::crearToolBar(QToolBar* /*t*/)
{}

void credencialesplugin::seCierraGestotux()
{
    Q_CLEANUP_RESOURCE(credenciales);
}

QImage credencialesplugin::imagenPrograma() const
{ return QImage( ":/imagenes/logoBS.png" ); }

QString credencialesplugin::nombrePrograma() const { return "credencialesplugin"; }

QIcon credencialesplugin::iconoPrograma() const
{ return QIcon( ":/imagenes/iconoBS.png" ); }

QString credencialesplugin::directorioBackup() const
{ return "credencialesplugin"; }

QString credencialesplugin::directorioActualizaciones() const
{ return "credencialesplugin"; }

QString credencialesplugin::empresa() const
{ return "credencialesplugin"; }

QString credencialesplugin::companeros()
{ return ""; }

bool credencialesplugin::publicidad()
{ return false; }

QString credencialesplugin::reporte( int tipo )
{
    switch( tipo ) {
        case EReporte::Factura:
        { return "Factura"; break; }
        case EReporte::Recibo:
        { return "Recibo"; break; }
        case EReporte::Presupuesto:
        { return "Presupuesto"; break; }
        case EReporte::AnulacionFactura:
        { return "AnulacionFactura"; break; }
        default: { return ""; break; }
    }
}


void credencialesplugin::reporteParametros(int, QString &, ParameterList &)
{ return; }

#include "DCredencial.h"
#include <QInputDialog>
#include <QSqlRecord>
#include "mequipos.h"
void credencialesplugin::crearcredencial()
{
    QStringList lista; QList<int> ids;
    MEquipos *m = new MEquipos();
    m->select();
    for( int i = 0; i<m->rowCount(); i++ ) {
        lista.append( m->record(i).value(1).toString() );
        ids.append( m->record(i).value(0).toInt() );
    }
    delete m;
    bool ok = false;
    QString equipo = QInputDialog::getItem( 0, "Elija el equipo contrario", "Equipo", lista, 0, false, &ok );
    if( ok ) {
        DCredencial *d = new DCredencial();
        emit agregarVentana( d );
        d->setearEquipo( ids.at( lista.indexOf( equipo ) ), equipo );
    }
}

#include "vequipos.h"
void credencialesplugin::verEquipos()
{ emit agregarVentana( new VEquipos() ); }

#include "vcredenciales.h"
void credencialesplugin::verCredenciales()
{ emit agregarVentana( new VCredenciales() ); }

Q_EXPORT_PLUGIN2( credenciales, credencialesplugin )