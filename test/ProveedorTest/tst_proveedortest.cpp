#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../edatabasetest.h"

#include "mproveedor.h"

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
};

ProveedorTest::ProveedorTest()
{
    this->tablas << "proveedor";
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

QTEST_MAIN(ProveedorTest)

#include "tst_proveedortest.moc"
