#ifndef FORMPRODUCTOBASE_H
#define FORMPRODUCTOBASE_H

#include "ui_formproductobase.h"

class FormProductoBase : public QDialog, private Ui::FormProductoBase
{
    Q_OBJECT

public:
    explicit FormProductoBase(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // FORMPRODUCTOBASE_H
