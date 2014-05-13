#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "mcategorias.h"
#include "mproductos.h"
#include "../edatabasetest.h"

class ProductosTest : public QObject, public EDatabaseTest
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
    void testMarcaProveedor();
    void testCategoriaRepetida();
    void testCategoriaRepetida_data();
    void testOcultarCodigo();
    void testCategoriaEnAltaProducto();
    void testModeloEnAltaProducto();
    void testMarcaEnAltaProducto();
    void testDescripcionEnAltaProducto();
    void testOcultarCosto();
    void testPreferenciasStockLista();

private:
    MProductos *mp;
    bool doClean;

};

/*!
 * \brief ProductosTest::ProductosTest
 */
ProductosTest::ProductosTest()
{
    this->tablas << "categorias_productos"
                 << "productos"
                 << "compras"
                 << "compras_productos";
}

void ProductosTest::init() { EDatabaseTest::init(); doClean = true; }

void ProductosTest::initTestCase() { EDatabaseTest::initTestCase(); }

void ProductosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void ProductosTest::cleanup() { if( this->doClean ) { EDatabaseTest::cleanup(); } }

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
        doClean = false;
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
#include "formmodificarproducto.h"
#include "vproductos.h"
#include <QHeaderView>
#include <QTableView>
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
    delete fap;
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
    QTest::newRow("Proveedor1") << "mc2" << "Proveedor 2" << 10.4 << 2.0;
}

/*!
 * \brief ProductosTest::testMarcaProveedor
 */
void ProductosTest::testMarcaProveedor()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "marca_proveedor", true );
    p->endGroup();
    p->endGroup();
    p=0;

    VProductos *vp = new VProductos();
    QCOMPARE( vp->vista->model()->headerData( qobject_cast<QSqlTableModel*>(vp->vista->model())->fieldIndex( "marca" ), Qt::Horizontal, Qt::DisplayRole ).toString(),
              QString("Proveedor") );
    delete vp;

    FormAgregarProducto *fap = new FormAgregarProducto();
    QVERIFY2( fap->LEMarca->isVisible() == false, "El campo marca debería de ser proveedor o no estar visible" );
    QVERIFY2( fap->LMarca->isVisible() == false, "El campo marca debería de ser proveedor o no estar visible" );
    delete fap;

    FormModificarProducto *fmp = new FormModificarProducto( new MProductos() );
    QVERIFY2( fmp->LEMarca->isVisible() == false, "El campo marca debería de ser proveedor o no estar visible" );
    QVERIFY2( fmp->LMarca->isVisible() == false, "El campo marca debería de ser proveedor o no estar visible" );
    delete fmp;
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
    QVERIFY2( mp->agregarProducto( QString(), "Test nulo", 1.0, 1.1, 1.0, 1 ) > 0, "Se debería de permitir agregar un producto con codigo nulo si está habilitado" );
    QVERIFY2( mp->agregarProducto( QString(), "Test nulo2", 1.0, 1.1, 1.0, 1 ) > 0, "Se debería de permitir agregar un producto con codigo nulo si está habilitado" );
    QVERIFY2( mp->agregarProducto( QString(), "Test nulo3", 1.0, 1.1, 1.0, 1 ) > 0, "Se debería de permitir agregar un producto con codigo nulo si está habilitado" );

    delete mp;
}

/*!
 * \brief ProductosTest::testCategoriaEnAltaProducto
 * Test para verificar ticket issue #73
 */
void ProductosTest::testCategoriaEnAltaProducto()
{
    // Habilito el uso de las categorías en el sistema
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "categorias", true );
    p->inicio();
    p=0;

    int id_categoria = 1;

    MProductos *mp = new MProductos();

    int id_producto = mp->agregarProducto( "Codigo1", "Test", 10.0, 12.0, 1, id_categoria );

    QVector<int> lista = mp->idsSegunCategoria( id_categoria );
    QVERIFY2( lista.size() != 0, "La lista de productos de la cateogría esta vacia" );
    QVERIFY2( lista.contains( id_producto ), "El Producto no se dio de alta en la categoria asignada" );

}

#include <QSqlRecord>
#include <QVector>
/*!
 * \brief ProductosTest::testMarcaEnAltaProducto
 * Test para verificar que se de correctamente de alta el modelo al agregar un producto
 * Ver issue #77.
 */
