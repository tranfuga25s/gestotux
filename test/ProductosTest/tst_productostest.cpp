#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "mcategorias.h"
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
    void testIdsSegunCategoria();
    void testIdsSegunCategoria_data();
    void testIdsSegunCategoriaInvalida();
    void testListadoCategorias();
    void testListadoCategorias_data();
    void testIdsSegunProveedor();
    void testIdsSegunProveedor_data();
    void testIdsSegunProveedorInvalida();
    void testAutocompletadoMarcaProveedor();
    void testAutocompletadoMarcaProveedor_data();

private:
    MProductos *mp;

};

/*!
 * \brief ProductosTest::ProductosTest
 */
ProductosTest::ProductosTest()
{
    this->tablas << "categorias_productos" << "productos" << "compras" << "compras_productos";
}

void ProductosTest::init() { EDatabaseTest::iniciarTablas(); }

void ProductosTest::initTestCase() { EDatabaseTest::generarTablas(); }

void ProductosTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void ProductosTest::cleanup() { EDatabaseTest::vaciarTablas(); }

/*!
 * \brief ProductosTest::testCodigoRepetido
 */
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

/*!
 * \brief ProductosTest::testCodigoRepetido_data
 */
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
    QTest::newRow("original") << "t1" << "test1" << 1.0 << 1.1 << 1 << 0 << "descripcion1" << "marca1" << "modelo1" << true;
    QTest::newRow("repetido") << "t1" << "test1" << 1.0 << 1.1 << 1 << 0 << "descripcion1" << "marca1" << "modelo1" << false;
}

/*!
 * \brief ProductosTest::testIdsSegunCategoria
 */
void ProductosTest::testIdsSegunCategoria()
{
    QFETCH( int, id_categoria );
    QFETCH( int, id_producto );
    QVector<int> list = MProductos::idsSegunCategoria( id_categoria );
    QVERIFY2( list.contains( id_producto ), "Producto no encontrado" );
}

/*!
 * \brief ProductosTest::testIdsSegunCategoria_data
 */
void ProductosTest::testIdsSegunCategoria_data()
{
    QTest::addColumn<int>("id_categoria");
    QTest::addColumn<int>("id_producto");
    QTest::newRow("Arte") << 1 << 2;
    QTest::newRow("Arte") << 1 << 3;
    QTest::newRow("Inicial") << 0 << 1;
}

/*!
 * \brief ProductosTest::testIdsSegunCategoriaInvalida
 */
void ProductosTest::testIdsSegunCategoriaInvalida()
{
    QVector<int> list = MProductos::idsSegunCategoria( -1 );
    QVERIFY2( list.empty(), "Productos con id nulo no están permitidos" );
}

/*!
 * \brief ProductosTest::testListadoCategorias
 */
void ProductosTest::testListadoCategorias()
{
    QFETCH( QString, texto );
    QStringList lista = MCategorias::getListado();
    QVERIFY2( lista.contains( texto ), "Categoria no encontrada" );
}

/*!
 * \brief ProductosTest::testListadoCategorias_data
 */
void ProductosTest::testListadoCategorias_data()
{
    QTest::addColumn<QString>("texto");
    QTest::newRow("Inicial") << "0 ~ Sin Categoria";
    QTest::newRow("Arte") << "1 ~ Arte";
}

/*!
 * \brief ProductosTest::testIdsSegunProveedor
 */
void ProductosTest::testIdsSegunProveedor()
{
    QFETCH( int, id_proveedor );
    QFETCH( int, id_producto );
    QVector<int> list = MProductos::idsSegunProveedor( id_proveedor );
    QVERIFY2( list.contains( id_producto ), "Producto no encontrado" );
}

/*!
 * \brief ProductosTest::testIdsSegunProveedor_data
 */
void ProductosTest::testIdsSegunProveedor_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<int>("id_producto");
    QTest::newRow("Proveedor 1") << 1 << 1;
    QTest::newRow("Proveedor 2") << 2 << 2;
}

/*!
 * \brief ProductosTest::testIdsSegunProveedorInvalida
 */
void ProductosTest::testIdsSegunProveedorInvalida()
{
    QVector<int> list = MProductos::idsSegunProveedor( -1 );
    QVERIFY2( list.empty(), "Productos con id nulo no están permitidos" );
}

#include "preferencias.h"
#include "formagregarproducto.h"
/*!
 * \brief ProductosTest::testAutocompletadoMarcaProveedor
 */
void ProductosTest::testAutocompletadoMarcaProveedor()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "marca_proveedor", true );
    p->endGroup();
    p->endGroup();
    p=0;

    FormAgregarProducto *fap = new FormAgregarProducto();
    QFETCH( QString, nombre_producto );
    QFETCH( QString, proveedor );
    QFETCH( double, precio );
    QFETCH( double, stock );
    fap->setearDesdeCompra( true );
    fap->setearNombre( nombre_producto );
    fap->setearStockInicial( stock );
    fap->setearPrecioCosto( precio );
    fap->setearProveedor( proveedor );

    QVERIFY( fap->LEMarca->text() == proveedor );

}

/*!
 * \brief ProductosTest::testAutocompletadoMarcaProveedor_data
 */
void ProductosTest::testAutocompletadoMarcaProveedor_data()
{
    QTest::addColumn<QString>("nombre_producto");
    QTest::addColumn<QString>("proveedor");
    QTest::addColumn<double>("precio");
    QTest::addColumn<double>("stock");
    QTest::newRow("Proveedor1") << "mc1" << "Proveedor 1" << 10.4 << 2.0;
}

QTEST_MAIN(ProductosTest)

#include "tst_productostest.moc"
