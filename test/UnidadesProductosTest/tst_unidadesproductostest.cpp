#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "munidadesproductos.h"
#include "mcategorias.h"
#include "mproductos.h"
#include "../edatabasetest.h"

class UnidadesProductosTest : public QObject, public EDatabaseTest
{
    Q_OBJECT
    
public:
    UnidadesProductosTest();
    
private Q_SLOTS:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void tstHerencia();
    void tstHerencia_data();

private:
    MProductos *mp;
    MUnidadesProductos *mup;
    bool doClean;

};

/*!
 * \brief UnidadesProductosTest::UnidadesProductosTest
 */
UnidadesProductosTest::UnidadesProductosTest()
{
    this->tablas << "categorias_productos"
                 << "productos"
                 << "compras"
                 << "compras_productos"
                 << "unidades_productos";
}

void UnidadesProductosTest::init() { EDatabaseTest::init(); doClean = true; }

void UnidadesProductosTest::initTestCase() { EDatabaseTest::initTestCase(); }

void UnidadesProductosTest::cleanupTestCase() { EDatabaseTest::cleanupTestCase(); }

void UnidadesProductosTest::cleanup() { if( this->doClean ) { EDatabaseTest::cleanup(); } }

/**
 * @brief UnidadesProductosTest::tstHerencia
 * Verifica que se cumpla la herencia para las unidades correspondientes
 */
void UnidadesProductosTest::tstHerencia()
{
    QFETCH( int, id_unidad );
    QFETCH( bool, tiene_padre );
    mup = new MUnidadesProductos();
    QVERIFY2( mup->tienePadre( id_unidad ) == tiene_padre, "Este elemento deberia de tener padre" );
    delete mup;
    mup = 0;
}

void UnidadesProductosTest::tstHerencia_data()
{
    QTest::addColumn<int>("id_unidad");
    QTest::addColumn<bool>("tiene_padre");
    QTest::newRow("SinPadre") << 1 << false;
    QTest::newRow("ConPadre") << 2 << true;
}


QTEST_MAIN(UnidadesProductosTest)

#include "tst_unidadesproductostest.moc"
