#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "comprobanteestatico.h"

class ComprobantesTest : public QObject
{
    Q_OBJECT
    
public:
    ComprobantesTest();
    
private Q_SLOTS:
    void testCargaInicial();
};

ComprobantesTest::ComprobantesTest()
{}

void ComprobantesTest::testCargaInicial()
{
    ComprobanteEstatico *c = new ComprobanteEstatico();
    c->cargarArchivo( "Comprobantes/test.svg" );
}

QTEST_APPLESS_MAIN(ComprobantesTest)

#include "tst_comprobantestest.moc"
