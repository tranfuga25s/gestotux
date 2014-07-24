#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "mcategorias.h"
#include "mproductos.h"
#include "../edatabasetest.h"

class UnidadesProductosTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    UnidadesProductosTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

private:
    MProductos *mp;
    bool doClean;

};

/*!
 * \brief UnidadesProductosTest::UnidadesProductosTest
 */
UnidadesProductosTest::UnidadesProductosTest()
{
    this->tablas << "categorias_productos"
                 << "productos"
                 << "compras"
                 << "compras_productos";
}

void UnidadesProductosTest::init() { EDatabaseTest::init(); doClean = true; }

void UnidadesProductosTest::initTestCase() { EDatabaseTest::initTestCase(); }

void UnidadesProductosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void UnidadesProductosTest::cleanup() { if( this->doClean ) { EDatabaseTest::cleanup(); } }


QTEST_MAIN(UnidadesProductosTest)

#include "tst_unidadesproductostest.moc"
