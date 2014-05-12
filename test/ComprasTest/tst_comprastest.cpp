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
};

ComprasTest::ComprasTest()
{
    this->tablas << "compras" << "compras_productos" << "productos";
}

void ComprasTest::initTestCase() { EDatabaseTest::generarTablas(); }
void ComprasTest::init() { EDatabaseTest::iniciarTablas(); }
void ComprasTest::cleanup() { EDatabaseTest::vaciarTablas(); }
void ComprasTest::cleanupTestCase() { EDatabaseTest::borrarTablas(); }

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
#include "einputdialog.h"
/*!
 * \brief ComprasTest::testAutoAgregarProductos
 */
void ComprasTest::testAutoAgregarProductos()
{
    QFETCH(QString,nombre);
    QFETCH(double,precio);
    QFETCH(int,id_proveedor);

    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Compras" );
    p->setValue( "auto-agregar-productos", true );
    p->endGroup();
    p->endGroup();

    FormAgregarCompra *fac = new FormAgregarCompra();
    fac->CBProveedor->setearId( id_proveedor );

    QTest::keyClicks( fac->CBProducto, nombre );
    /*fac->PBAgregarProducto->click();

    EInputDialogDoubleSpinBox *dialogo_precio = fac->findChildren<EInputDialog *>()->findChildren<EInputDialogDoubleSpinBox *>();
    dialogo_precio->setValue( precio );
    qobject_cast<EInputDialog *>( dialogo_precio->parent() )->accept();*/



}

/*!
 * \brief ComprasTest::testAutoAgregarProductos_data
 */
void ComprasTest::testAutoAgregarProductos_data()
{
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<double>("precio");
    QTest::addColumn<int>("id_proveedor");
    QTest::newRow("Primer elemento") << "Prueba insercion 1" << 10.40 << 1;
}

QTEST_MAIN(ComprasTest)

#include "tst_comprastest.moc"
