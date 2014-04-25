#include "formprefdescuentos.h"

#include "preferencias.h"
#include "mdescuentostipo.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSqlRecord>

FormPrefDescuentos::FormPrefDescuentos(QWidget *parent) :
 FormPrefDescuentosBase(), FormPrefHijo()
{
    this->setParent( parent );
    setupUi( this );
    this->setWindowTitle( "Descuentos" );
    this->setWindowIcon( QIcon( ":/imagenes/descuento.png" ) );

    PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
    PBEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );

    connect( PBAgregar, SIGNAL( clicked() ), this, SLOT( agregar() ) );
    connect( PBEliminar, SIGNAL( clicked() ), this, SLOT( eliminar() ) );

    mdescuentos = new MDescuentosTipo( this );
    mdescuentos->setEditStrategy( QSqlTableModel::OnManualSubmit );

    TVLista->setModel( mdescuentos );
    TVLista->setAlternatingRowColors( true );
    TVLista->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    TVLista->setSelectionBehavior( QTableView::SelectRows );
    TVLista->hideColumn( 0 );

}

void FormPrefDescuentos::guardar()
{
    if( !mdescuentos->submitAll() ) {
        QMessageBox::warning( this, "Error", "No se pudieron guardar los descuentos modificados" );
    }
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Descuentos" );
    p->setValue( "usar", GBDescuentos->isChecked() );
    p->endGroup();
    p->endGroup();

}

void FormPrefDescuentos::cargar()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Descuentos" );
    GBDescuentos->setChecked( p->value( "usar", true ).toBool() );
    p->endGroup();
    p->endGroup();
    p=0;
    mdescuentos->select();
}

void FormPrefDescuentos::aplicar()
{ return; }

void FormPrefDescuentos::agregar()
{
    bool ok = false;
    QString codigo = QInputDialog::getText( this, "Codigo", "Codigo del descuento", QLineEdit::Normal, QString(), &ok );
    if( !ok )
        return;

    QString nombre = QInputDialog::getText( this, "Nombre", "Nombre:", QLineEdit::Normal, QString(), &ok );
    if( !ok )
        return;

    if( nombre.isEmpty() ) {
        QMessageBox::warning( this, "Error", "El nombre no puede ser nulo" );
        return;
    }

    double porcentaje = QInputDialog::getDouble( this, "Porcentaje", "Ingrese el porcentaje a aplicar:", 0.0, 0.0, 2147483647, 3, &ok );

    if( !ok )
        return;

    if( porcentaje <= 0.0 ) {
        QMessageBox::warning( this, "Error", "El porcentae no puede puede ser cero o negativo" );
        return;
    }

    QSqlRecord r = mdescuentos->record();
    r.setValue( "codigo", codigo );
    r.setValue( "nombre", nombre );
    r.setValue( "cantidad", porcentaje );
    if( mdescuentos->insertRecord( -1, r ) ) {
        QMessageBox::information( this, "Correcto", "Descuento agregado correctamente" );
        return;
    } else {
        QMessageBox::warning( this, "Error", "No se pudo agregar el registro" );
        return;
    }
}

void FormPrefDescuentos::eliminar()
{
  //
  if( TVLista->selectionModel()->selectedRows().isEmpty() ) {
      QMessageBox::information( this, "Error", "Por favor, seleccione un descuento para eliminar" );
      return;
  }
  int cant = TVLista->selectionModel()->selectedRows().count();
  int resp = 0;
  if( cant > 1 ) {
    resp = QMessageBox::question( this, "Confirmar", QString( "¿Esta seguro que desea eliminar estos %1 descuentos?" ).arg( cant ), QMessageBox::Yes, QMessageBox::No );
  } else {
    resp = QMessageBox::question( this, "Confirmar", "¿Esta seguro que desea eliminar este descuento?", QMessageBox::Yes, QMessageBox::No );
  }
  if( resp == QMessageBox::Yes ) {
      foreach( QModelIndex idx, TVLista->selectionModel()->selectedRows() ) {
          mdescuentos->removeRow( idx.row() );
      }
  }
  mdescuentos->submitAll();
}
