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
    void testMostrarStock();
    void testMostrarStock_data();

private:
     ECBProductos *ecb;
};

/*!
 * \brief ECBProductosTest::ECBProductosTest
 */
ECBProductosTest::ECBProductosTest()
{
    this->tablas << "productos"
                 << "proveedor"
                 << "compras"
                 << "compras_productos";
    ecb = new ECBProductos();
}

/*!
 * \brief ECBProductosTest::~ECBProductosTest
 */
ECBProductosTest::~ECBProductosTest() {
    delete ecb;
}

/*!
 * \brief ECBProductosTest::init
 */
void ECBProductosTest::init() { EDatabaseTest::init(); }

/*!
 * \brief ECBProductosTest::initTestCase
 */
void ECBProductosTest::initTestCase() { EDatabaseTest::initTestCase(); }

/*!
 * \brief ECBProductosTest::cleanupTestCase
 */
void ECBProductosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

/*!
 * \brief ECBProductosTest::cleanup
 */
void ECBProductosTest::cleanup() { EDatabaseTest::cleanup(); }

/*!
 * \brief ECBProductosTest::testFiltroProveedor
 */
void ECBProductosTest::testFiltroProveedor()
{
    QFETCH( int, id_proveedor );
    QFETCH( int, conteo );

    ecb->filtrarPorProveedor( id_proveedor );
    int conteo_real = ecb->count();

    QCOMPARE( conteo_real, conteo );
}

/*!
 * \brief ECBProductosTest::testFiltroProveedor_data
 */
void ECBProductosTest::testFiltroProveedor_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<int>("conteo");
    QTest::newRow("Proveedor 1") << 1 << 1;
    QTest::newRow("Proveedor 2") << 2 << 1;
}

#include "preferencias.h"
/*!
 * \brief ECBProductosTest::testMostrarStock
 * Permite probar la característica de mostrar el stock al lado del nombre del producto
 */
void ECBProductosTest::testMostrarStock()
{
    QFETCH( bool, habilitado );
    QFETCH( int, posicion );
    QFETCH( double, stock );

    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    p->setValue( "mostrar-stock-lista", habilitado );
    p->endGroup();
    p->endGroup();
    p=0;

    ECBProductos *ecb = new ECBProductos();
    QTest::qWait( 1000 );
    if( habilitado ) {
        QString texto = ecb->itemText( posicion );
        QVERIFY( !texto.isEmpty() );
        QVERIFY( texto.contains( QString( "(%L1)" ).arg( stock ) ) );
    } else {
        QString texto = ecb->itemText( posicion );
        QVERIFY( !texto.isEmpty() );
        QVERIFY( texto.contains( "(" ) == false );
    }
    delete ecb;
}

/*!
 * \brief ECBProductosTest::testMostrarStock_data
 */
void ECBProductosTest::testMostrarStock_data()
{
    QTest::addColumn<bool>("habilitado");
    QTest::addColumn<int>("posicion");
    QTest::addColumn<double>("stock");
    QTest::newRow("ConStock") << true << 1 << 1.0;
    QTest::newRow("SinStock") << false << 1 << 0.0;
}

QTEST_MAIN(ECBProductosTest)

#include "tst_ecbproductostest.moc"
