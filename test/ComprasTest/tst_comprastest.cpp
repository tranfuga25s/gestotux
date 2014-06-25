#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

class ComprasTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    ComprasTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testComprasEtiquetaFecha();
    void testAutoAgregarProductosPreferencias();
    void testAutoAgregarProductos();
    void testAutoAgregarProductos_data();
    void testCantidadDecimalesAgregarCompra();
    void testCantidadMaxima();
};

ComprasTest::ComprasTest()
{
    this->tablas << "compras" << "compras_productos" << "productos";
}

void ComprasTest::initTestCase() { EDatabaseTest::initTestCase(); }
void ComprasTest::init() { EDatabaseTest::init(); }
void ComprasTest::cleanup() { EDatabaseTest::cleanup(); }
void ComprasTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

#include "mcompra.h"
/*!
 * \brief ComprasTest::testComprasEtiquetaFecha
 */
void ComprasTest::testComprasEtiquetaFecha()
{
    MCompra *mc = new MCompra();
    mc->select();
    QDate fecha = mc->data( mc->index( 0, mc->fieldIndex( "fecha" ) ), Qt::EditRole ).toDate();
    QVERIFY2( mc->data( mc->index( 0, mc->fieldIndex( "fecha" ) ), Qt::DisplayRole ).toString()
              ==
              fecha.toString( Qt::SystemLocaleShortDate )
             , "Formato de fecha incorrecto" );
    delete mc;
}

#include "preferencias.h"
#include "formprefcompras.h"
/*!
 * \brief ComprasTest::testAutoAgregarProductosPreferencias
 * Test que verifica el correcto funcionamiento de auto agregar productos al ponerlos en una compra.
 * ver issue #80.
 */
void ComprasTest::testAutoAgregarProductosPreferencias()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Compras" );
    p->setValue( "auto-agregar-productos", true );
    p->endGroup();
    p->endGroup();

    FormPrefCompras *fpc = new FormPrefCompras();
    fpc->cargar();
    QVERIFY2( fpc->CkBAutoAgregarProductos->isChecked() == true, "No se seteo correctamente el autoagregar en comrpas" );

    fpc->CkBAutoAgregarProductos->setChecked( false );
    fpc->guardar();
    p->sync();

    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Compras" );
    QVERIFY2( p->value( "auto-agregar-productos", true ).toBool() == false, "No se guardó la preferencia bien" );
    p->endGroup();
    p->endGroup();
    p=0;
}

#include "formagregarcompra.h"
#include "mproductostotales.h"
#include <QSqlRecord>
#include "einputdialog.h"
/*!
 * \brief ComprasTest::testAutoAgregarProductos
 */
void ComprasTest::testAutoAgregarProductos()
{
    QFETCH(QString,nombre);
    QFETCH(double,precio);
    QFETCH(int,id_proveedor);
    QFETCH(QString,nombre_proveedor);

    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Compras" );
    p->setValue( "auto-agregar-productos", true );
    p->endGroup();
    p->beginGroup( "Productos");
    p->setValue( "marcas", true );
    p->setValue( "marca_proveedor", true );
    p->setValue( "ocultar_codigo", true );
    double ganancia = p->value( "ganancia", 10.0 ).toDouble();
    p->endGroup();
    p->endGroup();



    FormAgregarCompra *fac = new FormAgregarCompra();

    fac->CBProveedor->setearId( id_proveedor );
    QTest::qWait( 1000 );

    // Agrego un nuevo item
    QTest::keyClicks( fac->CBProducto, nombre );
    QCOMPARE( fac->CBProducto->currentText(), nombre );
    fac->DSBPrecioUnitario->setValue( precio );
    QTest::mouseClick( fac->PBAgregarProducto, Qt::LeftButton );

    QVERIFY2( fac->TVLista->model()->rowCount() == 2, "Error de conteo" );
    QCOMPARE( fac->TVLista->model()->data( fac->TVLista->model()->index( 0, 0 ) ).toDouble(), 1.0  );
    QCOMPARE( fac->TVLista->model()->data( fac->TVLista->model()->index( 0, 1 ) ).toString(), nombre );
    QCOMPARE( fac->TVLista->model()->data( fac->TVLista->model()->index( 0, 2 ), Qt::EditRole ).toDouble(), precio );

    fac->guardar();

    QSqlQuery cola;
    // Verifico que se haya agregado el producto con el proveedor correspondiente
    QVERIFY2( cola.exec( QString( "SELECT marca, precio_costo, precio_venta, id FROM producto WHERE nombre = '%1'" ).arg( nombre ) ) == true, cola.lastError().text().toLocal8Bit() );
    QVERIFY2( cola.next() == true, cola.lastError().text().toLocal8Bit() );
    QVERIFY2( cola.record().value(0).toString() == nombre_proveedor, "No coincide el nombre del proveedor en la marca del producto agregado" );
    QCOMPARE( cola.record().value(1).toDouble(), precio );
    QCOMPARE( cola.record().value(2).toDouble(), precio*( 1 + ( ganancia /100 ) ) );

    int id_producto = cola.record().value(3).toInt();
    QVERIFY( id_producto > 0 );

    QVERIFY2( cola.exec( QString( "SELECT COUNT( id_producto ) FROM compras_productos WHERE id_producto = %1" ).arg( id_producto ) ) == true, cola.lastError().text().toLocal8Bit() );
    QVERIFY2( cola.next() == true, cola.lastError().text().toLocal8Bit() );
    QVERIFY2( cola.record().value(0).toString() > 0, "No coincide el numero de id_producto en la orden de compra" );


}

/*!
 * \brief ComprasTest::testAutoAgregarProductos_data
 */
void ComprasTest::testAutoAgregarProductos_data()
{
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<double>("precio");
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<QString>("nombre_proveedor");
    QTest::newRow("Primer elemento") << "Prueba insercion 1" << 10.0 << 1 << "Proveedor 1";
    QTest::newRow("SegundoItem") << "Prueba insersión 2 " << 11.0 << 2 << "Proveedor 2";
}

/**
 * @brief ComprasTest::testCantidadDecimales
 * Verifica que se cumpla la cantidad de decimales en el formulario de agregar compra
 */
void ComprasTest::testCantidadDecimalesAgregarCompra()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    p->setValue( "cantidad-decimales", 4 );
    p->setValue( "mostrar-decimales", true );
    p->sync();

    FormAgregarCompra *ac = new FormAgregarCompra();
    QCOMPARE( ac->DSBCant->decimals(), 4 );
    delete ac;
    ac=0;

    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->beginGroup( "Stock" );
    p->setValue( "cantidad-decimales", 4 );
    p->setValue( "mostrar-decimales", false );
    p->sync();

    ac = new FormAgregarCompra();
    QCOMPARE( ac->DSBCant->decimals(), 0 );
    delete ac;
    ac=0;
}

/*!
 * Verifica que se tenga la cantidad suficiente de valores para ingresar
 */
void ComprasTest::testCantidadMaxima()
{
    FormAgregarCompra *ac = new FormAgregarCompra();
    ac->DSBCant->setValue( 9999999.0 );
    QCOMPARE( ac->DSBCant->value(), 9999999.0 );
    delete ac;
    ac=0;
}

QTEST_MAIN(ComprasTest)

#include "tst_comprastest.moc"
