#include "vequipos.h"

#include <QTableView>

VEquipos::VEquipos(QWidget *parent) :
    EVLista(parent)
{
    this->setObjectName( "listaequipos" );
    this->setWindowTitle( "Equipos activos" );

    this->modelo = new MEquipos( this );
    this->vista->setModel( this->modelo );
    this->vista->hideColumn( 0 );
    this->vista->hideColumn( 2 );
    this->vista->setAlternatingRowColors( true );
    this->modelo->select();

    /// agregar accion de eliminar

    addAction( ActAgregar );
    //addAction( ActEliminar );
    addAction( ActCerrar );
}


#include <QInputDialog>
#include <QMessageBox>
#include <QSqlRecord>
void VEquipos::agregar( bool /*autoeliminarid*/ )
{
    bool ok = false;
    QString nuevo = QInputDialog::getText( this, "Nuevo equipo", "Nombre:", QLineEdit::Normal, "", &ok );
    if( ok ) {
        QSqlRecord r = this->modelo->record();
        r.setValue( "nombre", nuevo );
        r.setValue( "activo", true );
        if( this->modelo->insertRecord( -1, r ) ) {
            QMessageBox::information( this, "Correcto", "El equipo se registro correctamente" );
        } else {
            qDebug( "No se pudo agregar el equipo" ) ;
        }

    }
}
