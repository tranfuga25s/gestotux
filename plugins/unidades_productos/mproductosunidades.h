#ifndef MPRODUCTOSUNIDADES_H
#define MPRODUCTOSUNIDADES_H

#include <QSqlTableModel>

class MProductosUnidades : public QSqlTableModel
{
Q_OBJECT

public:
    MProductosUnidades( QObject *parent = 0 );
    QVector<int> getUnidadesSegunProducto( const int id_producto );
    
private:
    static const int predeterminada = 1;
};

#endif // MPRODUCTOSUNIDADES_H
