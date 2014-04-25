#ifndef MEQUIPOS_H
#define MEQUIPOS_H

#include <QSqlTableModel>

class MEquipos : public QSqlTableModel
{
    Q_OBJECT
public:
    MEquipos(QObject *parent = 0);

};

#endif // MEQUIPOS_H
