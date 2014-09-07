#ifndef MUNIDADESPRODUCTOSARBOL_H
#define MUNIDADESPRODUCTOSARBOL_H

#include <QProxyModel>
class MUnidadesProductos;

class MUnidadesProductosArbol : public QProxyModel
{
    Q_OBJECT
public:
    MUnidadesProductosArbol( QObject *parent = 0 );

private:
    /**
     * @brief _adb
     * Mapa del elemento con la base de datos
     */
    QHash<int, QHash<int, int> > _adb;

    MUnidadesProductos *mup;
    
};

#endif // MUNIDADESPRODUCTOSARBOL_H
