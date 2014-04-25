#ifndef MDESCUENTOS_H
#define MDESCUENTOS_H

#include <QSqlTableModel>

class MDescuentos : public QSqlTableModel
{
    Q_OBJECT
public:
   /*!
    * \enum Tipo
    * Tipo de comprobante que posee descuento
    */
    enum Tipo {
        Invalido = 0, /** Tipo invalido */
        Presupuesto = 1, /** Tipo presupuesto */
        Factura = 2, /** Tipo Factura */
        Remito = 3 /** Tipo Remito */
    };

    MDescuentos( QObject *parent = 0 );
    void inicializar();

    bool agregarDescuento( Tipo t, int id_comprobante, QString texto, double porcentaje );

    void setearTipo( Tipo t ) { _tipo = t; }
    void setearIdComprobante( int id ) { _id_comprobante = id; }
    bool existenDatos();
    bool seleccionarDatos();

    
private:
    Tipo _tipo;
    int _id_comprobante;
    bool _seleccionado;
};

#endif // MDESCUENTOS_H
