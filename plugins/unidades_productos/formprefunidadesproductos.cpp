#include "formprefunidadesproductos.h"

FormPrefUnidadesProductos::FormPrefUnidadesProductos(QWidget *parent) :
FormPrefHijo(), Ui::FormPrefUnidadesProductos()
{
    this->setParent( parent );
    setupUi(this);
    this->setWindowTitle( "Unidades" );
    this->setWindowIcon( QIcon( ":/imagenes/.png" ) );
    this->setAttribute( Qt::WA_DeleteOnClose );
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
}