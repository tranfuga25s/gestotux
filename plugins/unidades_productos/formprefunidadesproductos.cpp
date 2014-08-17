#include "formprefunidadesproductos.h"

#include "munidadesproductos.h"

FormPrefUnidadesProductos::FormPrefUnidadesProductos(QWidget *parent) :
FormPrefHijo(), Ui::FormPrefUnidadesProductos()
{
    this->setParent( parent );
    setupUi(this);
    this->setWindowTitle( "Unidades" );
    this->setWindowIcon( QIcon( ":/imagenes/unidades_productos.png" ) );
    this->setAttribute( Qt::WA_DeleteOnClose );

    this->modelo = new MUnidadesProductos( this->TVUnidades );
    this->modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
    this->TVUnidades->setModel( modelo );
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
{

}

/**
 * @brief FormPrefUnidadesProductos::aplicar
 */
void FormPrefUnidadesProductos::aplicar()
{
}

/**
 * @brief FormPrefUnidadesProductos::guardar
 */
void FormPrefUnidadesProductos::guardar()
{
    this->modelo->submitAll();
}
