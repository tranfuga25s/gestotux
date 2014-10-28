#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "comprobanteestatico.h"

class ComprobanteTest : public QObject
{
    Q_OBJECT
    
public:
    ComprobanteTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCargaInicial();
};

ComprobanteTest::ComprobanteTest()
{
}

void ComprobanteTest::initTestCase()
{
}

void ComprobanteTest::cleanupTestCase()
{
}

void ComprobanteTest::testCaseCargaInicial()
{
    ComprobanteEstatico *c = new ComprobanteEstatico();
    c->cargarArchivo( "../../../gestotux/comprobantes/test.svg" );
    QVERIFY( c->version() == "1" );
    QCOMPARE( c->getCantidadCopias(), 2 );
}

QTEST_MAIN(ComprobanteTest)

#include "tst_comprobantetest.moc"
