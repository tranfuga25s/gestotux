#ifndef DIALOGORESUMENPORSERVICIO_H
#define DIALOGORESUMENPORSERVICIO_H

#include "ui_dialogoresumenporservicio.h"

class DialogoResumenPorServicio : public QDialog, private Ui::DialogoResumenPorServicio
{
    Q_OBJECT

public:
    explicit DialogoResumenPorServicio(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

public slots:
    void accept();
};

#endif // DIALOGORESUMENPORSERVICIO_H
