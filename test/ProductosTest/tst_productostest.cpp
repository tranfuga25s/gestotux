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
    void testCategoriaRepetida();
    void testCategoriaRepetida_data();
    void testOcultarCodigo();
    void testCategoriaEnAltaProducto();

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
        QCOMPARE( mp->agregarProducto( codigo, nombre, costo, venta, stock, categoria, descripcion, marca, modelo ), -1 );
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

#include "mcategorias.h"
/*!
 * \brief ProductosTest::testCategoriaRepetida
 */
void ProductosTest::testCategoriaRepetida()
{
    QFETCH( QString, nombre );
    QFETCH( bool, devolucion );
    QVERIFY2( devolucion == MCategorias::buscarRepetido( nombre ), QString( "La categoria %1 ya existe!" ).arg( nombre ).toLocal8Bit() );
}

/*!
 * \brief ProductosTest::testCategoriaRepetida_data
 */
void ProductosTest::testCategoriaRepetida_data()
{
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<bool>("devolucion");
    QTest::newRow("Nuevo") << "NuevaCategoria" << false;
    QTest::newRow("Repetida") << "Arte" << true;
}

#include "formprefproductos.h"
#include "vproductos.h"
#include <QTableView>
/*!
 * \brief ProductosTest::testOcultarCodigo
 */
void ProductosTest::testOcultarCodigo()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "ocultar_codigo", true );
    p->endGroup();
    p->endGroup();
    p=0;

    // En las preferencias se debe respetar
    FormPrefProductos *fpp = new FormPrefProductos();
    fpp->cargar();
    QVERIFY2( fpp->CkBOcultarCodigo->isChecked() == true, "Falta respetar las preferencias" );
    delete fpp;

    // En la vista de productos no se tiene qu ever el codigo
    FormAgregarProducto *fap = new FormAgregarProducto();
    fap->show();
    QVERIFY2( !fap->LECodigo->isVisible() , "Campo de codigo visible en vista de productos" );
    QVERIFY2( !fap->LCodigo->isVisible(), "Etiqueta de codigo visible en vista de productos" );
    delete fap;

    MProductos *mp = new MProductos();

    // En la lista de productos no se debe ver la columna
    VProductos *vp = new VProductos();
    QVERIFY2( vp->vista->isColumnHidden( mp->fieldIndex( "codigo" ) ), "La columna de codigo no debería de estar visible" );
    delete vp;

    // Intento guardar un producto con codigo nulo
    QVERIFY2( mp->agregarProducto( QString(), "Test nulo", 1.0, 1.1, 1.0, 1, "", "", "" ) > 0, "Se debería de permitir agregar un producto con codigo nulo si está habilitado" );

    delete mp;
}

/*!
 * \brief ProductosTest::testCategoriaEnAltaProducto
 * Test para verificar ticket issue #73
 */
void ProductosTest::testCategoriaEnAltaProducto()
{
    // Habilito el uso de las categorías en el sistema
    preferencias::getInstancia()->setValue( "Preferencias/Productos/categorias", true );

    MProductos *mp = new MProductos();

    int id_producto = mp->agregarProducto( "Codigo1", "Test", 10.0, 12.0, 1, 2 );
    int id_categoria = 2;

    QVector<int> lista = mp->idsSegunCategoria( id_categoria );

    QVERIFY2( lista.contains( id_producto ), "El Producto no se dio de alta en la categoria asignada" );

}

QTEST_MAIN(ProductosTest)

#include "tst_productostest.moc"
