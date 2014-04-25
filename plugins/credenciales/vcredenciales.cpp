#include <QTableView>
#include "MCredenciales.h"
#include "vcredenciales.h"
#include <QMessageBox>
#include "EReporte.h"
#include "DCredencial.h"
#include "dsino.h"

VCredenciales::VCredenciales(QWidget *parent) :
    EVLista(parent)
{
    this->setObjectName( "listacredenciales" );
    this->setWindowTitle( "Listado de Credenciales" );

    this->rmodel = new MCredenciales( this );
    this->vista->setModel( this->rmodel );
    this->vista->hideColumn( 0 );
    this->vista->setSelectionBehavior( QAbstractItemView::SelectRows );
    this->vista->setAlternatingRowColors( true );
    this->vista->setItemDelegateForColumn(  5, new DSiNo( this->vista ) );
    this->vista->setItemDelegateForColumn(  6, new DSiNo( this->vista ) );
    this->vista->setItemDelegateForColumn(  7, new DSiNo( this->vista ) );
    this->vista->setItemDelegateForColumn(  8, new DSiNo( this->vista ) );
    this->vista->setItemDelegateForColumn(  9, new DSiNo( this->vista ) );
    this->vista->setItemDelegateForColumn( 10, new DSiNo( this->vista ) );
    this->rmodel->select();

    addAction( ActAgregar );
    addAction( ActBuscar );
    addAction( ActImprimir );
    addAction( ActCerrar );
}


void VCredenciales::agregar( bool /*autoeliminarid*/ )
{  emit agregarVentana( new DCredencial() ); }

void VCredenciales::imprimir()
{
    // Busco los ids seleccionados
    QModelIndexList lista = this->vista->selectionModel()->selectedRows();
    if( lista.isEmpty() ) {
        QMessageBox::information( this, "Error", "Seleccione una credencial para reimprimirla." );
        return;
    }
    // Busca el id
    EReporte *rep = new EReporte( 0 );
    ParameterList param;
    foreach( QModelIndex indx, lista ) {
        int id = this->rmodel->data( this->rmodel->index( indx.row(), 0 ), Qt::EditRole ).toInt();
        param.append( Parameter( "id_credencial", id ) );
        rep->especial( "credencial-union", param );
        if( ! rep->hacer() ) {
            qWarning( "Error al imprimir" );
        }
        param.clear();
     }
    delete rep;
}

#include "dbusquedacredencial.h"
void VCredenciales::buscar()
{
    DBusquedaCredencial *d = new DBusquedaCredencial( this );
    d->setearModelo( qobject_cast<MCredenciales *>(this->rmodel) );
    d->exec();
}
