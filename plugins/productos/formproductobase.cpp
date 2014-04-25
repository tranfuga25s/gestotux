#include "formproductobase.h"

FormProductoBase::FormProductoBase(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void FormProductoBase::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
