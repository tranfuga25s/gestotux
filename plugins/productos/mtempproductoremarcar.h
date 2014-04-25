#ifndef MTEMPPRODUCTOREMARCAR_H
#define MTEMPPRODUCTOREMARCAR_H

#include <QAbstractTableModel>

/*!
 * \brief The MTempProductoRemarcar class
 * Clase para administrar los productos agregados a la lista de elementos cuando se intenta remarcar productos masivamente
 * \author Esteban Zeller
 */
class MTempProductoRemarcar : public QAbstractTableModel
{
  Q_OBJECT

public:
  MTempProductoRemarcar( QObject *parent = 0 );

  bool esFijo() { return _fijo; }
  bool esPorcentaje() { return _porcentaje; }
  void setearValorFijo( bool p );
  void setearPorcentaje( bool p );

  int columnCount(const QModelIndex &parent = QModelIndex() ) const;
  int rowCount(const QModelIndex &parent = QModelIndex() ) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  QPair<int,int> remarcar();

signals:
  void aumentoProgreso( int );

public slots:
  void agregarProducto( int id );
  void eliminarProducto( QModelIndex idx );
  void eliminarTodos();

  void cambioSinStock( bool estado );
  void cambioDeshabilitados( bool estado );

  void cambioValor( double valor );

private:
  QList<int> *_id_productos;
  QList<QString> *_codigos;
  QList<QString> *_nombres;
  QList<double> *_precio_original;
  QList<bool> *_sin_stock;
  QList<bool> *_deshabilitado;

  double _valor;

  bool _porcentaje;
  bool _fijo;


  
};

#endif // MTEMPPRODUCTOREMARCAR_H
