#include <QDate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "mservicios.h"
#include "mperiodoservicio.h"
#include "math.h"

#include <QMessageBox>
#include <QDebug>

MPeriodoServicio::MPeriodoServicio( QObject *parent ) :
QSqlRelationalTableModel( parent ) {
    //inicializar();
}


void MPeriodoServicio::inicializar() {
    setTable( "periodo_servicio" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "#Servicio" );
    setHeaderData( 2, Qt::Horizontal, "Periodo" );
    setHeaderData( 3, Qt::Horizontal, QString::fromUtf8( "Año" ) );
    setHeaderData( 4, Qt::Horizontal, "Fecha de Facturacion" );
    setHeaderData( 5, Qt::Horizontal, "Fecha de Inicio" );
    setHeaderData( 6, Qt::Horizontal, "Fecha de Fin" );
}

QVariant MPeriodoServicio::data(const QModelIndex &item, int role) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( item.column() ) {
                case 5:
                case 6:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                case 2:
                case 3:
                {
                    return QSqlRelationalTableModel::data( item, role ).toInt();
                    break;
                }
                case 4:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDateTime().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                default:
                {
                    return QSqlRelationalTableModel::data( item, role );
                    break;
                }
            }
            break;
        }
        case Qt::TextAlignmentRole:
        {
            return int( Qt::AlignCenter | Qt::AlignVCenter );
        }
        case Qt::EditRole:
        {
            switch( item.column() ) {
                case 5:
                case 6:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDate();
                    break;
                }
                case 0:
                case 1:
                case 2:
                case 3:
                {
                    return QSqlRelationalTableModel::data( item, role ).toInt();
                    break;
                }
                case 4:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDateTime();
                    break;
                }
                default:
                {
                    return QSqlRelationalTableModel::data( item, role );
                    break;
                }
            }
            break;
        }
        default:
        {
            return QSqlRelationalTableModel::data( item, role );
            break;
        }
    }
}

Qt::ItemFlags MPeriodoServicio::flags( const QModelIndex &/*index*/ ) const
{ return Qt::ItemIsEnabled | Qt::ItemIsSelectable; }


void MPeriodoServicio::relacionar() {
    setRelation( 1, QSqlRelation( "id_servicio", "servicios", "nombre" ) );
}

/*!
 * \fn MPeriodoServicio::agregarPeriodoServicio( const int id_servicio, const int periodo, const int ano, const QDate fecha_inicio, const QDate fecha_fin )
 * Agrega el registro de que se realizo el cobro de un servicio identificado mediante la factura en un periodo del servicio.
 * \param id_servicio Identificador del servicio a facturar
 * \param periodo Numero de periodo dentro del año
 * \param ano Año del cobro
 * \param fecha_inicio Fecha de inicio del periodo
 * \param fecha_fin Fecha de fin del periodo
 * \return Identificador de id_periodo_servicio o -1 si hubo un error.
 */
int MPeriodoServicio::agregarPeriodoServicio( const int id_servicio, const int periodo, const int ano, const QDate fecha_inicio, const QDate fecha_fin )
{
 QSqlQuery cola;
 cola.prepare( "INSERT INTO periodo_servicio( id_servicio, periodo, ano, fecha_inicio, fecha_fin, fecha ) VALUES( :id_servicio, :periodo, :ano, :fecha_inicio, :fecha_fin, :fecha )" );
 cola.bindValue( ":id_servicio", id_servicio );
 cola.bindValue( ":periodo", periodo );
 cola.bindValue( ":ano", ano );
 cola.bindValue( ":fecha_inicio", fecha_inicio );
 cola.bindValue( ":fecha_fin", fecha_fin );
 cola.bindValue( ":fecha", QDateTime::currentDateTime() );
 if( cola.exec() ) {
     return cola.lastInsertId().toInt();
 } else {
     qDebug() << "Servicios::MPeriodoServicio::agregarPeriodoServicio - Error al intentar agregar el periodo de un servicio ( exec )";
     qDebug() << cola.lastError().text();
     return -1;
 }
}

/*!
 * \fn MPeriodoServicio::ultimaFechaDeServicio( const int id_servicio )
 * Devuelve la ultima fecha en que se realizo un registro de periodo de servicio. Si no existe fecha registrada hasta ese momento, retorna el día de ayer.
 * \param id_servicio Identificador del servicio
 * \return QDate con la fecha, si hubo un error, retorna una fecha invalida
 */
