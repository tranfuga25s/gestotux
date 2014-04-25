#ifndef MVPAGOS_H
#define MVPAGOS_H

#include <QSqlRelationalTableModel>
#include <QDate>

class MVPagos : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MVPagos(QObject *parent = 0);
    QVariant data(const QModelIndex& item, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void filtrarCancelados( bool estado );
    void filtrarNoPagados( bool estado );

private:
    bool _cancelados;
    bool _no_pagados;
    void filtrar();
};

#endif // MVPAGOS_H
