#ifndef FORMVENCIMIENTOGARANTIAS_H
#define FORMVENCIMIENTOGARANTIAS_H

#include "ui_formvencimientogarantias.h"
#include "mvgarantias.h"
#include "eventana.h"
class EActCerrar;

class FormVencimientoGarantias : public EVentana, private Ui::FormVencimientoGarantias
{
    Q_OBJECT
    
public:
    FormVencimientoGarantias( QWidget *parent = 0 );

protected slots:
    void cambioFecha( QDate fecha );
    void cambioBaja( bool );
    
protected:
    void changeEvent(QEvent *e);

private:
    MVGarantias *mgarantias;
    EActCerrar *ActCerrar;

};

#endif // FORMVENCIMIENTOGARANTIAS_H
