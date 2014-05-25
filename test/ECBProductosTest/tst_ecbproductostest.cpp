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
   /* void testFiltroProveedor();
    void testFiltroProveedor_data();
    void testMostrarStock();
    void testMostrarStock_data();*/
    void testECBProductosModel();
    void testECBProductosModel_data();
    void testECBProductosModelAgregarItem();
    void testECBProductosModelAgregarItem_data();
    void testECBProductosConFiltrado();
    void testECBModeloCambiarAnterior();
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
}

/*!
 * \brief ECBProductosTest::~ECBProductosTest
 */
ECBProductosTest::~ECBProductosTest() {}


void ECBProductosTest::init() { EDatabaseTest::init(); }
void ECBProductosTest::initTestCase() { EDatabaseTest::initTestCase(); }
void ECBProductosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }
void ECBProductosTest::cleanup() { EDatabaseTest::cleanup(); }

/*!
 * \brief ECBProductosTest::testFiltroProveedor
 */
/*void ECBProductosTest::testFiltroProveedor()
{
    QFETCH( int, id_proveedor );
    QFETCH( int, conteo );

    ECBProductos *ecb = new ECBProductos();
    ecb->filtrarPorProveedor( id_proveedor );
    int conteo_real = ecb->count();

    QCOMPARE( conteo_real, conteo );
    delete ecb;
}*/

/*!
 * \brief ECBProductosTest::testFiltroProveedor_data
 */
/*void ECBProductosTest::testFiltroProveedor_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<int>("conteo");
    QTest::newRow("Proveedor 1") << 1 << 1;
    QTest::newRow("Proveedor 2") << 2 << 1;
}*/

#include "preferencias.h"
/*!
 * \brief ECBProductosTest::testMostrarStock
 * Permite probar la característica de mostrar el stock al lado del nombre del producto
 */
/*void ECBProductosTest::testMostrarStock()
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
    p->endGroup();
    p->sync();
    p=0;

    ECBProductos *ecb = new ECBProductos();
    QTest::qWait( 1000 );
    if( habilitado ) {
        ecb->setCurrentIndex( posicion );
        QString texto = ecb->currentText();
        QVERIFY( !texto.isEmpty() );
        QVERIFY2( texto.contains( QString( "(%L1)" ).arg( stock ) ), texto.toLocal8Bit() );
    } else {
        QString texto = ecb->itemText( posicion );
        QVERIFY( !texto.isEmpty() );
        QVERIFY2( texto.contains( "(" ) == false, texto.toLocal8Bit() );
    }
    delete ecb;
}*/

/*!
 * \brief ECBProductosTest::testMostrarStock_data
 */
/*void ECBProductosTest::testMostrarStock_data()
{
    QTest::addColumn<bool>("habilitado");
    QTest::addColumn<int>("posicion");
    QTest::addColumn<double>("stock");
    QTest::newRow("SinStock") << false << 1 << 0.0;
    QTest::newRow("ConStock") << true << 1 << 1.0;

}*/

#include "ecbproductosmodel.h"
/*!
 * \brief ECBProductosTest::testECBProductosModel
 */
void ECBProductosTest::testECBProductosModel()
{
    ECBProductosModel *m = new ECBProductosModel();
    m->inicializar();

    QFETCH( int, i );
    QFETCH( int, id_producto );
    QFETCH( QString, codigo );
    QFETCH( QString, nombre );
    QFETCH( double, stock );
    QFETCH( bool, habilitado );
    QFETCH( int, id_proveedor );

    QCOMPARE( m->data( m->index( i, 0 ) ).toInt()   , id_producto  );
    QCOMPARE( m->data( m->index( i, 1 ) ).toString(), codigo       );
    QCOMPARE( m->data( m->index( i, 2 ) ).toString(), nombre       );
    QCOMPARE( m->data( m->index( i, 3 ) ).toDouble(), stock        );
    QCOMPARE( m->data( m->index( i, 4 ) ).toBool()  , habilitado   );
    QCOMPARE( m->data( m->index( i, 5 ) ).toInt()   , id_proveedor );
}

/*!
 * \brief ECBProductosTest::testECBProductosModel_data
 */
void ECBProductosTest::testECBProductosModel_data()
{
    QTest::addColumn<int>("i");
    QTest::addColumn<int>("id_producto");
    QTest::addColumn<QString>("codigo");
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<double>("stock");
    QTest::addColumn<bool>("habilitado");
    QTest::addColumn<int>("id_proveedor");
    QTest::newRow("Producto1") << 0 << 1 << "1" << "Producto 1" << 1.0 << true << 1;
    QTest::newRow("Producto2") << 1 << 2 << "2" << "Producto 2" << 1.0 << true << 2;
}

/*!
 * \brief ECBProductosTest::testECBProductosModelAgregarItem
 */
