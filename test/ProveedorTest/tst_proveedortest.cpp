#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

#include "mproveedor.h"

#include <QSqlQuery>
#include <QSqlRecord>

class ProveedorTest : public QObject, private EDatabaseTest
{
    Q_OBJECT
    
public:
    ProveedorTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void testListado();
    void testListado_data();
    void testVentanaModificar();
    void testVentanaModificar_data();
    void testVerificarDatosRelacionados();
    void testRazonSocialRepetida();
    void testRazonSocialRepetida_data();
    void testCuitCuilRepetido();
    void testCuitCuilRepetido_data();
    void testExistenProveedores();
};

ProveedorTest::ProveedorTest()
{
    this->tablas << "proveedor"
                 << "compras"
                 << "compras_productos";
}
void ProveedorTest::init() { EDatabaseTest::init(); }

void ProveedorTest::initTestCase() { EDatabaseTest::initTestCase(); }

void ProveedorTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void ProveedorTest::cleanup() { EDatabaseTest::cleanup(); }

void ProveedorTest::testListado()
{
    QFETCH( QString, texto );
    QStringList lista = MProveedor::getListado();
    QVERIFY2( lista.contains( texto ), "Proveedor no encontrada" );
}

void ProveedorTest::testListado_data()
{
    QTest::addColumn<QString>("texto");
    QTest::newRow("Proveedor 1") << "1 ~ Proveedor 1";
    QTest::newRow("Proveedor 2") << "2 ~ Proveedor 2";
}

#include "formmodificarproveedor.h"
/*!
 * \brief ProveedorTest::testVentanaModificar
 * Testea que la ventana de modificar funcione correctamente
 */
void ProveedorTest::testVentanaModificar()
{
    QFETCH(int,id_proveedor);
    QFETCH(QString,nombre);
    QFETCH(QString,direccion);
    QFETCH(QString,fax);
    QFETCH(QString,telefono);
    QFETCH(QString,email);
    QFETCH(QString,celular);
    QFETCH(QString,cuit_cuil);

    MProveedor *mp = new MProveedor();
    mp->select();

    FormModificarProveedor *fmp = new FormModificarProveedor( mp, 0, true );
    fmp->setearItem( 0 );
    fmp->LERazonSocial->setText( nombre );
    fmp->TEDireccion->setPlainText( direccion );
    fmp->LEFax->setText( fax );
    fmp->LETelFijo->setText( telefono );
    fmp->LETelCel->setText( celular );
    fmp->LEEmail->setText( email );
    fmp->LECUIT->setText( cuit_cuil );

    QVERIFY( fmp->guardar() );
    delete fmp;
    delete mp;

    QSqlQuery cola;
    QVERIFY2( cola.exec( QString( "SELECT nombre, direccion, fax, telefono_linea, email, telefono_celular, cuit_cuil FROM proveedor WHERE id = %1" ).arg( id_proveedor ) ), cola.lastError().text().toLocal8Bit() );
    QVERIFY( cola.next() );
    QCOMPARE( cola.record().value(0).toString(), nombre );
    QCOMPARE( cola.record().value(1).toString(), direccion );
    QCOMPARE( cola.record().value(2).toString(), fax );
    QCOMPARE( cola.record().value(3).toString(), telefono );
    QCOMPARE( cola.record().value(4).toString(), email );
    QCOMPARE( cola.record().value(5).toString(), celular );
    QCOMPARE( cola.record().value(6).toString().replace( '-', "" ), cuit_cuil );
}

void ProveedorTest::testVentanaModificar_data()
{
    QTest::addColumn<int>("id_proveedor");
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<QString>("direccion");
    QTest::addColumn<QString>("fax");
    QTest::addColumn<QString>("telefono");
    QTest::addColumn<QString>("email");
    QTest::addColumn<QString>("celular");
    QTest::addColumn<QString>("cuit_cuil");
    QTest::newRow("Todos") << 1
                           << "Nombre1"
                           << "Dirección1"
                           << "2039103829"
                           << "394820498"
                           << "email@gmail.com"
                           << "20391038923"
                           << "20309983042";
}

void ProveedorTest::testVerificarDatosRelacionados() {
    // el proveedor 1 tiene que tener datos
    QVERIFY( MProveedor::tieneDatosRelacionados( 1 ) );

    // Agrego un proveedor sin datos relacionados ( en el schema )
    QVERIFY( MProveedor::tieneDatosRelacionados( 3 ) == false );
}

/*!
 * Verifica que no se permita ingresar 2 proveedores con la misma razon social - issue #51
 */
void ProveedorTest::testRazonSocialRepetida()
{
    QFETCH( QString, nombre );
    QFETCH( bool, error );
    QCOMPARE( MProveedor::existeProveedor( nombre ), error );
}

void ProveedorTest::testRazonSocialRepetida_data()
{
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<bool>("error");
    QTest::newRow("Proveedor1") << "Proveedor 1" << true;
    QTest::newRow("Proveedor2") << "Proveedor 2" << true;
    QTest::newRow("ProvedorNuevo") << "Proveedor 3" << false;
}

/*!
 *
 */
void ProveedorTest::testCuitCuilRepetido()
{
    QFETCH( QString, cuit );
    QFETCH( bool, error );
    QCOMPARE( MProveedor::existeCuitCuil( cuit ), error );
}

void ProveedorTest::testCuitCuilRepetido_data()
{
    QTest::addColumn<QString>("cuit");
    QTest::addColumn<bool>("error");
    QTest::newRow("Proveedor1") << "20309983042" << true;
    QTest::newRow("Proveedor2") << "56497824556" << false;
}

void ProveedorTest::testExistenProveedores()
{
    QCOMPARE( MProveedor::existenProveedores(), true );

    QSqlQuery cola;
    QVERIFY2( cola.exec( "DELETE FROM proveedor;" ), cola.lastError().text().toLocal8Bit() );

    QCOMPARE( MProveedor::existenProveedores(), false );
}

QTEST_MAIN(ProveedorTest)

#include "tst_proveedortest.moc"
