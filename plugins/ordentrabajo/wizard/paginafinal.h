#ifndef PAGINAFINAL_H
#define PAGINAFINAL_H

#include "ui_paginafinal.h"

class PaginaFinal : public QWizardPage, private Ui::PaginaFinal
{
    Q_OBJECT
    
public:
    PaginaFinal( QWidget *parent = 0 );
    int nextId() const;
    void initializePage();
    
};

#endif // PAGINAFINAL_H
