#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include <QSqlRecord>

#include "../edatabasetest.h"
#include "mproveedor.h"
#include "ecbproductos.h"

class ECBProductosTest : public QObject, private EDatabaseTest
{
    Q_OBJECT
    
public:
    ECBProductosTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void testFiltroProveedor();
    void testFiltroProveedor_data();
};

ECBProductosTest::ECBProductosTest()
{
    this->tablas << "productos"
                 << "proveedor"
                 << "compras"
                 << "compras_productos";
}

void ECBProductosTest::init() { EDatabaseTest::iniciarTablas(); }

void ECBProductosTest::initTestCase() { EDatabaseTest::generarTablas(); }

void ECBProductosTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void ECBProductosTest::cleanup() { EDatabaseTest::vaciarTablas(); }

void ECBProductosTest::testFiltroProveedor()
{
    QFETCH( int, id_proveedor );
    QFETCH( int, conteo );

    ECBProductos *ecb = new ECBProductos();
    ecb->filtrarPorProveedor( id_proveedor );
    int conteo_real = ecb->count();
    delete ecb;

    QCOMPARE( conteo_real, conteo );
}

void ECBProductosTest::testFiltroProveedor_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<int>("conteo");
    QTest::newRow("Proveedor 1") << 1 << 2;
    QTest::newRow("Proveedor 2") << 2 << 2;
}

QTEST_MAIN(ECBProductosTest)

#include "tst_ecbproductostest.moc"
