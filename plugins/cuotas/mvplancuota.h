#ifndef MVPLANCUOTA_H
#define MVPLANCUOTA_H

#include <QSqlTableModel>

class MVPlanCuota : public QSqlTableModel
{
    Q_OBJECT
public:
    MVPlanCuota( QObject *parent = 0 );

    QVariant data(const QModelIndex &idx, int role) const;
    
};

#endif // MVPLANCUOTA_H
