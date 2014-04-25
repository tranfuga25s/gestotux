#ifndef FORMPREFORDENTRABAJO_H
#define FORMPREFORDENTRABAJO_H

#include "formprefhijo.h"
#include "ui_FormPrefOrdenTrabajo.h"

class FormPrefOrdenTrabajo : public QWidget, public FormPrefHijo, private Ui::FormPrefOrdenTrabajo
{
    Q_OBJECT
public:
    explicit FormPrefOrdenTrabajo( QWidget *parent = 0 );

public slots:
    void cargar();
    void guardar();
    void aplicar() {}
    
};

#endif // FORMPREFORDENTRABAJO_H
