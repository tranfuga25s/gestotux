#ifndef FORMRESUMENCOMPRASTOTALES_H
#define FORMRESUMENCOMPRASTOTALES_H

#include "ui_formresumencomprastotales.h"
#include "eventana.h"
#include "resumencomprastotales.h"

class FormResumenComprasTotales : public EVentana, private Ui::FormResumenComprasTotales
{
    Q_OBJECT
    
public:
    FormResumenComprasTotales( QWidget *parent = 0 );

protected slots:
    void cambiarDivisionTemporal();
    
protected:
    void changeEvent( QEvent *e );

private slots:
    void cambioDivisionTemporal( const QString &texto );

private:
    QAction *ActCambiarDivisionTemporal;
    ResumenComprasTotales::DivisionTemporal _metodo_temporal;
    ResumenComprasTotales *_modelo;

};

#endif // FORMRESUMENCOMPRASTOTALES_H
