#include "formagregarproducto.h"
#include "preferencias.h"
#include <QMessageBox>
#include <QSqlQuery>
#include "mproductos.h"

FormAgregarProducto::FormAgregarProducto(QWidget *parent) :
  QDialog( parent ), Ui::FormProductoBase()
{
    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    setObjectName( "FormAgregarProducto" );
    setWindowTitle( "Agregar nuevo producto" );

    connect( DSBCosto, SIGNAL( valueChanged( double ) ), this, SLOT( cambioPrecioCosto( double ) ) );
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito la categoria del producto si esta habilitado
    if( ! preferencias::getInstancia()->value( "Preferencias/Productos/categorias" ).toBool() )
    {
      this->LCategoria->setVisible( false );
      this->CBCategoria->setVisible( false );
        _categorias = false;
    } else {
        // Cargo las cateogrías que haya
        _categorias = true;
    }
    /////////////
    // Veo si está oculto el codigo del producto
    if( preferencias::getInstancia()->value( "Preferencias/Productos/ocultar_codigo" ).toBool() )
    {
      this->LCodigo->setVisible( false );
      this->LECodigo->setVisible( false );
      _codigo_oculto = true;
    } else { _codigo_oculto = false; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito la descripcion si esta habilitado
    if( ! preferencias::getInstancia()->value( "Preferencias/Productos/descripcion" ).toBool() )
    {
        this->LDescripcion->setVisible( false );
        this->LEDescripcion->setVisible( false );
        _descripcion = false;
    } else { _descripcion = true; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito la marca del producto si esta habilitado
    if( ! preferencias::getInstancia()->value( "Preferencias/Productos/marcas" ).toBool() )
    {
        this->LMarca->setVisible( false );
        this->LEMarca->setVisible( false );
        _marcas = false;
    } else {
        // cargo el autocompletado de las marcas
        _marcas = true;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito el stock del producto si esta habilitado
    if( ! preferencias::getInstancia()->value( "Preferencias/Productos/stock" ).toBool() ) {
       this->LStock->setVisible( false );
       this->SBStock->setVisible( false );
        _stock = false;
    } else { _stock = false; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito el modelo si esta habilitado
    if( ! preferencias::getInstancia()->value( "Preferencias/Productos/modelo" ).toBool() ) {
        this->LModelo->setVisible( false );
        this->LEModelo->setVisible( false );
        _modelo = false;
    } else { _modelo = true; }
    /// Cargo el recargo para hacer los calculos sin tener que consultarlo todas las veces
    _recargo = preferencias::getInstancia()->value( "Preferencias/Productos/ganancia", 10.0 ).toDouble();

    // Tiene que ser positivo para que tenga sentido la comparación con numeros negativos desde mproductos totales.
    _id_anterior = 100;
}

void FormAgregarProducto::cambioPrecioCosto( double precio ) {
    this->DSBVenta->setValue( ( 1.00 + ( _recargo / 100 ) ) * precio );
    this->DSBVenta->setMinimum( precio );
}

void FormAgregarProducto::accept() {
    // Verificaciones de estado correcto
    if( this->LECodigo->text().isEmpty() && !_codigo_oculto ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("El código del producto no puede ser nulo. Por favor, ingrese un código para el producto" ) );
        return;
    }
    if( this->LENombre->text().isEmpty() ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("El nombre del producto no puede ser nulo. Por favor, ingrese un nombre para el producto" ) );
        return;
    }
/*    if( this->LEDescripcion->text().isEmpty() && _descripcion ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("La descripción del producto no puede ser nula. Por favor, ingrese una descripción para el producto" ) );
        return;
    }
    if( this->LEMarca->text().isEmpty() && _descripcion ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El código del producto no puede ser nulo. Por favor, ingrese un código para el producto" ) );
        return;
    }*/
    if( this->CBCategoria->currentIndex() == 0 && _categorias ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("La categoría del producto no puede ser nula. Por favor, seleccióne una categoría para el producto" ) );
        return;
    }
    if( this->DSBCosto->value() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El precio de costo del producto no puede ser cero. Por favor, ingrese un precio de costo para el producto" ) );
        return;
    }
    if( this->DSBVenta->value() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("El precio de venta del producto no puede ser nulo. Por favor, ingrese un precio de venta para el producto" ) );
        return;
    }
    // Dar de alta con stock cero?
    if( this->SBStock->value() == 0 && _stock ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El stock inicial del producto no puede ser nulo. Por favor, ingrese un stock inicial para el producto" ) );
        return;
    }
    MProductos *m = new MProductos( this );
    // Verifico que exista el codigo
    if( m->existeCodigo( this->LECodigo->text() ) ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El producto con codigo %1 ya ha sido dado de alta!" ).arg( this->LECodigo->text() ) );
        return;
    }
    // Verifico que exista el nombre
    if( m->existeNombre( this->LENombre->text() ) ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El producto con nombre %1 ya ha sido dado de alta!" ).arg( this->LENombre->text() ) );
        return;
    }
    // Todos los datos pasaron bien luego de este punto
    if( m->agregarProducto(
                this->LECodigo->text(),
                this->LENombre->text(),
                this->DSBCosto->value(),
                this->DSBVenta->value(),
                this->SBStock->value(), 0,
                //this->CBCategoria->idActual(),
                this->LEDescripcion->text(),
                this->LEMarca->text(),
                this->LEModelo->text() ) ) {
        QMessageBox::information( this, "Correcto", QString::fromUtf8( "El producto se agregó correctamente" ) );
        if( _id_anterior < 0 ) {
            emit agregarProducto( _id_anterior,  m->idProductoPorCodigo( LECodigo->text() )  );
        }
        QDialog::accept();
        delete m;
        return;
    } else {
        delete m;
        QMessageBox::warning( this, "Erroneo", "No se pudo agregar el producto." );
        return;
    }
}

/*!
 * \fn FormAgregarProducto::setearNombre( const QString nombre )
 * Coloca como predeterminado el nombre del producto
 * \param nombre Nombre del producto a agregar
 */
void FormAgregarProducto::setearNombre(const QString nombre)
{ this->LENombre->setText( nombre ); }

/*!
 * \fn FormAgregarProducto::setearStockInicial( const int cantidad )
 * Coloca como predeterminado la cantidad de stock inicial del producto
 * \param cantidad stock inicial del producto a agregar
 */
void FormAgregarProducto::setearStockInicial( const int cantidad )
{ this->SBStock->setValue( cantidad ); }

/*!
 * \brief FormAgregarProducto::setearDesdeCompra
 * \param estado
 */
void FormAgregarProducto::setearDesdeCompra( bool estado )
{
    if( estado ) {
        _stock = false;
        LStock->setVisible( false );
        SBStock->setVisible( false );
    } else {
        _stock = true;
        LStock->setVisible( true );
        SBStock->setVisible( true );
    }
}

/*!
 *
 */
void FormAgregarProducto::setearProveedor( QString proveedor )
{
    this->_nombre_proveedor = proveedor;
    if( preferencias::getInstancia()->value( "Preferencias/Productos/marca_proveedor" ).toBool() ) {
        this->LEMarca->setText( proveedor );
    }
}

/*!
 * \fn FormAgregarProducto::setearPrecioCosto( const double cantidad )
 * Coloca como predeterminado el precio de costo del producto
 * \param cantidad precio de compra del producto a agregar
 */
void FormAgregarProducto::setearPrecioCosto(const double precio)
{ this->DSBCosto->setValue( precio ); }
