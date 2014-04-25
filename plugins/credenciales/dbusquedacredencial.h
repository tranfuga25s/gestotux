#ifndef DBUSQUEDACREDENCIAL_H
#define DBUSQUEDACREDENCIAL_H

#include "ui_dbusquedacredencial.h"
#include "MCredenciales.h"

class DBusquedaCredencial : public QDialog, private Ui::DBusquedaCredencialBase
{
    Q_OBJECT

public:
    explicit DBusquedaCredencial(QWidget *parent = 0);
    void setearModelo( MCredenciales *m );

public slots:
    void accept();
    void reject();

protected:
    void changeEvent(QEvent *e);

private:
    MCredenciales *modelo;
};

#endif // DBUSQUEDACREDENCIAL_H
