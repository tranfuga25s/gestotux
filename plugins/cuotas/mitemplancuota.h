#ifndef MITEMPLANCUOTA_H
#define MITEMPLANCUOTA_H

#include <QSqlRelationalTableModel>
#include <QDate>

/*!
 * \brief Clase para administrar los items y pagos de las cuotas.
 *
 * Esta clase permite mantener y administrar los pagos de los planes de cuotas asociados.
 * \author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MItemPlanCuota : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MItemPlanCuota( QObject *parent = 0 );
    
    static bool agregarItem( int id_plan, int num_cuota, QDate fecha_venc, double importe );

    static double obtenerProximoImporte( const int id_plan );
    static int obtenerIdProximaCuota( const int id_plan );

    static int buscarReciboEmitido( const int id_plan );
    static bool buscarSiReciboAPagar( const int id_recibo );

    static bool setearItemCuotaPagado(const int id_item_cuota, const int id_recibo , QDateTime fecha_pagado = QDateTime::currentDateTime() );
    static bool setearItemCuotaPagadoSegunRecibo( const int id_recibo , QDateTime fecha_pagado = QDateTime::currentDateTime() );
    static bool setearReciboItemCuota( const int id_recibo, const int id_item_cuota );

    static bool eliminarItemsNoPagadosNoEmitidos( const int id_plan_cuota );

    static bool agregarAdelanto( const int id_plan_cuota, double monto );

    QVariant data(const QModelIndex &item, int role) const;
    void setearPlanCuota( const int id_plan_cuota );

};

#endif // MITEMPLANCUOTA_H
