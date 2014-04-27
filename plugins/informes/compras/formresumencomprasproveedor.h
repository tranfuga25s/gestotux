#ifndef FORMRESUMENCOMPRASPROVEEDOR_H
#define FORMRESUMENCOMPRASPROVEEDOR_H

#include "ui_formresumencomprasproveedor.h"
class ResumenComprasProveedor;

class FormResumenComprasProveedor : public QWidget, private Ui::FormResumenComprasProveedor
{
    Q_OBJECT
    
public:
    explicit FormResumenComprasProveedor(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

private:
    ResumenComprasProveedor *_modelo;
};

#endif // FORMRESUMENCOMPRASPROVEEDOR_H
