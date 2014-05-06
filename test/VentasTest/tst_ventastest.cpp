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

void VentasTest::init() { EDatabaseTest::iniciarTablas(); }

void VentasTest::initTestCase() { EDatabaseTest::generarTablas(); }

void VentasTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

void VentasTest::cleanup() { EDatabaseTest::vaciarTablas(); }

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
        ids_productos.append( cola.record().value(0).toInt() );
        cantidad_productos.append( cola.record().value(1).toDouble() );
    } while( cola.next() );
    QVERIFY2( ids_productos.count() > 0, "No hay items cargados en la lista de productos" );
    QVERIFY2( cantidad_productos.count() > 0, "No hay items cargados en la lista de cantidades vendidas" );

    // Busco el stock actual
    QVector<double> stock_actual; stock_actual.clear();
    foreach( int id_producto, ids_productos ) {
        stock_actual.append( MProductos::stock( id_producto ) );
    }
    QVERIFY2( stock_actual.count() > 0, "No se cargó ningun item de stock" );

    // Anulo la factura
    QVERIFY2( MFactura::anularFactura( id_factura, "Prueba" ) != false, "No se pudo anular la factura!" );

    // Controlo nuevamente el stock
    int i = 0;
    foreach( int id_producto, ids_productos ) {
        QVERIFY2( stock_actual.at(i) - cantidad_productos.at(i) == MProductos::stock( id_producto ),
                  QString( "No coincide el stock del producto %1: anterior: %2, nuevo: %3, diferencia esperada: -%4" )
                  .arg( id_producto ).arg( stock_actual.at(i) ).arg( MProductos::stock( id_producto ) ).arg( cantidad_productos.at(i) )
                  .toLocal8Bit() );
        i++;
    }
}

QTEST_MAIN(VentasTest)

#include "tst_ventastest.moc"
