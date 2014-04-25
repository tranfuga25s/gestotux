#ifndef FORMPREFCUOTAS_H
#define FORMPREFCUOTAS_H

#include "ui_formprefcuotasbase.h"
#include "formprefhijo.h"

class FormPrefCuotas : public QWidget, private Ui::FormPrefCuotasBase, public FormPrefHijo
{
    Q_OBJECT
    
public:
    FormPrefCuotas( QWidget *parent = 0 );

public slots:
    void aplicar();
    void cargar();
    void guardar();
    
protected:
    void changeEvent( QEvent *e );


};

#endif // FORMPREFCUOTAS_H