QDate MPeriodoServicio::ultimaFechaDeServicio( const int id_servicio ) {
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT MAX( fecha_fin ) FROM periodo_servicio WHERE id_servicio = %1" ).arg( id_servicio ) ) ) {
      if( cola.next() ) {
          return cola.record().value(0).toDate();
      } else {
          qDebug() << "Servicios::MPeriodoServicio::ultimaFechaDeServicio : Error al hacer next en la cola -  Se tomará la fecha de hoy";
          qDebug() << cola.lastError().text();
          qDebug() << cola.lastQuery();
          return QDate::currentDate().addDays( -1 );
      }
  } else {
      qDebug() << "Servicios::MPeriodoServicio::ultimaFechaDeServicio : Error al hacer next en la cola -  Se tomará la fecha de hoy";
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
      return QDate();
  }
}

/*!
 * \fn MPeriodoServicio::obtenerFechaFinPeriodo( const int id_servicio, const QDate fecha_inicio )
 * Obtiene la fecha de fin del periodo dado un serivicio determinado y la fecha de inicio pasada como parametro
 * \param id_servicio Servicio en cuestion
 * \param fecha_inicio Fecha desde la cual calcular el periodo
 * \return Fecha de fin o una fecha invalida si hubo error
 */
QDate MPeriodoServicio::obtenerFechaFinPeriodo( const int id_servicio, const QDate fecha_inicio ) {
    MServicios::Periodo p = MServicios::obtenerPeriodo( id_servicio );
    if( p != MServicios::Invalido ) {
        return fecha_inicio.addDays( diasEnPeriodo( p, fecha_inicio ) - 1 );
    }
    return QDate();
}

/*!
 * \fn MPeriodoServicio::diasEnPeriodoServicio( const int id_servicio, QDate fecha_calculo )
 * Considerando que todos los periodos se ajustan dentro de un año, devolverá el numero de días que tiene el periodo del servicio seleccionado en la fecha elegida ( sin parametro fecha actual ) segun la fecha de alta del servicio.
 * En el caso de que sea mensual, se devolverá la cantidad de días que tiene el mes de fecha_calculo
 * En el caso de que sea bimestral, se devolverá la cantidad de días que tiene el mes de fecha_calculo mas la cantidad de días que tiene el mes siguiente.
 * En el caso de que sea trimestra, se devolverá la cantidad de días que tiene el mes de fecha_calculo mas la cantidad de días que tienen los 2 meses siguientes.
 * @param id_servicio Identificador del servicio
 * @param fecha_calculo Fecha que se desea averiguar el periodo ( predeterminada fecha actual )
 * @return Cantidad de Días que tiene el periodo que corresponde a la fecha solicitada
 */
int MPeriodoServicio::diasEnPeriodoServicio( const int id_servicio, QDate fecha_calculo ) {
    int tipo = MServicios::obtenerPeriodo( id_servicio );
    return diasEnPeriodo( tipo, fecha_calculo );
}

/*!
 * \fn MPeriodoServicio::diasEnPeriodo( const int tipo_periodo, QDate fecha_inicio )
 * Considerando que todos los periodos se ajustan dentro de un año, devolverá el numero de días que tiene el periodo seleccionado en la fecha elegida ( sin parametro fecha actual ) segun la fecha de alta del servicio.
 * En el caso de que sea mensual, se devolverá la cantidad de días que tiene el mes de fecha_calculo
 * En el caso de que sea bimestral, se devolverá la cantidad de días que tiene el mes de fecha_calculo mas la cantidad de días que tiene el mes siguiente.
 * En el caso de que sea trimestra, se devolverá la cantidad de días que tiene el mes de fecha_calculo mas la cantidad de días que tienen los 2 meses siguientes.
 * @param tipo_periodo Tipo de periodo que estamos considerando
 * @param fecha_inicio Fecha que se desea averiguar el periodo ( predeterminada fecha actual )
 * @return Cantidad de Días que tiene el periodo que corresponde a la fecha solicitada
 */
