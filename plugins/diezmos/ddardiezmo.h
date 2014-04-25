#ifndef DDARDIEZMO_H
#define DDARDIEZMO_H

#include "ui_ddardiezmo.h"

class DDarDiezmo : public QDialog, private Ui::DDarDiezmo
{
    Q_OBJECT
    
public:
    explicit DDarDiezmo( QWidget *parent = 0 );

signals:
    void actualizarModelos();

public slots:
    void accept();
    
protected:
    void changeEvent(QEvent *e);
};

#endif // DDARDIEZMO_H
