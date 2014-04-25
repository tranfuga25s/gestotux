#include "dagregargarantia.h"

#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QDebug>
#include <QRegExp>
#include <QSqlQuery>
#include <QSqlError>

#include "mequipamiento.h"
#include "ecbequipamiento.h"
#include "preferencias.h"
#include "egarantiasvg.h"
#include "mvgarantiassvg.h"
#include "mgarantias.h"
#include "NumeroComprobante.h"

DAgregarGarantia::DAgregarGarantia( QWidget *parent ) :
QDialog( parent ), _proxima_garantia( this, -1, -1 )
{
    setupUi(this);
    setWindowTitle( QString::fromUtf8( "Agegar nueva garantía" ) );
    setWindowIcon( QIcon( ":/imagenes/garantia_agregar.png" ) );

    _id_cliente = -1;
    _id_comprobante = -1;
    _id_producto = -1;
    _nombre_producto = QString();

    connect( CBCliente, SIGNAL( cambioIdCliente( int ) ), this, SLOT( buscarEquipamientos( int ) ) );
    connect( CBEquipamiento, SIGNAL( cambioId( int ) ), this, SLOT( buscarFactura( int ) ) );

    connect( DECompra, SIGNAL( dateChanged( QDate ) ), this, SLOT( actualizarFechaFinGarantia( QDate ) ) );
    DECompra->setDate( QDate::currentDate() );

    CkBImprimir->setChecked( true );
    CkBAvisarEmail->setEnabled( false ); /// @TODO: Habilitar esto cuando se genere el sistema de alertas automaticas

    LECodigoGarantia->setReadOnly( true );
    LECodigoGarantia->setText( MGarantias::obtenerProximoComprobante().aCadena() );

    this->adjustSize();
}

/*!
 * \brief DAgregarGarantia::setearIdProducto
 * \param id_producto Identificador del producto
 */
void DAgregarGarantia::setearIdProducto( const int id_producto )
{
    if( id_producto > 0 ) {
        _id_producto = id_producto;
    }
}

/*!
 * \brief DAgregarGarantia::setearNombreProducto
 * \param nombre_producto Nombre del producto
 */
void DAgregarGarantia::setearNombreProducto( const QString nombre_producto )
{
    if( !nombre_producto.isEmpty() ) {
        _nombre_producto = nombre_producto;
        CBEquipamiento->insertItem( 0, nombre_producto );
    }
}

/*!
 * \brief DAgregarGarantia::setearIdCliente
 * \param id_cliente Identificador del cliente
 */
void DAgregarGarantia::setearIdCliente( const int id_cliente )
{
    _id_cliente = id_cliente;
    CBCliente->setearId( id_cliente );
}

/*!
 * \brief DAgregarGarantia::setearIdComprobante
 * \param id_comprobante Identificador del Comprobante
 */
void DAgregarGarantia::setearIdComprobante( const int id_comprobante )
{
    _id_comprobante = id_comprobante;
    LEFactura->setText( MFactura::obtenerComprobante( _id_comprobante ).aCadena() );
    DECompra->setDate( MFactura::obtenerFecha( _id_comprobante ) );

    actualizarFechaFinGarantia( DECompra->date() );
}

/*!
 * \brief DAgregarGarantia::setearIdEquipamiento
 * \param id_equipamiento
 */
void DAgregarGarantia::setearIdEquipamiento( const int id_equipamiento )
{
    _id_equipamiento = id_equipamiento;
    CBEquipamiento->setearId( id_equipamiento );
}

/*!
 * \brief DAgregarGarantia::actualizarFechaFinGarantia
 * \param fecha
 */
void DAgregarGarantia::actualizarFechaFinGarantia( QDate fecha ) {
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Garantias" );
    DEFin->setDate( fecha.addMonths( p->value( "duracion_garantia", 6 ).toInt() ) );
    p->endGroup();
    p->endGroup();
    p=0;
}

/*!
 * \brief DAgregarGarantia::accept
 */
