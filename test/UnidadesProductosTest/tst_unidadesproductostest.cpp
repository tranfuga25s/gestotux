#include <QString>
#include <QtTest>

#include <QSqlQuery>

#include "munidadesproductos.h"
#include "mproductosunidades.h"
#include "mcategorias.h"
#include "mproductos.h"
#include "../edatabasetest.h"
#include "munidadesproductosarbol.h"

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
    void tstConversion();
    void tstConversion_data();
    void tstUnidadesSegunProducto();
    void tstUnidadesSegunProducto_data();
    void tstEliminacion();
    void tstEliminacion_data();
    void tstAgregarElemento();
    void tstAgregarElemento_data();
    void tstArbolUnidadesProductos();

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
                 << "unidades_productos"
                 << "productos_unidades";
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

/**
 * @brief UnidadesProductosTest::tstConversion
 * Verifica la conversión entre unidades
 */
void UnidadesProductosTest::tstConversion()
{
    QFETCH( int, id_unidad );
    QFETCH( double, original );
    QFETCH( double, convertido );
    mup = new MUnidadesProductos();
    QCOMPARE( mup->getValorNominal( id_unidad, original ), convertido );
    delete mup;
    mup=0;
}

void UnidadesProductosTest::tstConversion_data()
{
    QTest::addColumn<int>("id_unidad");
    QTest::addColumn<double>("original");
    QTest::addColumn<double>("convertido");
    QTest::newRow("SinPadre") << 1 << 1.0 << 1.0;
    QTest::newRow("ConPadre") << 2 << 1000.0 << 1.0;
    QTest::newRow("Tonelada") << 3 << 1.0 << 1000.0;
}

/**
 * @brief UnidadesProductosTest::tstUnidadesSegunProducto
 * Verifica que funcionen correctamente las funciones para obtener las unidades de un producto
 */
void UnidadesProductosTest::tstUnidadesSegunProducto()
{
    QFETCH( int, id_producto );
    QFETCH( int, unidad );

    MProductosUnidades *mpu = new MProductosUnidades();
    QVector<int> unidades_traidas = mpu->getUnidadesSegunProducto( id_producto );
    QVERIFY2( unidades_traidas.contains( unidad ), "No se encontró una unidad" );
    delete mpu;
}

void UnidadesProductosTest::tstUnidadesSegunProducto_data()
{
    QTest::addColumn<int>("id_producto");
    QTest::addColumn<int>("unidad");

    QTest::newRow("Caso 1") << 1 << 1;

    QTest::newRow("Sin relaciones") << 2 << 1;
}

/**
 * @brief UnidadesProductosTest::tstEliminacion
 * Verifica que la eliminacion de datos se haga correctamente.
 */
void UnidadesProductosTest::tstEliminacion()
{
    QFETCH( int, id_unidad );
    QFETCH( bool, eliminar_hijos );
    QFETCH( bool, resultado );

    MUnidadesProductos *mup = new MUnidadesProductos();
    QCOMPARE( mup->eliminar( id_unidad, eliminar_hijos ), resultado );
    if( resultado ) {
        // Verifico que se eliminaron las asociacones
        MProductosUnidades *mpu = new MProductosUnidades();
        QCOMPARE( mpu->getProductosSegunUnidad( id_unidad ).count(), 0 );
        delete mpu;
    }
    delete mup;
}

void UnidadesProductosTest::tstEliminacion_data()
{
    QTest::addColumn<int>("id_unidad");
    QTest::addColumn<bool>("eliminar_hijos");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("HijoNormal") << 2 << true << true;
    //QTest::newRow("PadreSinHijos") << ? << false << false;
    QTest::newRow("PadreConHijos") << 1 << true << true;
}

/**
 * @brief UnidadesProductosTest::tstAgregarElemento
 * Verifica que las agregaciones se realizen correctamente
 */
void UnidadesProductosTest::tstAgregarElemento()
{
    QFETCH( int    , id_unidad_padre );
    QFETCH( QString, nombre          );
    QFETCH( double , multiplicador   );
    QFETCH( bool   , resultado       );

    MUnidadesProductos *mup = new MUnidadesProductos();
    QCOMPARE( resultado, mup->agregarUnidad( id_unidad_padre, nombre, multiplicador ) );
    delete mup;
}

void UnidadesProductosTest::tstAgregarElemento_data()
{
    QTest::addColumn<int>("id_unidad_padre");
    QTest::addColumn<QString>("nombre");
    QTest::addColumn<double>("multiplicador");
    QTest::addColumn<bool>("resultado");
    QTest::newRow("Padre") << 0 << "UnidadPadre" << 1.0 << true;
    QTest::newRow("Hijo1") << 1 << "UnidadHija" << 1.0 << true;
    QTest::newRow("PadreIncorrecto") << 0 << "" << 1.0 << false;
    QTest::newRow("PadreIncorrecto2") << 0 << "UnidadPadre" << 0.0 << false;
    QTest::newRow("HijoIncorrecto") << 1 << "" << 1.0 << false;
    QTest::newRow("HijoIncorrecto2") << 1 << "UnidadPadre" << 0.0 << false;
}

/**
 * @brief UnidadesProductosTest::tstArbolUnidadesProductos
 */
void UnidadesProductosTest::tstArbolUnidadesProductos()
{
    MUnidadesProductosArbol *mupa = new MUnidadesProductosArbol();
    delete mupa;
}


QTEST_MAIN(UnidadesProductosTest)

#include "tst_unidadesproductostest.moc"
