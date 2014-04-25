#include "mitemplancuota.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QColor>
#include <QDebug>

MItemPlanCuota::MItemPlanCuota(QObject *parent) :
QSqlRelationalTableModel(parent)
{
    setTable( "item_cuota" );
    setHeaderData( 0, Qt::Horizontal, "#ID Item" );
    setHeaderData( 1, Qt::Horizontal, "#ID Plan" );
    setHeaderData( 2, Qt::Horizontal, "#Cuota" );
    setHeaderData( 3, Qt::Horizontal, "Importe" );
    setHeaderData( 4, Qt::Horizontal, "Fecha Vencimiento" );
    setHeaderData( 5, Qt::Horizontal, "Fecha de Pago" );
    setHeaderData( 6, Qt::Horizontal, "#ID Recibo" );
}

/*!
 * \fn MItemPlanCuota::agregarItem( int id_plan, int num_cuota, QDate fecha_venc, double importe )
 * Agrega un nuevo registro de item de cuota cuando se agrega un nuevo plan de cuota.
 * \param id_plan Identificador de relacion hacia el plan de cuotas.
 * \param num_cuota Numero de la cuota del plan.
 * \param fecha_venc Fecha desde la cual se empezará a pagar el plan.
 * \param importe Importe a pagar en esa cuota
 * \return Verdadero si se pudo agregar el item
 */
