#ifndef MITEMFACTURA_H
#define MITEMFACTURA_H

#include <QSqlRelationalTableModel>

class MItemFactura : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MItemFactura(QObject *parent = 0);
    bool agregarItemFactura( const int id_venta, const double cantidad, const QString texto, const double precio_unitario, const int id_producto );
    static bool anularItemsFactura( const int id_factura );

private:
    void inicializar();
    void relacionar();

    /*!
     * Lleva el orden en que van a aparecer los items en la factura - Se aumenta automaticamente al agregar un item
     */
    int _orden;

};

#endif // MITEMFACTURA_H
