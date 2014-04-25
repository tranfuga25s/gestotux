#ifndef FORMPREFDESCUENTOS_H
#define FORMPREFDESCUENTOS_H

#include "formprefhijo.h"
#include "ui_FormPrefDescuentosBase.h"
#include <QWidget>
class MDescuentosTipo;

class FormPrefDescuentos : public QWidget, protected Ui::FormPrefDescuentosBase, public FormPrefHijo
{
Q_OBJECT
public:
    FormPrefDescuentos( QWidget *parent = 0 );

public slots:
    void aplicar();
    void cargar();
    void guardar();

protected slots:
    void agregar();
    void eliminar();

private:
    MDescuentosTipo *mdescuentos;

};

#endif // FORMPREFDESCUENTOS_H
