#ifndef MITEMREMITO_H
#define MITEMREMITO_H

#include <QSqlRelationalTableModel>

class MItemRemito : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MItemRemito( QObject *parent = 0 );
    bool agregarItemRemito( const int id_venta, const double cantidad, const QString texto, const double precio_unitario, const int id_producto );

    void cargarItems( const int id_remito );

private:
    void inicializar();
    void relacionar();

    /*!
     * Lleva el orden en que van a aparecer los items en la factura - Se aumenta automaticamente al agregar un item
     */
    int _orden;

};

#endif // MITEMREMITO_H