int MPeriodoServicio::diasEnPeriodo( const int tipo_periodo, QDate fecha_calculo )
{
    // Cuidado con los mensuales! ( feb en año bisiesto )
    switch( tipo_periodo )
    {
        case MServicios::Semanal:
        {
            // Semanal -> La semana siempre tiene 7 dias independientemente del día del mes
            /// \todo Corte anual ¿Considerarlo? ( cuando la semana no esta completa un año )
            return 7;
        }
        case MServicios::Quincenal:
        {
            // Quincenal -> se considera como "medio mes"
            /// \todo Verificar caso de febrero y meses con 30 o 31 días
            return 14;
        }
        case MServicios::Mensual:
        {
            // Mensual
            // Verificar el mes del periodo y devolver la cantidad de días
            int dias_en_mes = QDate( fecha_calculo.year(), fecha_calculo.month(), 1 ).daysInMonth();
            if (fecha_calculo.day() > 1 ) {
                dias_en_mes -= fecha_calculo.day();
            }
            return dias_en_mes;
            // Eso se encarga automaticamnete de los años bisiestos
        }
        case MServicios::BiMensual:
        {
            // BiMensual
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(1);
            QDate f3(  f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) - 1;
        }
        case MServicios::Trimestral:
        {
            // Trimensual
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(2);
            QDate f3( f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) - 1;
        }
        case MServicios::Cuatrimestral:
        {
            // Cuatrimestral
            // Siempre voy a pedir estos datos al inicio del periodo
        QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(3);
            QDate f3( f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) -1 ;
        }
        case MServicios::Seximestral:
        {
            //Seximestral
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(5);
            QDate f3( f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) - 1 ;
        }
        case MServicios::Anual:
        {
            // Como consideramos los servicios con base en 1 año, siempre es periodo 1
            return fecha_calculo.daysInYear() -1 ;
        }
        default:
        { return 0; }
    }
}

/*!
 * \fn MPeriodoServicio::existeFacturacion( const int id_servicio )
 * Devuelve verdadero si existe alguna facturación para el servicio elegido como parametro
 * \param id_servicio Identificador del servicio
 **/
bool MPeriodoServicio::existeFacturacion(const int id_servicio)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT( id_servicio ) FROM periodo_servicio WHERE id_servicio = %1" ).arg( id_servicio ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        } else {
            return false;
        }
    } else {
        qDebug() << "Servicios::MPeriodoServicio::existeFacturacion: Error en el exec de la cola actual";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
}

/*!
 * \fn MPeriodoServicio::getPeriodoActual( const int id_servicio, bool facturar )
 * Devuelve el numero de periodo correspondiente al servicio a facturar de acuerdo con los ultimos datos dados
 * \param id_servicio Identificador del servicio
 * \param facturar Calcula los datos para el periodo siguiente al actual.
 * \returns identificador dentro del numero de periodos o -1 si hubo un error.
 */
