#ifndef FORMVERIFICARRECARGOS_H
#define FORMVERIFICARRECARGOS_H

#include "ui_FormVerificarRecargos.h"
#include "eventana.h"

class FormVerificarRecargos : public EVentana, private Ui::FormVerificarRecargos
{
    Q_OBJECT

public:
    explicit FormVerificarRecargos(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

protected slots:
    void iniciar();
    void detener();

private:
    bool _detener;

    void l( QString cadena = QString() );

    QAction *ActDetener;
    QAction *ActIniciar;

    int cant_servicios;
    QList<int> lista_servicios;

};

#endif // FORMVERIFICARRECARGOS_H
