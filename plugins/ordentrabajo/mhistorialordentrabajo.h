#ifndef MHISTORIALORDENTRABAJO_H
#define MHISTORIALORDENTRABAJO_H

#include <QSqlRelationalTableModel>
#include <QHash>
#include <QDateTime>

class MHistorialOrdenTrabajo : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MHistorialOrdenTrabajo( QObject *parent = 0 );
    ~MHistorialOrdenTrabajo();

    void setearRelacionTecnico();
    void setearRelacionTipo();

    bool tieneRelaciones() const { return _tiene_relaciones; }

    QVariant data( const QModelIndex &item, int role ) const;
    int columnCount(const QModelIndex &parent) const;
    
    int agregarHistorial( const int id_orden_trabajo, QDateTime fecha_hora, QString descripcion, double costo, const int tipo_operacion, const int id_tecnico );
    bool eliminarHistorial( const int id_historial );

    void setearOrdenTrabajo( const int id_orden_trabajo );
    void mostrarCostosSumados( bool activar = true );

private:
    bool _mostrar_suma_costos;
    bool _tiene_relaciones;
    QMap<int,double> *_sumas;
    int _id_orden_trabajo;
};

#endif // MHISTORIALORDENTRABAJO_H
