#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "mproductos.h"

class ProductosTest : public QObject
{
    Q_OBJECT
    
public:
    ProductosTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCodigoRepetido();
    void testCodigoRepetido_data();

private:
    MProductos *mp;

};

ProductosTest::ProductosTest()
{
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( "gestotux.test.database" );
    mp = new MProductos( 0 );
}

void ProductosTest::initTestCase()
{}

void ProductosTest::cleanupTestCase()
{
    QSqlQuery cola;
    cola.exec( "DELETE * FROM productos");
    delete mp;
}

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
    QEXPECT_FAIL("repetido", "Fallo la inserción de producto con codigo repetido", Continue );
    QVERIFY2( true == mp->agregarProducto( codigo, nombre, costo, venta, stock, categoria, descripcion, marca, modelo ), "Falló aqui!" );
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
    QTest::newRow("original") << "1" << "test1" << 1.0 << 1.1 << 1 << 0 << "descripcion1" << "marca1" << "modelo1";
    QTest::newRow("repetido") << "1" << "test1" << 1.0 << 1.1 << 1 << 0 << "descripcion1" << "marca1" << "modelo1";
}

QTEST_APPLESS_MAIN(ProductosTest)

#include "tst_productostest.moc"
