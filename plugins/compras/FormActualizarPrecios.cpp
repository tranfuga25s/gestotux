#include "FormActualizarPrecios.h"

FormActualizarPrecios::FormActualizarPrecios(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void FormActualizarPrecios::changeEvent(QEvent *e)
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