bool MItemPlanCuota::agregarItem(int id_plan, int num_cuota, QDate fecha_venc, double importe)
{
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO item_cuota( id_plan_cuota, num_cuota, monto, fecha_vencimiento ) VALUES (  :id_plan_cuota, :num_cuota, :monto, :fecha_vencimiento )" ) ) {
        qDebug( "Error al preparar la cola para la inseción de los datos de un item de cuota" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    cola.bindValue( ":id_plan_cuota", id_plan );
    cola.bindValue( ":num_cuota", num_cuota );
    cola.bindValue( ":monto", importe ),
    cola.bindValue( ":fecha_vencimiento", fecha_venc );
    if( !cola.exec() ) {
        qDebug( "Error al ejecutar la cola de insersión de los datos de un item de cuota" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

/**
 * @brief MItemPlanCuota::obtenerProximoImporte
 * Devuelve el importe del item de cuota que no esté pagado.
 * @param id_plan Identificador del plan
 * @return valor de la cuota
 */
double MItemPlanCuota::obtenerProximoImporte( const int id_plan )
{
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT monto FROM item_cuota WHERE id_plan_cuota = %1 AND fecha_pago is NULL ORDER BY fecha_vencimiento LIMIT 1" ).arg( id_plan ) ) ) {
      if( cola.next() ) {
          return cola.record().value(0).toDouble();
      } else {
          qDebug( "Error al hacer next en la cola de averiguacion del importe de un item de cuota" );
          qDebug() << cola.lastError().text();
          qDebug() << cola.lastQuery();
      }
  } else {
      qDebug( "Error al ejecutar la cola de averiguacion del importe de un item de cuota" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return -1.0;
}

/**
 * @brief MItemPlanCuota::obtenerIdProximaCuota
 * Busca el ID del proximo item de cuota a pagar para un plan específico
 * @param id_plan Identificador del plan
 * @return Identificador o -1 si hubo error o -2 si no existe proximo ítem
 */
int MItemPlanCuota::obtenerIdProximaCuota( const int id_plan )
{
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT id_item_cuota FROM item_cuota WHERE id_plan_cuota = %1 AND fecha_pago is NULL ORDER BY fecha_vencimiento LIMIT 1" ).arg( id_plan ) ) ) {
      if( cola.next() ) {
          return cola.record().value(0).toInt();
      } else {
          qDebug( "Error al hacer next en la cola de averiguacion del id de un item de cuota" );
          qDebug() << cola.lastError().text();
          qDebug() << cola.lastQuery();
          return -2;
      }
  } else {
      qDebug( "Error al ejecutar la cola de averiguacion del id de un item de cuota" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return -1.0;
}

/**
 * @brief MItemPlanCuota::buscarReciboEmitido
 * @param id_plan Identificador del plan
 * @return Identificador del recibo si existe, -2 si no se encuentra el dato y -1 si hubo un error
 */
int MItemPlanCuota::buscarReciboEmitido( const int id_plan )
{
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT id_recibo FROM item_cuota WHERE id_plan_cuota = %1 AND fecha_pago is NULL ORDER BY fecha_vencimiento LIMIT 1" ).arg( id_plan ) ) ) {
      if( cola.next() ) {
          return cola.record().value(0).toInt();
      } else {
          qDebug( "Error al hacer next en la cola de averiguacion del id de recibo de un item de cuota" );
          qDebug() << cola.lastError().text();
          qDebug() << cola.lastQuery();
          return -2;
      }
  } else {
      qDebug( "Error al ejecutar la cola de averiguacion del id de recibo de un item de cuota" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return -1;
}

/**
 * @brief MItemPlanCuota::setearItemCuotaPagadoSegunRecibo
 * Setea un plan de cuota como pagado según el recibo pagado
 * @param id_recibo Identificador de recibo
 * @return Verdadero si se pudo setear, falso en caso contrario
 */
bool MItemPlanCuota::setearItemCuotaPagadoSegunRecibo( const int id_recibo, QDateTime fecha_pagado )
{
  QSqlQuery cola;
  if( cola.exec( QString( "UPDATE item_cuota SET fecha_pago = \"%2\" WHERE id_recibo = %1 AND fecha_pago is NULL" ).arg( id_recibo ).arg( fecha_pagado.toString( Qt::ISODate ) ) ) ) {
      return true;
  } else {
      qDebug( "Error al ejecutar la cola de seteo de item de cuota pagado según id de recibo " );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return false;
}

/**
 * @brief MItemPlanCuota::buscarSiReciboAPagar
 * Verifica si el identificador de recibo se encuentra en un item de cuota a pagar
 * @param id_recibo Identificador del recibo
 * @return verdader si corresponde a una cuota o falso.
 */
bool MItemPlanCuota::buscarSiReciboAPagar( const int id_recibo )
{
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT COUNT( id_recibo ) FROM item_cuota WHERE id_recibo = %1 AND fecha_pago is NULL" ).arg( id_recibo ) ) ) {
      if( cola.next() ) {
          if ( cola.record().value(0).toInt() > 0 ) {
              return true;
          }
      } else {
          qDebug( "Error al hacer next en la cola de averiguacion del id de recibo a pagar de un item de cuota" );
          qDebug() << cola.lastError().text();
          qDebug() << cola.lastQuery();
      }
  } else {
      qDebug( "Error al ejecutar la cola de averiguacion del id de recibo a pagar de un item de cuota" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return false;
}

/**
 * @brief MItemPlanCuota::setearItemCuotaPagado
 * Setea el item de cuota como pagado con el identificador de recibo
 * @param id_item_cuota Identificador de item de cuota
 * @param id_recibo Identificador del recibo que paga la cuota.
 * @return Verdadero si se pudo hacer la asociacion.
 */
bool MItemPlanCuota::setearItemCuotaPagado( const int id_item_cuota, const int id_recibo, QDateTime fecha_pagado )
{
  QSqlQuery cola;
  if( cola.exec( QString( "UPDATE item_cuota SET fecha_pago = \"%2\", id_recibo = %1 WHERE id_item_cuota = %3" ).arg( id_recibo ).arg( fecha_pagado.toString( Qt::ISODate ) ).arg( id_item_cuota ) ) ) {
      return true;
  } else {
      qDebug( "Error al ejecutar la cola de seteo de item de cuota pagado" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return false;
}

/**
 * @brief MItemPlanCuota::setearReciboItemCuota
 * Setea el item de cuota con el identificador de recibo
 * @param id_item_cuota Identificador de item de cuota
 * @param id_recibo Identificador del recibo que paga la cuota.
 * @return Verdadero si se pudo hacer la asociacion.
 */
bool MItemPlanCuota::setearReciboItemCuota( const int id_item_cuota, const int id_recibo )
{
  QSqlQuery cola;
  if( cola.exec( QString( "UPDATE item_cuota SET id_recibo = %1 WHERE id_item_cuota = %2" ).arg( id_recibo ).arg( id_item_cuota ) ) ) {
      return true;
  } else {
      qDebug( "Error al ejecutar la cola de seteo de item de cuota pagado" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  return false;
}

/*!
 * \brief MItemPlanCuota::eliminarItemsNoPagadosNoEmitidos
 * ELimina todos los items de plan de cuota que no hayan sido pagados o emitidos.
 * \param id_plan_cuota Identificador del plan de cuotas
 * \return Verdadero si la consulta fue exitosa
 */
bool MItemPlanCuota::eliminarItemsNoPagadosNoEmitidos(const int id_plan_cuota)
{
    QSqlQuery cola;
    if( cola.exec( QString( "DELETE FROM item_cuota WHERE id_plan_cuota = %1"
                            "  AND fecha_pago IS NULL "
                            "  AND id_recibo  IS NULL " ).arg( id_plan_cuota ) ) ) {
        return true;
    } else {
        qDebug( "Error al ejecutar la cola de eliminacion de items no pagados, no emitidos de plan de cuota" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MItemPlanCuota::agregarAdelanto
 *  Genera un adelanto de cuotas según el importe ingresado.
 *  Esto implica: cargar todos los items de cuota, buscar cuales deben ser eliminados segun el monto y cambiar el monto de la ultima cuota según el resto.
 * \param id_plan_cuota Identificador de plan de cuota emitido
 * \param monto Monto a adelantar
 * \return Verdadero si se pudo realizar el registro
 */
bool MItemPlanCuota::agregarAdelanto(const int id_plan_cuota, double monto)
{
    // Busco todos los identificadores de cuotas en orden inverso.
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT id_item_cuota, monto FROM item_cuota WHERE id_plan_cuota = %1"
                             "   AND fecha_pago IS NULL "
                             "   AND id_recibo  IS NULL "
                             " ORDER BY fecha_pago DESC ").arg( id_plan_cuota ) ) ) {
        qDebug() << "Error al ejecutar la cola de obtención de datos de planes de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    QVector<int> ids;
    QVector<double> montos;
    while( cola.next() ) {
        ids.append( cola.record().value(0).toInt() );
        montos.append( cola.record().value(0).toDouble() );
    }
    if( ids.size() <= 0 || montos.size() <= 0 ) {
        qDebug() << "Error al cargar los datos - ids.size() <= 0 || montos.size() <= 0";
        return false;
    }
    // Veo hasta que ID tengo que eliminar
    int cantidad = 0;
    double temp = monto;
    for( int i = ids.size()-1; i >= 0; i-- ) {
        if( temp > montos.at( i ) ) {
            temp -= montos.at( i );
            cantidad++;
        }
    }
    // Elimino los ultimos "cantidad" items de cuota
    QStringList a_eliminar;
    for( int i=0; i<cantidad; i++ ) {
        int id = ids.at( ids.size() - ( i + 1 ) );
        a_eliminar.append( QString::number( id ) );
    }
    if( !cola.exec( QString( "DELETE FROM item_cuota WHERE id_plan_cuota = %1 AND id_item_cuota IN ( %2 ) " ).arg( id_plan_cuota ).arg( a_eliminar.join( "," ) ) ) ) {
        qDebug() << "Error al ejecutar la cola de eliminación de items de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    // Busco cuanto tengo que cambiar de la ultima cuota
    // La variable temporal posee lo que falta restar a la ultima cuota
    int id_cuota_modificar = ids.at( ids.size() - cantidad );
    double nuevo_monto = montos.at( ids.size() - cantidad ) - temp;
    // Actualizo el valor de la cuota
    if( !cola.exec( QString( "UPDATE item_cuota SET monto = %1"
                             " WHERE id_item_cuota = %2 " ).arg( nuevo_monto ).arg( id_cuota_modificar ) ) ) {
        qDebug() << "Error al ejecutar la cola de modificacion de precio del item de cuota";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    return true;
}

QVariant MItemPlanCuota::data(const QModelIndex &item, int role) const
{
    if( item.isValid() ) {
        switch( item.column() ) {
            case 0: // Identificador del item del plan de cuota
            case 1: // Identificador del plan de cuota padre
            case 2: // Numero de cuota
            {
                switch( role ) {
                    case Qt::DisplayRole:
                    { return QString( "#%1").arg( QSqlRelationalTableModel::data( item, role ).toInt() ); break; }
                    case Qt::EditRole:
                    { return QSqlRelationalTableModel::data( item, role ).toInt(); break; }
                    case Qt::BackgroundColorRole:
                    {
                        if( QSqlRelationalTableModel::data( index( item.row(), 5 ), Qt::EditRole ).toDate().isValid() ) {
                            return QColor( Qt::darkGreen );
                        }
                        break;
                    }
                    default:
                    { return QSqlRelationalTableModel::data( item, role ); break; }
                }
                break;
            }
            case 3: // Importe
            {
                switch( role ) {
                    case Qt::DisplayRole:
                    { return QString( "$ %L1" ).arg( QSqlRelationalTableModel::data( item, role ).toDouble(), 10, 'f', 2 ); break; }
                    case Qt::EditRole:
                    { return QSqlRelationalTableModel::data( item, role ).toDouble(); break; }
                    case Qt::BackgroundColorRole:
                    {
                        if( QSqlRelationalTableModel::data( index( item.row(), 5 ), Qt::EditRole ).toDate().isValid() ) {
                            return QColor( Qt::darkGreen );
                        }
                        break;
                    }
                    default:
                    { return QSqlRelationalTableModel::data( item, role ); break; }
                }
                break;
            }
            case 4: // Fecha de vencimiento
            case 5: // Fecha de Pago
            {
                switch( role ) {
                    case Qt::DisplayRole:
                    { return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::LocaleDate ); break; }
                    case Qt::EditRole:
                    { return QSqlRelationalTableModel::data( item, role ).toDate(); break; }
                    case Qt::BackgroundColorRole:
                    {
                        if( QSqlRelationalTableModel::data( index( item.row(), 5 ), Qt::EditRole ).toDate().isValid() ) {
                            return QColor( Qt::darkGreen );
                        }
                        break;
                    }
                    default:
                    { return QSqlRelationalTableModel::data( item, role ); break; }
                }
                break;
            }
            default:
            {
                return QSqlRelationalTableModel::data( item, role ); break;
            }
        }
    }
    return QSqlRelationalTableModel::data( item, role );
}

void MItemPlanCuota::setearPlanCuota(const int id_plan_cuota)
{
    this->setFilter( QString( " id_plan_cuota = %1" ).arg( id_plan_cuota ) );
}
