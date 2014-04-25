#ifndef FORMPREFGARANTIAS_H
#define FORMPREFGARANTIAS_H

#include "ui_formprefgarantias.h"
#include "formprefhijo.h"

class FormPrefGarantias : public QWidget, public FormPrefHijo, private Ui::FormPrefGarantias
{
    Q_OBJECT
    
public:
    FormPrefGarantias(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

public slots:
    void cargar();
    void guardar();
    void aplicar() {}
};

#endif // FORMPREFGARANTIAS_H
