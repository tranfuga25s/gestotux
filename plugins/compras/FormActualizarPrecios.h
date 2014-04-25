#ifndef FORMACTUALIZARPRECIOS_H
#define FORMACTUALIZARPRECIOS_H

#include "ui_FormActualizarPrecios.h"

class FormActualizarPrecios : public QWidget, private Ui::FormActualizarPrecios
{
    Q_OBJECT

public:
    explicit FormActualizarPrecios(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // FORMACTUALIZARPRECIOS_H
