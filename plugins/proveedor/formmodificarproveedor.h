#ifndef FORMMODIFICARPROVEEDOR_H
#define FORMMODIFICARPROVEEDOR_H

#include "eventana.h"
#include "ui_FormProveedorBase.h"
#include "mproveedor.h"
#include <QDataWidgetMapper>

/*!
 * \brief Formulario para modificar un proveedor.
 *
 *
 * \author Esteban Zeller
 */
class FormModificarProveedor : public EVentana, public Ui::FormProveedorBase
{
    Q_OBJECT
public:
    explicit FormModificarProveedor( MProveedor *m, QWidget *parent = 0, bool testing = false );
    void setearItem( const int id );

public slots:
    bool guardar();

private:
    QDataWidgetMapper *mapa;
    MProveedor *modelo;
    bool testing;

};

#endif // FORMMODIFICARPROVEEDOR_H
