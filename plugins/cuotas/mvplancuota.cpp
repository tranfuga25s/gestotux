#include "mvplancuota.h"

#include <QDate>

MVPlanCuota::MVPlanCuota(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "v_plan_cuota" );
    setHeaderData( 0, Qt::Horizontal, "#Plan" );
    setHeaderData( 1, Qt::Horizontal, "Cliente" );
    setHeaderData( 2, Qt::Horizontal, "Progreso" );
    setHeaderData( 3, Qt::Horizontal, "Faltante" );
    setHeaderData( 4, Qt::Horizontal, "Ultimo Pago" );
    setHeaderData( 5, Qt::Horizontal, "Proximo Pago" );
}


/*!
 * \brief MPlanCuota::data
 * \param idx Indice a mostrar
 * \param role Rol a mostrar
 */
QVariant MVPlanCuota::data(const QModelIndex &idx, int role) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( idx.column() ) {
                case 0: // Número de cuota
                {
                    return QString( "#%1" ).arg( QSqlTableModel::data( idx, role ).toInt() );
                    break;
                }
                case 3: // Cantidad faltante
                {
                    return QString( "$ %L1" ).arg( QSqlTableModel::data( idx, role ).toDouble(), 10, 'f', 2 );
                    break;
                }
                case 4:
                case 5:
                {
                    return QSqlTableModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                case 1: // Nombre del cliente
                case 2: // Progreso
                default:
                {
                    return QSqlTableModel::data( idx, role );
                    break;
                }
            }
            break;
        }
        case Qt::TextAlignmentRole:
        {
            switch( idx.column() ) {
                case 0: // Número de plan de cuota
                case 2: // Progreso
                case 4: // Ultimo pago
                case 5: // Proximo pago
                {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
                case 3: // Cantidad faltante
                {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                    break;
                }
                default:
                {
                    return QSqlTableModel::data( idx, role );
                    break;
                }
            }
            break;
        }
        default:
        {
            return QSqlTableModel::data( idx, role );
            break;
        }
    }
    return QVariant();
}

/*
CREATE VIEW `v_plan_cuota` AS
SELECT DISTINCT pc.id_plan_cuota AS plan_cuota,
       c.razon_social AS cliente,
       coalesce( ( SELECT COUNT( ic2.id_plan_cuota ) FROM item_cuota AS ic2 WHERE ic2.id_plan_cuota = pc.id_plan_cuota AND ic2.fecha_pago IS NOT NULL ), 0 ) || '/' || ( SELECT COUNT( ic2.id_plan_cuota ) FROM item_cuota AS ic2 WHERE ic2.id_plan_cuota = pc.id_plan_cuota AND ic2.fecha_pago IS NULL )  AS cuotas,
       ( SELECT SUM( ic2.monto ) FROM item_cuota AS ic2 WHERE ic2.id_plan_cuota = pc.id_plan_cuota AND ic2.fecha_pago IS NULL ) AS total_faltante,
       coalesce( ( SELECT MAX( ic2.fecha_pago ) FROM item_cuota AS ic2 WHERE ic2.id_plan_cuota = pc.id_plan_cuota AND ic2.fecha_pago IS NOT NULL ), 'Ninguno' ) AS ultimo_pago,
       ( SELECT MIN( ic2.fecha_vencimiento ) FROM item_cuota AS ic2 WHERE ic2.id_plan_cuota = pc.id_plan_cuota AND ic2.fecha_pago IS NULL ) AS proximo_pago
FROM plan_cuota AS pc
  INNER JOIN item_cuota AS ic ON ic.id_plan_cuota = pc.id_plan_cuota
  INNER JOIN factura AS f ON pc.id_factura = f.id_factura
  INNER JOIN clientes AS c ON f.id_cliente = c.id;
*/