#include <QString>
#include <QtTest>

#include "mproductostotales.h"

class MProductosTotalesTest : public QObject
{
    Q_OBJECT
    
public:
    MProductosTotalesTest();
    
private Q_SLOTS:
    void initTestCase();
    void testAgregarProducto();
    void testAgregarProducto_data();
    void cleanupTestCase();
private:
    MProductosTotales *mpt;
};

MProductosTotalesTest::MProductosTotalesTest()
{}

void MProductosTotalesTest::initTestCase()
{
  mpt = new MProductosTotales( this, new QMap<int,QString>() );
}

void MProductosTotalesTest::cleanupTestCase()
{
    delete mpt;
}

void MProductosTotalesTest::testAgregarProducto()
{
    // Cargo cada producto
    QFETCH( double, cantidad );
    QFETCH( QString, producto );
    QFETCH( double, precio_unitario );

    int cant_anterior = mpt->rowCount();
    mpt->agregarItem( cantidad, producto, precio_unitario );
    double subtotal = mpt->data( mpt->index( cant_anterior, 3 ), Qt::EditRole ).toDouble();
    double total = mpt->total();
    int cant_nueva = mpt->rowCount();

    QCOMPARE( subtotal, total );
    QVERIFY( cant_anterior == cant_nueva - 1 );
}

void MProductosTotalesTest::testAgregarProducto_data()
{
    QTest::addColumn<double>("cantidad");
    QTest::addColumn<QString>("producto");
    QTest::addColumn<double>("precio_unitario");
    QTest::newRow("0") << 1.0 << "Producto 1" << 12.40;
    QTest::newRow("1") << 2.0 << "Producto 2" << 30.00;
    QTest::newRow("2") << 3.0 << "Producto 3" << 25.00;
}

QTEST_APPLESS_MAIN(MProductosTotalesTest)

#include "tst_mproductostotalestest.moc"
