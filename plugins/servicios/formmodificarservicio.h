#ifndef FORMMODIFICARSERVICIO_H
#define FORMMODIFICARSERVICIO_H

#include "mservicios.h"
#include "eventana.h"
#include <QDataWidgetMapper>
#include <QWidget>
#include "ui_formServicioBase.h"

/*!
 * \brief Formulario para modificar un servicio
 *
 *
 * \author Esteban Zeller
 */
class FormModificarServicio : public EVentana, private Ui::FormServicioBase
{
    Q_OBJECT
public:
    FormModificarServicio( MServicios *m, QWidget *parent = 0, Qt::WFlags fl = 0 );

protected slots:
    void guardar();
    void cambiarBaja( bool estado );

signals:
    void actualizarVista();

private:
    MServicios *modelo;
    QDataWidgetMapper *mapa;

};

#endif // FORMMODIFICARSERVICIO_H