int MPeriodoServicio::getPeriodoActual( const int id_servicio, bool facturar ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT periodo, fecha_fin FROM periodo_servicio WHERE id_servicio = %1 ORDER BY fecha_fin DESC LIMIT 1" ).arg( id_servicio ) ) ) {
        if( cola.next() ) {
            if( facturar ) {
                int per = cola.record().value(0).toInt();
                if( (per + 1 ) > cantidadPeriodos( id_servicio ) ) {
                    per = 1;
                } else {
                    per++;
                }
                return per;
            } else {
                return cola.record().value(0).toInt();
            }
        } else {
            // No hay ningun registro todavía - Es el primer periodo a registrar
            // Por lo tanto facturo el periodo en el que estamos no anteriores.
            int n = this->getPeriodoSegunFecha( id_servicio, QDate::currentDate() );
            if( facturar ) {
                if( (n+1) > cantidadPeriodos( id_servicio ) ) {
                    n=1;
                } else { n++; }
            }
            return n;
        }
    } else {
        qDebug( "Servicios::MPeriodoServicio::getPeriodoActual: Error en el exec de la cola actual" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
}

/*!
 * \fn MPeriodoServicio::getAnoActual( const int id_servicio )
 * Devuelve el numero de año correspondiente al servicio a facturar de acuerdo con los ultimos datos dados
 * \param id_servicio Identificador del servicio
 * \param facturar Permite calcular el periodo siguiente al actual, el cual se debe facturar.
 * \returns identificador del año o -1 si hubo un error.
 */
int MPeriodoServicio::getAnoActual( const int id_servicio, bool facturar ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT periodo, ano, fecha_fin FROM periodo_servicio WHERE id_servicio = %1 ORDER BY fecha_fin DESC LIMIT 1" ).arg( id_servicio ) ) ) {
        if( cola.next() ) {
            if( facturar ) {
                if( ( cola.record().value(0).toInt() + 1 ) > cantidadPeriodos( id_servicio ) ) {
                    return cola.record().value(1).toInt() + 1;
                } else {
                    return cola.record().value(1).toInt();
                }
            } else {
                return cola.record().value(1).toInt();
            }
        } else {
            // No hay ningun registro todavía - Es el primer periodo a registrar
            //qDebug( "Devolviendo el año actual - Ningun registro anterior" );
            return QDate::currentDate().year();
        }
    } else {
        qDebug() << "Servicios::MPeriodoServicio::getPeriodoActual: Error en el exec de la cola actual";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
}

/*!
 * \fn MPeriodoServicio::getFechaInicioPeriodoActual( const int id_servicio )
 * Devuelve la fecha de inicio del periodo correspondiente al servicio a facturar de acuerdo con los ultimos datos dados
 * \param id_servicio Identificador del servicio.
 * \param facturar Calcula los datos para el periodo siguiente al actual.
 * \returns identificador del año o -1 si hubo un error.
 */
QDate MPeriodoServicio::getFechaInicioPeriodoActual( const int id_servicio, bool facturar ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT periodo, fecha_inicio, fecha_fin FROM periodo_servicio WHERE id_servicio = %1 ORDER BY fecha_fin DESC LIMIT 1" ).arg( id_servicio ) ) ) {
        if( cola.next() ) {
            if( facturar ) {
                // Esto supone que la fecha de fin del periodo anterior es correcta!
                return cola.record().value(2).toDate().addDays(1);
            } else {
                return cola.record().value(0).toDate();
            }
        } else {
            // No hay ningun registro todavía - Es el primer periodo a registrar
            return MPeriodoServicio::generarFechaInicioPeriodo( id_servicio, MPeriodoServicio::getPeriodoActual( id_servicio, facturar ), QDate::currentDate().year() );
        }
    } else {
        qDebug() << "Servicios::MPeriodoServicio::getFechaInicioPeriodoActual: Error en el exec de la cola actual";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return QDate();
    }
}

/*!
 * \fn MPeriodoServicio::agregarPeriodoAFacturarNuevo( const int id_servicio )
 * Agrega un nuevo periodo a la lista de periodos de un servicio contolando que los datos sean correctos.
 * @param id_servicio Identificador del servicio que se desea
 * @returns Identificador del periodo del servicio o -1 si hubo un error
 */
int MPeriodoServicio::agregarPeriodoAFacturarNuevo( const int id_servicio ) {
    // Busca el proximo periodo a facturar y lo agrega devolviendo el id agregado
    // Busco si existe un periodo anterior
    int periodo = this->getPeriodoActual( id_servicio, true );
    int ano = this->getAnoActual( id_servicio, true );
    QDate fecha_inicio = this->getFechaInicioPeriodo( id_servicio, periodo, ano );
    QDate fecha_fin = this->obtenerFechaFinPeriodo( id_servicio, fecha_inicio );
    // Verifico
    QDate fecha_ultimo_periodo = this->getUltimaFecha( id_servicio );
    if( fecha_ultimo_periodo.isValid() ) {
        if( fecha_ultimo_periodo >= fecha_inicio ) {
            qDebug( "Error - La fecha de inicio del periodo es anterior a la fecha de fin del periodo anterior." );
            QMessageBox::warning( 0, "Control", "Ya existe un periodo que esta incluido en el periodo que intenta facturar. <br /> Seguramente ya facturo este periodo de este servicio" );
            return -2;
        }
    } else {
        qDebug( "Atencion - La ulima fecha de inicio del periodo no existe - se utilizara la generica." );
    }
    int ultimo_periodo = this->getUltimoPeriodo( id_servicio );
    if( ultimo_periodo != -1 && ultimo_periodo >= periodo && periodo != 1 ) { // La ultima opción es para evitar que de error al cambiar de año
        qDebug( "Error- El numero de periodo es invalido o igual que el periodo que se facturara." );
        return -1;
    }
    return this->agregarPeriodoServicio( id_servicio, periodo, ano, fecha_inicio, fecha_fin );
}


