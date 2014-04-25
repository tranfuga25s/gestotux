#ifndef DAGREGARGARANTIA_H
#define DAGREGARGARANTIA_H

#include "../ordentrabajo/ecbequipamiento.h"
#include "ui_dagregargarantia.h"
class MEquipamiento;
#include "NumeroComprobante.h"

class DAgregarGarantia : public QDialog, private Ui::DAgregarGarantia
{
    Q_OBJECT
    
public:
    DAgregarGarantia( QWidget *parent = 0 );
    void setearIdComprobantante( const int id_comprobante );
    void setearIdProducto( const int id_producto );
    void setearNombreProducto( const QString nombre_producto );
    void setearIdCliente( const int id_cliente );
    void setearIdComprobante( const int id_comprobante );
    void setearIdEquipamiento( const int id_equipamiento );

signals:
    void actualizarModelos();

public slots:
    void accept();
    
protected:
    void changeEvent(QEvent *e);

protected slots:
    void buscarEquipamientos( int id_cliente );
    void buscarFactura( int id_equipamiento );
    void actualizarFechaFinGarantia( QDate fecha );

private:
    int _id_comprobante;
    int _id_producto;
    int _id_cliente;
    int _id_equipamiento;

    QString _nombre_producto;

    NumeroComprobante _proxima_garantia;

    MEquipamiento *modelo_equipamiento;
};

class MFactura {
public:
    static NumeroComprobante& obtenerComprobante( const int id_factura );
    static QDate obtenerFecha( const int id_factura );
};

#endif // DAGREGARGARANTIA_H
