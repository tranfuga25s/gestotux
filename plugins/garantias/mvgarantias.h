#ifndef MVGARANTIAS_H
#define MVGARANTIAS_H

#include <QSqlTableModel>

class MVGarantias : public QSqlTableModel
{
    Q_OBJECT
public:
    MVGarantias( QObject *parent = 0 );
    QVariant data(const QModelIndex &idx, int role) const;
};

#endif // MVGARANTIAS_H
