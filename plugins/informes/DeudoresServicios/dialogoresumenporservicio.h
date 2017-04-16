#ifndef DIALOGORESUMENPORSERVICIO_H
#define DIALOGORESUMENPORSERVICIO_H

#include "ui_dialogoresumenporservicio.h"
class MServicios;

class DialogoResumenPorServicio : public QDialog, private Ui::DialogoResumenPorServicio
{
    Q_OBJECT

public:
    explicit DialogoResumenPorServicio(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

public slots:
    void accept();

private:
    MServicios *mservicios;
};

#endif // DIALOGORESUMENPORSERVICIO_H
