#ifndef FORMRECIBO_H
#define FORMRECIBO_H

#include "ui_FormReciboBase.h"
#include "eventana.h"

#include "eactpdf.h"
#include "eactimprimir.h"

class FormRecibo : public EVentana, private Ui::FormReciboBase
{
    Q_OBJECT
    
public:
    explicit FormRecibo(QWidget *parent = 0);
    void setId( int id );

public slots:
    void aPdf();
    void imprimir();
    
protected:
    void changeEvent(QEvent *e);

private:
    int _id_actual;
    void cargarDatos();
    EActImprimir *ActImprimir;
    EActPdf *ActPdf;
};

#endif // FORMRECIBO_H