void ProductosTest::testMarcaEnAltaProducto()
{
    // Habilito el uso de las categorías en el sistema
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "marcas", true );
    p->inicio();
    p=0;

    MProductos *mp = new MProductos();

    int id_producto = mp->agregarProducto( QString(), "testMarcaEnAltaProducto", 10.0, 12.0, 1, 1, QString(), "Marcass1" );

    QVERIFY2( id_producto > 0, "No se pudo insertar el producto" );

    QSqlQuery cola;
    QVERIFY( cola.exec( "SELECT COUNT( marca ) FROM producto WHERE marca = 'Marcass1'" ) );
    QVERIFY( cola.next() );
    QVERIFY( cola.record().value(0).toInt() == 1 );
}

/*!
 * \brief ProductosTest::testModeloEnAltaProducto
 * Test para verificar que se de correctamente de alta el modelo al agregar un producto
 * Ver issue #77.
 */
void ProductosTest::testModeloEnAltaProducto()
{
    // Habilito el uso de las categorías en el sistema
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "modelo", true );
    p->inicio();
    p=0;

    MProductos *mp = new MProductos();

    int id_producto = mp->agregarProducto( QString(), "testModeloEnAltaProducto", 10.0, 12.0, 1, 1, QString(), "Marca1", "Modelos1" );

    QVERIFY2( id_producto > 0, "No se pudo insertar el producto" );

    QSqlQuery cola;
    QVERIFY( cola.exec( "SELECT COUNT( modelo ) FROM producto WHERE modelo = 'Modelos1'" ) );
    QVERIFY( cola.next() );
    QVERIFY( cola.record().value(0).toInt() == 1 );
}

/*!
 * \brief ProductosTest::testDescripcionEnAltaProducto
 */
void ProductosTest::testDescripcionEnAltaProducto()
{
    // Habilito el uso de las categorías en el sistema
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "marcas", true );
    p->inicio();
    p=0;

    MProductos *mp = new MProductos();

    int id_producto = mp->agregarProducto( QString(), "testDescripcionEnAltaProducto", 10.0, 12.0, 1, 1, "testDescripcionEnAltaProducto" );

    QVERIFY2( id_producto > 0, "No se pudo insertar el producto" );

    QSqlQuery cola;
    QVERIFY( cola.exec( "SELECT COUNT( descripcion ) FROM producto WHERE descripcion = 'testDescripcionEnAltaProducto'" ) );
    QVERIFY( cola.next() );
    QVERIFY( cola.record().value(0).toInt() == 1 );
}

/*!
 * \brief ProductosTest::testOcultarCosto
 */
void ProductosTest::testOcultarCosto()
{
    // Habilito el uso de las categorías en el sistema
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "mostrar-costo", false );
    p->inicio();
    p=0;

    VProductos *vp = new VProductos();
    vp->show();
    QVERIFY2( vp->actions().contains( vp->ActVerCosto ) == false, "No se deberia de mostrar el boton del costo del producto" );
    delete vp;

    FormAgregarProducto *fap = new FormAgregarProducto();
    QVERIFY2( fap->DSBCosto->isVisible() == false, "No se debería de ver el precio de costo en agregar producto" );
    delete fap;

    FormModificarProducto *fmp = new FormModificarProducto( new MProductos() );
    QVERIFY2( fmp->DSBCosto->isVisible() == false, "No se debería de ver el precio de costo en modificar producto" );
    delete fmp;
}

/*!
 * \brief ProductosTest::testPreferenciasStockLista
 */
void ProductosTest::testPreferenciasStockLista()
{
    // Habilito el uso de las categorías en el sistema
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "mostrar-stock-lista", false );
    p->inicio();
    p=0;

    FormPrefProductos *fpp = new FormPrefProductos();
    fpp->cargar();
    QVERIFY( fpp->CkBMostrarStockLista->isChecked() );

    fpp->CkBMostrarStockLista->setChecked( false );
    fpp->guardar();

    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    QVERIFY( p->value( "mostrar-stock-lista", true ) == false );
    p->inicio();
    p=0;
}

QTEST_MAIN(ProductosTest)

#include "tst_productostest.moc"