void DAgregarGarantia::accept()
{
    if( _id_cliente == -1 )
        _id_cliente = CBCliente->idClienteActual();

    if( _id_producto == -1 )
        _id_producto = CBEquipamiento->idActual();

    if( _nombre_producto.isEmpty() )
        _nombre_producto = CBEquipamiento->currentText();

    if( _id_comprobante == -1 ) {
        // Si no esta seteado significa que el elemento corresponde a una factura ingresada manualmente
        // o que fallo la cola de averiguacion
        if( LEFactura->text().isEmpty() ) {
            // No tiene numero de factura
            _id_comprobante = 0; // Para que se pueda guardar igualmente
            _id_producto = 0;
        } else {
            // Tiene forma de expresion regular?
            QRegExp expresion( "^\\d{5}\\-{1}\\d{5}$" );
            if( expresion.indexIn( LEFactura->text() ) == -1 ) {
                QMessageBox::information( this,
                                          QString::fromUtf8( "No coincide" ),
                                          QString::fromUtf8( "El contenido del numero de factura no parece valido. No se buscará el numero de comprobante" ) );
                _id_comprobante = 0;
                _id_producto = 0;
            } else {
                /// @TODO: Buscar numeros de comprobante
                _id_comprobante = 0;
                _id_producto = 0;
            }
        }
    }

    if( _id_cliente == -1 || _id_comprobante == -1 || _id_producto == -1
        || _nombre_producto.isEmpty() ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "No se setearon correctamente los datos" ) );
        return;
    }

    if( !QSqlDatabase::database().transaction() ) {
        QMessageBox::information( this, "Error", "No se pudo empezar la transacción" );
        return;
    }

    MGarantias modelo;
    int id_garantia = modelo.agregarGarantia( MGarantias::obtenerProximoComprobante(),
                                              _id_cliente,
                                              _nombre_producto,
                                              DECompra->date(),
                                              DEFin->date(),
                                              CBEquipamiento->idActual(),
                                              _id_producto,
                                              _id_comprobante );

    if( id_garantia == -1 ) {
        QMessageBox::information( this, "Error", "No se pudo guardar la garantía" );
        QSqlDatabase::database().rollback();
        return;
    }


    if( !QSqlDatabase::database().commit() ) {
        QMessageBox::information( this, "Error", "No se pudo hacer el commit" );
        return;
    } else {

        emit actualizarModelos();

        if( CkBImprimir->isChecked() ) {
            // Imprimo la nueva garantia
            EGarantiaSVG svg;
            MVGarantiasSvg msvg;
            svg.setearRegistro( msvg.obtenerRegistro( id_garantia ) );
            svg.cargarDatos();

            QPrinter printer;

            QPrintDialog *dialog = new QPrintDialog( &printer, this );
            dialog->setWindowTitle( QString::fromUtf8( "Imprimir garantía" ) );
            if( dialog->exec() != QDialog::Accepted ) {
                svg.imprimir( &printer );
            }
        }
    }
    QDialog::accept();
}

/*!
 * \brief DAgregarGarantia::changeEvent
 * \param e
 */
void DAgregarGarantia::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \brief DAgregarGarantia::buscarEquipamientos
 * Genera la lista de equipamientos posibles para el cliente seleccionado
 * \param id_cliente Identificador del cliente
 */
void DAgregarGarantia::buscarEquipamientos( int id_cliente )
{
    if( id_cliente <= 0 ) { return; }
    //qDebug() << "Numero de cliente: "<<id_cliente;
    _id_cliente = id_cliente;
    //qDebug() << "Aplicando filtro: " << QString( " WHERE id_cliente = %1" ).arg( id_cliente );
    CBEquipamiento->setearFiltro( QString( " WHERE id_cliente = %1" ).arg( id_cliente ), true  );

    if( !MEquipamiento::existeEquipamientoParaCliente( id_cliente ) ) {
        //qDebug() << "No existen equipamientos para el cliente!";
        CBEquipamiento->setEditable( true );
    }
}

/*!
 * \brief DAgregarGarantia::buscarFactura
 * Busca si existe el identificador de equipamiento y si tiene factura de compra
 * \param id_equipamiento Identificador de equipamiento
 */
void DAgregarGarantia::buscarFactura( int id_equipamiento )
{
    if( id_equipamiento <= 0 )
    { return; }
    qDebug() << "Numero Equipamiento: "<< id_equipamiento;

    if( modelo_equipamiento != 0 )
    { modelo_equipamiento = new MEquipamiento( this ); }

    if( !modelo_equipamiento->cargarDatos( id_equipamiento ) ) {
        qDebug() << "No hay datos!";
        return;
    }

    setearIdComprobante( modelo_equipamiento->numeroComprobante() );
}

/*!
 * \brief MFactura::obtenerComprobante
 * \param id_factura
 * \return
 */
NumeroComprobante & MFactura::obtenerComprobante( const int id_factura ) {
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT serie, numero FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
    if( cola.next() ) {
        int serie = cola.record().value(0).toInt();
        int numero = cola.record().value(1).toInt();
        NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
        return *num;
    } else {
        qDebug( "Error de cola al hacer next para obtener el numero de comprobante de factura");
    }
  } else {
    qDebug( "Error de cola al hacer exec para obtener el numero de comprobante maximo" );
 }
  NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
  return *invalido;
}

/*!
 * \fn MFacura::obtenerFecha( const int id_factura )
 * Devuelve la fecha de la factura pasada con parametro
 * \param id_factura identificador de factura
 */
QDate MFactura::obtenerFecha(const int id_factura)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
        cola.next();
        return cola.record().value(0).toDate();
    } else {
        qWarning( "Error al buscar la fecha de la factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}
