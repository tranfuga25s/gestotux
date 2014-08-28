#include "formprefunidadesproductos.h"

#include "munidadesproductos.h"
#include "dunidadproducto.h"

#include <QMessageBox>


FormPrefUnidadesProductos::FormPrefUnidadesProductos(QWidget *parent) :
FormPrefHijo(), Ui::FormPrefUnidadesProductos()
{
    this->setParent( parent );
    setupUi(this);
    this->setWindowTitle( "Unidades de Productos" );
    this->setWindowIcon( QIcon( ":/imagenes/unidades_productos.png" ) );
    this->setAttribute( Qt::WA_DeleteOnClose );

    this->modelo = new MUnidadesProductos( this->TVUnidades );
    this->modelo->select();
    //this->modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
    this->TVUnidades->setModel( modelo );
    this->TVUnidades->setItemsExpandable( true );
    this->TVUnidades->setSortingEnabled( true );
    this->TVUnidades->setExpandsOnDoubleClick( true );
    this->TVUnidades->setRootIsDecorated( true );
    this->TVUnidades->hideColumn( 0 );
    this->TVUnidades->hideColumn( 2 );

    PBAgregarUnidad->setText( "Agregar unidad" );
    PBAgregarUnidad->setIcon( QIcon( ":/imagenes/add.png" ) );
    connect( PBAgregarUnidad, SIGNAL( clicked() ), this, SLOT( agregarUnidad() ) );

    PBEliminarUnidad->setText( "Eliminar unidad" );
    PBEliminarUnidad->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
    connect( PBEliminarUnidad, SIGNAL( clicked() ), this, SLOT( eliminarUnidad() ) );
}

void FormPrefUnidadesProductos::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

/**
 * @brief FormPrefUnidadesProductos::cargar
 */
void FormPrefUnidadesProductos::cargar()
{}

/**
 * @brief FormPrefUnidadesProductos::aplicar
 */
void FormPrefUnidadesProductos::aplicar()
{}

/**
 * @brief FormPrefUnidadesProductos::guardar
 */
void FormPrefUnidadesProductos::guardar()
{
    this->modelo->submitAll();
}

/**
 * @brief FormPrefUnidadesProductos::agregarUnidad
 * Agrega una nueva unidad
 */
void FormPrefUnidadesProductos::agregarUnidad()
{
    DUnidadProducto *d = new DUnidadProducto();
    d->setearModelo( this->modelo );
    d->adjustSize();
    d->exec();
}

/**
 * @brief FormPrefUnidadesProductos::eliminarUnidad
 */
void FormPrefUnidadesProductos::eliminarUnidad()
{
    QModelIndexList lista = this->TVUnidades->selectionModel()->selectedRows();
    if( lista.size() == 0 ) {
        QMessageBox::information( this,
                                  "Sin elementos",
                                  "Por favor, seleccione alguna unidad para eliminar" );
        return;
    }
    foreach( QModelIndex indice, lista ) {
        if( !this->modelo->eliminar( indice.row() ) ) {
            QMessageBox::warning( this,
                                  "No se pudo eliminar",
                                  "No se pudo eliminar el indice elegido" );
        }
    }
}
