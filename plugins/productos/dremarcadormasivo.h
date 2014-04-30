#ifndef DREMARCADORMASIVO_H
#define DREMARCADORMASIVO_H

#include "ui_dremarcadormasivobase.h"
#include "mtempproductoremarcar.h"

class DRemarcadorMasivo : public QDialog, private Ui::DRemarcadorMasivo
{
  Q_OBJECT
  
public:
  DRemarcadorMasivo( QWidget *parent = 0 );
  
public slots:
  void avanzarProgreso( int cantidad );

protected:
  void changeEvent(QEvent *e);

protected slots:
  void cambioAPorcentaje( bool );
  void cambioAMontoFijo( bool );
  void agregarProducto();
  void agregarCategoria();
  void agregarTodos();
  void agregarProveedor();
  void eliminarProducto();
  void eliminarTodos();
  void accept();
  void cambioSinStock( bool );
  void cambioDeshabilitados( bool );

private:
  MTempProductoRemarcar *modelo;
  void recalcularFiltro();
  int _total;

};

#endif // DREMARCADORMASIVO_H
