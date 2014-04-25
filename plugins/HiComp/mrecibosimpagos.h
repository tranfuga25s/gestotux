#ifndef MRECIBOSIMPAGOS_H
#define MRECIBOSIMPAGOS_H

#include <QSqlTableModel>

class MRecibosImpagos : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit MRecibosImpagos(QObject *parent = 0);
    QVariant data(const QModelIndex &idx, int role) const;
    
};

#endif // MRECIBOSIMPAGOS_H