void ECBProductosTest::testECBProductosModelAgregarItem()
{
    ECBProductosModel *m = new ECBProductosModel();
    m->inicializar();

    QFETCH( int    , i            );
    QFETCH( int    , id_producto  );
    QFETCH( QString, codigo       );
    QFETCH( QString, nombre       );
    QFETCH( double , stock        );
    QFETCH( bool   , habilitado   );
    QFETCH( int    , id_proveedor );

    if( i < 0 ) {
        m->agregarItem( nombre );
        i = m->rowCount()-1;
    }

    QCOMPARE( m->data( m->index( i, 0 ) ).toInt()   , id_producto  );
    QCOMPARE( m->data( m->index( i, 1 ) ).toString(), codigo       );
    QCOMPARE( m->data( m->index( i, 2 ) ).toString(), nombre       );
    QCOMPARE( m->data( m->index( i, 3 ) ).toDouble(), stock        );
    QCOMPARE( m->data( m->index( i, 4 ) ).toBool()  , habilitado   );
    QCOMPARE( m->data( m->index( i, 5 ) ).toInt()   , id_proveedor );

    delete m;
}

/*!
 * \brief ECBProductosTest::testECBProductosModelAgregarItem_data
 */
void ECBProductosTest::testECBProductosModelAgregarItem_data()
{
    QTest::addColumn<int>("i");
    QTest::addColumn<int>("id_producto");
    QTest::addColumn<QString>("codigo");
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<double>("stock");
    QTest::addColumn<bool>("habilitado");
    QTest::addColumn<int>("id_proveedor");
    QTest::newRow("Producto1") << 0 << 1 << "1" << "Producto 1" << 1.0 << true << 1;
    QTest::newRow("Producto2") << 1 << 2 << "2" << "Producto 2" << 1.0 << true << 2;
    QTest::newRow("ProductoInsertado") << -1 << -1 << "-1" << "Producto insertado" << 1.0 << true << 0;
}

#include "ecbproductosfilter.h"
/*!
 * \brief ECBProductosTest::testECBProductosConFiltrado
 * Permite probar la salida de los modelos de filtrado
 */
void ECBProductosTest::testECBProductosConFiltrado()
{
    ECBProductosModel *mp = new ECBProductosModel();
    mp->inicializar();

    ECBProductosFilter *modelo = new ECBProductosFilter( this );
    modelo->setSourceModel( mp );

    modelo->setearNoMostrarProductosSinStock( true );
    for( int i=0; i<modelo->rowCount(); i++ ) {
        QVERIFY( modelo->data( modelo->index( i, ECBProductosModel::Stock ) ).toDouble() > 0.0 );
    }

    modelo->setearNoMostrarProductosDeshabilitados( true );
    for( int i=0; i<modelo->rowCount(); i++ ) {
        QVERIFY( modelo->data( modelo->index( i, ECBProductosModel::Habilitado ) ).toBool() == true );
    }

    int id_proveedor = 1;
    modelo->setearIdProveedor( id_proveedor );
    for( int i=0; i<modelo->rowCount(); i++ ) {
        QCOMPARE( modelo->data( modelo->index( i, ECBProductosModel::IdProveedor ) ).toInt(), id_proveedor );
    }

}

#include "mproductostotales.h"
/*!
 * \brief ECBProductosTest::testECBModeloCambiarAnterior
 * Testea que funcione correctamente el cambiador de valores anteriores a nuevo en mproductostotales
 */
void ECBProductosTest::testECBModeloCambiarAnterior()
{
    ECBProductosModel *mp = new ECBProductosModel();
    mp->inicializar();
    int cantidad_mp = mp->rowCount();

    MProductosTotales *mpt = new MProductosTotales();
    mpt->setearListaProductos( mp );
    mpt->calcularTotales( false );
    int cantidad_mpt = mpt->rowCount();

    mpt->agregarItem( 1.0, "ProductoAgregadoExtra", 10.0 );

    QCOMPARE( mpt->rowCount(), 1 );
    QVERIFY( ( mpt->rowCount() - cantidad_mpt ) == 1 ); // Verifica que se haya agregado un elemento
    QVERIFY( ( mp->rowCount() - cantidad_mp ) == 1 ); // Verifica que se haya agregado un elemento

    QCOMPARE( mpt->data( mpt->index( mpt->rowCount()-1, 1 ), Qt::EditRole ).toInt(), -1 ); // Verifico que el ID sea negativo

    mp->arreglarItemTemporal( -1, 1000 );
    mpt->arreglarIdProductoAgregado( -1, 1000 );

    QCOMPARE( mp->data( mp->index( mp->rowCount()-1, ECBProductosModel::Ids ), Qt::EditRole ).toInt(), 1000 ); // Verifico que el ID se haya cambiado
    QCOMPARE( mpt->data( mpt->index( mpt->rowCount() - 1, 1 ), Qt::EditRole ).toInt(), 1000 ); // Verifico que el ID sea negativo

    delete mpt;
    delete mp;
}

QTEST_MAIN(ECBProductosTest)






#include "tst_ecbproductostest.moc"