#ifndef MPERIODOSERVICIO_H
#define MPERIODOSERVICIO_H

#include <QSqlRelationalTableModel>
//#include <QObject>
#include <QDate>

/*!
  \brief Clase para administrar los periodos de los servicios

  Clase que permite la administración y calculos sobre y desde los periodos de los servicios
  \author Esteban Zeller <tranfuga25s@gmail.com>
  \date
 */
class MPeriodoServicio : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MPeriodoServicio(QObject *parent = 0);
    void inicializar();
    void relacionar();
    QVariant data(const QModelIndex &item, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;


    int agregarPeriodoAFacturarNuevo( const int id_servicio );
    int getPeriodoActual( const int id_servicio, bool facturar = false );
    int getAnoActual( const int id_servicio, bool facturar = false  );
    QDate getFechaInicioPeriodoActual( const int id_servicio, bool facturar = false );
    QDate generarFechaInicioPeriodo( const int id_servicio, const int periodo, const int ano );
    QDate getFechaEmision(const int id_servicio, const QDate fecha_inicio_periodo );

    static int agregarPeriodoServicio( const int id_servicio, const int periodo, const int ano, const QDate fecha_inicio, const QDate fecha_fin );
    static QDate ultimaFechaDeServicio( const int id_servicio );
    static QDate obtenerFechaFinPeriodo( const int id_servicio, const QDate fecha_inicio );
    static int diasEnPeriodo( const int id_servicio, const QDate fecha_inicio = QDate::currentDate() );
    static bool existeFacturacion( const int id_servicio );

private:
    static int diasEnPeriodoServicio( const int tipo_periodo, QDate fecha_calculo );
    QDate getFechaInicioPeriodo( const int id_servicio, const int periodo, const int ano );
    QDate getUltimaFecha( const int id_servicio );
    int getUltimoPeriodo( const int id_servicio );
    int cantidadPeriodos( const int id_servicio );

};

#endif // MPERIODOSERVICIO_H