/*!
 * \fn MPeriodoServicio::getFechaInicioPeriodo( const int id_servicio, const int periodo, const int ano )
 * Devuelve la fecha de inicio del periodo solicitado
 * @param id_servicio ID del servicio elegido
 * @param periodo Periodo elegido a solicitar la fecha
 * @param ano Año solicitado
 * @returns Fecha de inicio del periodo solicitado o una fecha invalida si hubo un error.
 */
QDate MPeriodoServicio::getFechaInicioPeriodo( const int id_servicio, const int periodo, const int ano ) {
    // Busco si existe en la base de datos
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha_inicio FROM periodo_servicio WHERE id_servicio = %1 AND periodo = %2 AND ano = %3" ).arg( id_servicio ).arg( periodo ).arg( ano ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        } else {
            // Genero la fecha ya que no existe en la base de datos
            return generarFechaInicioPeriodo( id_servicio, periodo, ano );
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de fecha de inicio de un periodo para un servicio";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return QDate();
    }
    return QDate();
}

/*!
 * \fn MPeriodoServicio::getUltimaFecha( const int id_servicio )
 * Devuelve la ultima fecha de fin del servicio solicitado
 * @param id_servicio ID del servicio elegido
 * @returns Ultima fecha o una fecha invalida si no existe o hubo un error.
 */
QDate MPeriodoServicio::getUltimaFecha( const int id_servicio ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha_fin, periodo, ano FROM periodo_servicio WHERE id_servicio = %1 ORDER BY ano DESC, periodo DESC LIMIT 1" ).arg( id_servicio ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        } else {
            // No existe una fecha
            qDebug( "No existe una fecha de ultimo periodo de servicio elegido." );
            return QDate();
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de fecha de inicio de un periodo para un servicio";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return QDate();
    }
    return QDate();
}

/*!
 * \fn MPeriodoServicio::generarFechaInicioPeriodo( const int id_servicio, const int periodo, const int ano )
 * Devuelve la fecha teorica del inicio del periodo segun el servicio solicitado
 * @param id_servicio ID del servicio elegido
 * @param periodo numero de periodo del servicio en el año elegido
 * @param ano Año elegido
 * @returns Fecha de inicio o fecha invalida si hubo un error.
 */
QDate MPeriodoServicio::generarFechaInicioPeriodo( const int id_servicio, const int periodo, const int ano ) {
    QDate fecha = QDate();
    QDate fecha_inicio_servicio = MServicios::getFechaAlta( id_servicio );
    switch( MServicios::obtenerPeriodo( id_servicio ) ) {
        case MServicios::Semanal:
        {
            fecha = fecha_inicio_servicio;
            fecha = fecha.addMonths( floor( periodo / 4 ) );
            int dias_mes = fecha.daysInMonth();
            if( ( periodo % 4 ) !=  0 ) {
                fecha = fecha.addDays( floor( dias_mes / 4 ) );
            }
            break;
        }
        case MServicios::Quincenal:
        {
            fecha = fecha_inicio_servicio;
            fecha = fecha.addMonths( floor( periodo / 2 ) );
            int dias_mes = fecha.daysInMonth();
            if( ( periodo % 2 ) !=  0 ) {
                fecha = fecha.addDays( floor( dias_mes / 2 ) );
            }
            break;
        }
        case MServicios::Mensual:
        {
            fecha = QDate( ano, 1, 1 ).addMonths( ( periodo - 1 ) );
            break;
        }
        case MServicios::BiMensual:
        {
            fecha = QDate( ano, 1, 1 ).addMonths( ( periodo - 1 ) * 2 );
            break;
        }
        case MServicios::Trimestral:
        {
            fecha = QDate( ano, 1, 1 ).addMonths( ( periodo - 1 ) * 3 );
            break;
        }
        case MServicios::Cuatrimestral:
        {
            fecha = QDate( ano, 1, 1 ).addMonths( ( periodo - 1 ) * 4 );
            break;
        }
        case MServicios::Seximestral:
        {
            fecha = QDate( ano, 1, 1 ).addMonths( ( periodo - 1 ) * 6 );
            break;
        }
        case MServicios::Anual:
        {
            if( periodo > 1 ) { qDebug( "Periodo > 1 para tipo de periodo anual. Erroneo. Corregido." ); }
            fecha = QDate( ano, QDate::currentDate().month(), 1 ); break;
        }
        default: { qDebug( "Tipo de Periodo invalido" ); return QDate(); break; }
    }
    // Chequeo que la fecha de inicio del servicio sea menor que la buscada

    if( fecha_inicio_servicio > fecha ) {
        qDebug() << "Atencion - la fecha de inicio buscada es menor a la del alta del servicio.";
        qDebug() << fecha_inicio_servicio.toString();
        qDebug() << fecha.toString();
        fecha = fecha_inicio_servicio;
    }

    return fecha;
}

