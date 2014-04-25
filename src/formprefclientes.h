#ifndef FORMPREFCLIENTES_H
#define FORMPREFCLIENTES_H

#include "ui_formprefclientesbase.h"
#include "formprefhijo.h"
#include "eventana.h"
#include "preferencias.h"

class FormPrefClientes : public EVentana, public FormPrefHijo, private Ui::FormPrefClientesBase
{
    Q_OBJECT
    
public:
    FormPrefClientes( QWidget *parent = 0 );

public slots:
      void cargar();
      void guardar();
      void aplicar();

protected:
    void changeEvent( QEvent *e );

};

#endif // FORMPREFCLIENTES_H
