#ifndef FORMPREFCOMPRAS_H
#define FORMPREFCOMPRAS_H

#include <QWidget>
#include "formprefhijo.h"
#include "ui_FormPrefComprasBase.h"

class FormPrefCompras : public QWidget, public Ui::FormPrefComprasBase, public FormPrefHijo
{
    Q_OBJECT
public:
    FormPrefCompras( QWidget *parent = 0 );

public slots:
    void aplicar();
    void cargar();
    void guardar();
    
};

#endif // FORMPREFCOMPRAS_H
