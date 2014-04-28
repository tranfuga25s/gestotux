#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "mproductos.h"
#include "../edatabasetest.h"

class ProductosTest : public QObject, private EDatabaseTest
{
    Q_OBJECT
    
public:
    ProductosTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void testCodigoRepetido();
    void testCodigoRepetido_data();

private:
    MProductos *mp;

};

ProductosTest::ProductosTest()
{
    this->tablas << "categorias_productos" << "productos";
}

void ProductosTest::init() { EDatabaseTest::iniciarTablas(); }

void ProductosTest::initTestCase() { EDatabaseTest::generarTablas(); }

void ProductosTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void ProductosTest::cleanup() { EDatabaseTest::vaciarTablas(); }

void ProductosTest::testCodigoRepetido()
{
    QFETCH( QString, codigo );
    QFETCH( QString, nombre );
    QFETCH( double, costo );
    QFETCH( double, venta );
    QFETCH( int, stock );
    QFETCH( int, categoria );
    QFETCH( QString, descripcion );
    QFETCH( QString, modelo );
    QFETCH( QString, marca );
    QFETCH( bool, resultado );
    if( resultado ) {
        QVERIFY( mp->agregarProducto( codigo, nombre, costo, venta, stock, categoria, descripcion, marca, modelo ) > 0 );
    } else {
        QCOMPARE( mp->agregarProducto( codigo, nombre, costo, venta, stock, categoria, descripcion, marca, modelo ), false );
    }
}

void ProductosTest::testCodigoRepetido_data()
{
    QTest::addColumn<QString>("codigo");
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<double>("costo");
    QTest::addColumn<double>("venta");
    QTest::addColumn<int>("stock");
    QTest::addColumn<int>("categoria");
    QTest::addColumn<QString>("descripcion");
    QTest::addColumn<QString>("marca");
    QTest::addColumn<QString>("modelo");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("original") << "1" << "test1" << 1.0 << 1.1 << 1 << 0 << "descripcion1" << "marca1" << "modelo1" << true;
    QTest::newRow("repetido") << "1" << "test1" << 1.0 << 1.1 << 1 << 0 << "descripcion1" << "marca1" << "modelo1" << false;
}

QTEST_MAIN(ProductosTest)

#include "tst_productostest.moc"
