#ifndef MITEMSPRESUPUESTO_H
#define MITEMSPRESUPUESTO_H

#include <QSqlRelationalTableModel>

class MItemsPresupuesto : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MItemsPresupuesto(QObject *parent = 0);

signals:

public slots:

};

#endif // MITEMSPRESUPUESTO_H
