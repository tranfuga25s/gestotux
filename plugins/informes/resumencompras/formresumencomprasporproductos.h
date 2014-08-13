#ifndef FORMRESUMENCOMPRASPORPRODUCTOS_H
#define FORMRESUMENCOMPRASPORPRODUCTOS_H

#include "ui_formresumencomprasporproductos.h"

#include "eventana.h"
#include "resumencomprasporproductos.h"
#include "TiposPeriodos.h"

class FormResumenComprasPorProductos : public EVentana, private Ui::FormResumenComprasPorProductos
{
    Q_OBJECT
    
public:
    explicit FormResumenComprasPorProductos(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void cambioDivisionTemporal( const QString &texto );
    void cambiarDivisionTemporal();
    void cambioProducto( int id_producto );

private:
    QAction *ActCambiarDivisionTemporal;

    int _metodo_temporal;

    ResumenComprasPorProductos *_modelo;
};

#endif // FORMRESUMENCOMPRASPORPRODUCTOS_H
