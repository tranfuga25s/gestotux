#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "../edatabasetest.h"
#include "mproveedor.h"

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
    this->tablas << "productos" << "proveedor" << "compras";
}

void ECBProductosTest::init() { EDatabaseTest::iniciarTablas(); }

void ECBProductosTest::initTestCase() { EDatabaseTest::generarTablas(); }

void ECBProductosTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void ECBProductosTest::cleanup() { EDatabaseTest::vaciarTablas(); }

void ECBProductosTest::testFiltroProveedor()
{
    QFETCH( int, id_proveedor );
    QFETCH( int, conteo );
    QVERIFY2(true, "Failure");
}

void ECBProductosTest::testFiltroProveedor_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<int>("conteo");
    MProveedor *mp = new MProveedor( 0 );
    mp->select();
    qDebug() << "Cantidad de proveedores: " << mp->rowCount();
    for( int i=0; i<mp->rowCount(); i++ ) {
        int id_proveedor = mp->data( mp->index( i, 0 ), Qt::DisplayRole ).toInt();
        QTest::newRow( QString::number( i ).toAscii() ) << id_proveedor << 1;
    }
    delete mp;
}

QTEST_MAIN(ECBProductosTest)

#include "tst_ecbproductostest.moc"