QDate MPeriodoServicio::getFechaEmision(const int id_servicio, const QDate fecha_inicio_periodo )
{
    int dia = MServicios::getDiaFacturacion( id_servicio );
    // Si es el día 1 tengo que facturarlo en la fecha de inicio
    if( dia == 1 ) {
        return fecha_inicio_periodo;
    } else if( dia > 1 ) {
        qDebug( "Dia de facutracion > 1 " );
        dia = dia - 1;
        QDate fecha = fecha_inicio_periodo.addDays( dia );
        return fecha;
    } else {
        qDebug( "Error al buscar el día de facturación de un servicio" );
        return fecha_inicio_periodo;
    }
    qDebug( "Error de logica, dia < 1" );
    return QDate();
}


/*!
 * \fn MPeriodoServicio::getUltimoPeriodo( const int id_servicio )
 * Busca el ultimo periodo facturado para el servicio solicitado
 * \param id_servicio Identificador del servicio
 * \returns Numero de periodo o -1 si hubo error o no existe.
 */
int MPeriodoServicio::getUltimoPeriodo(const int id_servicio) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha_fin, periodo, ano FROM periodo_servicio WHERE id_servicio = %1 ORDER BY ano DESC, periodo DESC LIMIT 1" ).arg( id_servicio ) ) ) {
        if( cola.next() ) {
            return cola.record().value(1).toInt();
        } else {
            // No existe una fecha
            qDebug( "No existe un ultimo numero de periodo del servicio elegido." );
            return -1;
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de ultimo periodo para un servicio";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
    return -1;
}

int MPeriodoServicio::cantidadPeriodos(const int id_servicio)
{
    switch( MServicios::obtenerPeriodo( id_servicio ) ) {
        case MServicios::Semanal:
        {
            return 48;
            break;
        }
        case MServicios::Quincenal:
        {

            return 24;
            break;
        }
        case MServicios::Mensual:
        {
            return 12;
            break;
        }
        case MServicios::BiMensual:
        {
            return 6;
            break;
        }
        case MServicios::Trimestral:
        {
            return 4;
            break;
        }
        case MServicios::Cuatrimestral:
        {
            return 3;
            break;
        }
        case MServicios::Seximestral:
        {
            return 2;
            break;
        }
        case MServicios::Anual:
        {
            return 1;
            break;
        }
        default:
        {
            qDebug( "Tipo de Periodo invalido" );
            return -1;
            break;
        }
    }
}

/**
 * @brief MPeriodoServicio::getPeriodoSegunFecha
 * @param id_servicio Identificador del servicio
 * @param fecha_calculo Fecha donde se desea calcular el periodo
 * @return El numero de período que corresponde segun una fecha
 */
int MPeriodoServicio::getPeriodoSegunFecha(const int id_servicio, QDate fecha_calculo)
{
    QDate fecha_alta_servicio = MServicios::getFechaAlta( id_servicio );
    int cant_dias_periodo = diasEnPeriodoServicio( id_servicio, fecha_calculo );
    QDate inicio_ano = QDate( QDate::currentDate().year(), 1, 1 );
    int t = 0; double u = 0.0;

    if( fecha_alta_servicio < inicio_ano ) {
        // Calculo
        t = inicio_ano.daysTo( fecha_calculo );
    } else {
        t = fecha_alta_servicio.daysTo( fecha_calculo );
    }
    if( t >= cant_dias_periodo ) {
        u = ((double) t)/cant_dias_periodo;
        u++;
    } else {
        u = 1.0;
    }
    int n = round( u );
    return n;

}


