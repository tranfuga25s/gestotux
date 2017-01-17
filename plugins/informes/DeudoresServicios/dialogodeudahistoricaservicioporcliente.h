#ifndef DIALOGODEUDAHISTORICASERVICIOPORCLIENTE_H
#define DIALOGODEUDAHISTORICASERVICIOPORCLIENTE_H

#include "ui_dialogodeudahistoricaservicioporcliente.h"

class DialogoDeudaHistoricaServicioPorCliente : public QDialog, private Ui::DialogoDeudaHistoricaServicioPorCliente
{
    Q_OBJECT

public:
    explicit DialogoDeudaHistoricaServicioPorCliente(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

public slots:
    void accept();
};

#endif // DIALOGODEUDAHISTORICASERVICIOPORCLIENTE_H
