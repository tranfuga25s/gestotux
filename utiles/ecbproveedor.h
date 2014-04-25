#ifndef ECBPROVEEDOR_H
#define ECBPROVEEDOR_H

#include <QComboBox>

class ECBProveedor : public QComboBox
{
    Q_OBJECT
     Q_PROPERTY( int id_proveedor READ idProveedorActual WRITE setearId USER true )
public:
    ECBProveedor(QWidget *parent = 0);
    ~ECBProveedor();
    void setearFiltro( const QString f );
    int idProveedorActual();
    void setearId( const int id_proveedor );

signals:
    void cambioIdProveedor( int id_proveedor );

private slots:
    void verificarExiste();
    void inicializar();
    void cambioProveedor( int ) { emit cambioIdProveedor( idProveedorActual() ); }

private:
   /*!
    * Guarda el ID de cliente segun la posicion del cb
    */
    QList<int> *ids;
   /*!
    * Filtro para la consulta de clientes
    */
    QString filtro;

    bool _inicializado;
    int _id;

};

#endif // ECBPROVEEDOR_H
