#ifndef PAGINACLIENTE_H
#define PAGINACLIENTE_H

#include <QWizardPage>
#include <QLabel>

#include "ecbclientes.h"

class PaginaCliente : public QWizardPage
{
    Q_OBJECT
public:
    PaginaCliente( QWidget *parent = 0 );
    int nextId() const;

private:
    QLabel *arriba;
    ECBClientes *CBClientes;

};

#endif // PAGINACLIENTE_H
