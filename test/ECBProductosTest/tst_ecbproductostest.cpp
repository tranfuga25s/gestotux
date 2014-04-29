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
    ~ECBProductosTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void testFiltroProveedor();
    void testFiltroProveedor_data();

private:
     ECBProductos *ecb;
};

ECBProductosTest::ECBProductosTest()
{
    this->tablas << "productos"
                 << "proveedor"
                 << "compras"
                 << "compras_productos";
    ecb = new ECBProductos();
}

ECBProductosTest::~ECBProductosTest() {
    delete ecb;
}

void ECBProductosTest::init() { EDatabaseTest::iniciarTablas(); }

void ECBProductosTest::initTestCase() { EDatabaseTest::generarTablas(); }

void ECBProductosTest::cleanupTestCase() {} //EDatabaseTest::borrarTablas(); }

void ECBProductosTest::cleanup() {} //EDatabaseTest::vaciarTablas(); }

void ECBProductosTest::testFiltroProveedor()
{
    QFETCH( int, id_proveedor );
    QFETCH( int, conteo );

    ecb->filtrarPorProveedor( id_proveedor );
    int conteo_real = ecb->count();

    QCOMPARE( conteo_real, conteo );
}

void ECBProductosTest::testFiltroProveedor_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<int>("conteo");
    QTest::newRow("Proveedor 1") << 1 << 1;
    QTest::newRow("Proveedor 2") << 2 << 1;
}

QTEST_MAIN(ECBProductosTest)

#include "tst_ecbproductostest.moc"
