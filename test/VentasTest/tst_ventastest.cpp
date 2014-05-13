#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

class VentasTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    VentasTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void testPreferenciaConsumidorFinalPredeterminado();
    void testAnulacionFacturaDescensoStock();
};

VentasTest::VentasTest()
{
    this->tablas << "clientes"
                 << "factura"
                 << "item_factura"
                 << "productos";
}

void VentasTest::init() { EDatabaseTest::init(); }

void VentasTest::initTestCase() { EDatabaseTest::initTestCase(); }

void VentasTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void VentasTest::cleanup() { EDatabaseTest::cleanup(); }

#include "preferencias.h"
#include "formagregarventa.h"
#include "formprefventa.h"
#include <unistd.h>
void VentasTest::testPreferenciaConsumidorFinalPredeterminado()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Ventas" );
    p->setValue( "siempre_cf", true );
    p->endGroup();
    p->endGroup();

    FormAgregarVenta *fav = new FormAgregarVenta();
    usleep( 9000 );
    QVERIFY( fav->CBCliente->idClienteActual() == 0 );
    //QVERIFY( fav->CBCliente->currentText() == "Consumidor Final" );
    delete fav;
    fav = 0;

    FormPrefVenta *fpv = new FormPrefVenta();
    fpv->cargar();
    QVERIFY( fpv->CkBConsumidorFinal->isChecked() );
    delete fpv;
    fpv=0;

    p->beginGroup( "Preferencias" );
    p->beginGroup( "Ventas" );
    p->setValue( "siempre_cf", false );
    p->endGroup();
    p->endGroup();

    fav = new FormAgregarVenta();
    usleep( 9000 );
    QVERIFY( fav->CBCliente->idClienteActual() == 0 );
    //QVERIFY( fav->CBCliente->currentText() == "" );
    delete fav;

    fpv = new FormPrefVenta();
    fpv->cargar();
    QVERIFY( fpv->CkBConsumidorFinal->isChecked() == false );
    delete fpv;
    fpv=0;
}

#include "MFactura.h"
#include "mitemfactura.h"
#include "mproductos.h"
#include <QSqlRecord>
#include <QSqlError>
/*!
 * Test para verificar la disminución del stock al anular una factura
 */
void VentasTest::testAnulacionFacturaDescensoStock()
{
    // Busco una factura
    QSqlQuery cola;
    if( !cola.exec( "SELECT id_factura FROM factura LIMIT 1" ) ) {
        qDebug() << "Error al buscar el ID de factura";
        qDebug() << cola.lastError().text();
    }
    QVERIFY2( cola.next(), "NO hay datos para realizar el test" );
    int id_factura = cola.record().value(0).toInt();

    // Busco todos los productos comprados en esta factura
    if( !cola.exec( QString( "SELECT id_producto, cantidad FROM item_factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
        qDebug() << "Error al buscar los items de factura";
        qDebug() << cola.lastError().text();
    }
    QVERIFY2( cola.next(), "No hay items en la factura que se refieran a productos" );
    QVector<int> ids_productos;
    ids_productos.clear();
    QVector<double> cantidad_productos;
    cantidad_productos.clear();
    do {
        int id_producto = cola.record().value(0).toInt();
        QVERIFY2( id_producto != 0, "Id de producto incorrecto" );
        double cantidad = cola.record().value(1).toDouble();
        QVERIFY2( cantidad != 0.0, "Cantidad incorrecta" );
        ids_productos.append( id_producto );
        cantidad_productos.append( cantidad );
    } while( cola.next() );
    QVERIFY2( ids_productos.count() > 0, "No hay items cargados en la lista de productos" );
    QVERIFY2( cantidad_productos.count() > 0, "No hay items cargados en la lista de cantidades vendidas" );

    // Habilito el stock actual
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    p->setValue( "stock",  true );
    p->endGroup();
    p->endGroup();
    p=0;
    // Busco el stock actual
    QVector<double> stock_actual; stock_actual.clear();
    foreach( int id_producto, ids_productos ) {
        double stock = MProductos::stock( id_producto );
        QVERIFY2( stock != 0.0, "Error de stock" );
        stock_actual.append( stock );
    }
    QVERIFY2( stock_actual.count() > 0, "No se cargó ningun item de stock" );

    // Anulo la factura
    QVERIFY2( MFactura::anularFactura( id_factura, "Prueba" ) != false, "No se pudo anular la factura!" );

    // Controlo nuevamente el stock
    int i = 0;
    foreach( int id_producto, ids_productos ) {
        QVERIFY2( ( stock_actual.at(i) + cantidad_productos.at(i) ) == MProductos::stock( id_producto ),
                  QString( "No coincide el stock del producto %1: anterior: %2, nuevo: %3, diferencia esperada: %4" )
                  .arg( id_producto ).arg( stock_actual.at(i) ).arg( MProductos::stock( id_producto ) ).arg( cantidad_productos.at(i) )
                  .toLocal8Bit() );
        i++;
    }
}

QTEST_MAIN(VentasTest)

#include "tst_ventastest.moc"
