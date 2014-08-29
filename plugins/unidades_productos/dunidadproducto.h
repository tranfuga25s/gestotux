#ifndef DUNIDADPRODUCTO_H
#define DUNIDADPRODUCTO_H

#include <QDialog>
#include "munidadesproductos.h"

namespace Ui {
class DUnidadProducto;
}

class DUnidadProducto : public QDialog
{
    Q_OBJECT

public:
    DUnidadProducto(QWidget *parent = 0);
    ~DUnidadProducto();

    void setearModelo( MUnidadesProductos *m ) { this->modelo = m; }

public slots:
    void accept();

private:
    Ui::DUnidadProducto *ui;
    MUnidadesProductos *modelo;
};

#endif // DUNIDADPRODUCTO_H
