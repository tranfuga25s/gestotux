#ifndef FORMRESUMENCOMPRASPROVEEDOR_H
#define FORMRESUMENCOMPRASPROVEEDOR_H

#include "ui_formresumencomprasproveedor.h"
#include "eventana.h"
class ResumenComprasProveedor;

class FormResumenComprasProveedor : public EVentana, private Ui::FormResumenComprasProveedor
{
    Q_OBJECT
    
public:
    FormResumenComprasProveedor(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

private:
    ResumenComprasProveedor *_modelo;
};

#endif // FORMRESUMENCOMPRASPROVEEDOR_H
