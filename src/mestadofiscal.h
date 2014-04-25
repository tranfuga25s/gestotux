#ifndef MESTADOFISCAL_H
#define MESTADOFISCAL_H

#include <QSqlTableModel>

class MEstadoFiscal : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit MEstadoFiscal( QObject *parent = 0 );

};

#endif // MESTADOFISCAL_H
