#ifndef DIALOGORESUMENSERVICIOSHISTORICO_H
#define DIALOGORESUMENSERVICIOSHISTORICO_H

#include "ui_dialogoresumenservicioshistorico.h"

class DialogoResumenServiciosHistorico : public QDialog, private Ui::DialogoResumenServiciosHistorico
{
    Q_OBJECT

public:
    explicit DialogoResumenServiciosHistorico(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

public slots:
    void accept();
};

#endif // DIALOGORESUMENSERVICIOSHISTORICO_H
