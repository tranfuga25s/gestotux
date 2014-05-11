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
    void testAutoAgregarProductos();
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
 * \brief ComprasTest::testAutoAgregarProductos
 * Test que verifica el correcto funcionamiento de auto agregar productos al ponerlos en una compra.
 * ver issue #80.
 */
void ComprasTest::testAutoAgregarProductos()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Compras" );
    p->setValue( "auto-agregar-productos", true );
    p->endGroup();
    p->endGroup();
    p=0;

    FormPrefCompras *fpc = new FormPrefCompras();
    fpc->cargar();
    QVERIFY2( fpc->CkBAutoAgregarProductos->isChecked() == true, "No se seteo correctamente el autoagregar en comrpas" );
    delete fpc;
}

QTEST_MAIN(ComprasTest)

#include "tst_comprastest.moc"
