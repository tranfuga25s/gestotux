#ifndef FORMPREFUNIDADESPRODUCTOS_H
#define FORMPREFUNIDADESPRODUCTOS_H

#include "ui_formprefunidadesproductos.h"
#include "formprefhijo.h"

class MUnidadesProductos;

class FormPrefUnidadesProductos : public QWidget,  public FormPrefHijo,  public Ui::FormPrefUnidadesProductos
{
    Q_OBJECT
public:
    FormPrefUnidadesProductos( QWidget *parent = 0 );
    
protected:
    void changeEvent(QEvent *e);

public slots:
    void cargar();
    void aplicar();
    void guardar();

private:
    MUnidadesProductos *modelo;

};

#endif // FORMPREFUNIDADESPRODUCTOS_H
