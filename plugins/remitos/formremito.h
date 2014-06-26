#ifndef FORMREMITO_H
#define FORMREMITO_H

#include "eventana.h"
#include "ui_FormAgregarRemitoBase.h"

class FormRemito : public EVentana, public Ui::FormAgregarRemitoBase
{
    Q_OBJECT
public:
    FormRemito(QWidget *parent = 0);

    void setearValor( const int id_remito );
    
};

#endif // FORMREMITO_H
