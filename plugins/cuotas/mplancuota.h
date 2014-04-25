#ifndef MPLANCUOTA_H
#define MPLANCUOTA_H

#include <QSqlTableModel>
#include <QDate>
/*!
 * \brief Modelo para los planes de cuotas
 *
 * \author Esteban Zeller
 */
class MPlanCuota : public QSqlTableModel
{
    Q_OBJECT

public:
    /*! \enum Periodo
     *  Tipos de periodos que podemos considerar al generar el cobro de los servicios
     */
    enum Periodo {
        Semanal = 1, /**< Una semana - 7 dias */
        Quincenal = 2, /**< Dos semana - 15 dias */
        Mensual = 3,  /**< Un mes - 28 a 31 dias */
        BiMensual = 4,  /**< Dos meses - 56 a 62 dias */
        Trimestral = 5, /**< Tres meses */
        Cuatrimestral = 6, /**< Cuatro meses */
        Seximestral = 7, /**< Seis meses */
        Anual = 8, /**< 12 meses - 365 dias */
        Invalido = -1 /** Invalido - Para casos de error */
    };

    enum TipoComprobante {
        TipoInvalido = -1,
        Factura = 0,
        Remito = 1
    };

    MPlanCuota( QObject *parent = 0 );

    bool agregarPlanCuota( int id_cliente, double cantidad, double interes, int periodo, double entrega, QDate fecha_inicio, int cant_cuotas, int *id_plan, bool recibo_efectivo, TipoComprobante tipo );
    
    static int diasEnPeriodo( const int tipo_periodo, QDate fecha_calculo );

    static void asociarConFactura( int id_plan, int id_factura );
    static QString obtenerRazonSocial( const int id_plan );
    static TipoComprobante obtenerTipoComprobante( const int id_plan );
    static int obtenerIdCliente( const int id_plan );
    static QPair<int,int> obtenerEstadoCuotas( const int id_plan );
    static QPair<double,double> obtenerEstadoImportes( const int id_plan );

    static bool cancelarPlan( const int id_plan_cuota, QString razon, QDateTime fecha_hora = QDateTime::currentDateTime() );

};

#endif // MPLANCUOTA_H
