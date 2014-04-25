#ifndef MVPRESUPUESTOS_H
#define MVPRESUPUESTOS_H

#include <QSqlRelationalTableModel>

class MVPresupuestos : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MVPresupuestos(QObject *parent = 0);
    QVariant data(const QModelIndex& idx, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // MVPRESUPUESTOS_H
