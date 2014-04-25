#ifndef MTECNICOS_H
#define MTECNICOS_H

#include <QSqlTableModel>

class MTecnicos : public QSqlTableModel
{
    Q_OBJECT
public:
    MTecnicos( QObject *parent = 0 );
    QVariant data(const QModelIndex &idx, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    static bool existe( const QString nombre );
    bool tieneDatosRelacionados( const int id_tecnico );
};

#endif // MTECNICOS_H
