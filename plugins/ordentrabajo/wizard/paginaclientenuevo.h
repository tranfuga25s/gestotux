#ifndef PAGINACLIENTENUEVO_H
#define PAGINACLIENTENUEVO_H

#include "ui_paginaclientenuevo.h"

class PaginaClienteNuevo : public QWizardPage, private Ui::PaginaClienteNuevo
{
    Q_OBJECT
    
public:
    PaginaClienteNuevo( QWidget *parent = 0 );
    void initializePage();
    int nextId() const;

private slots:
    void rehacerRazonSocial( QString );
    
};

#endif // PAGINACLIENTENUEVO_H
