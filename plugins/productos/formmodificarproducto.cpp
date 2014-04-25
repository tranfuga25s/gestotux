#include "formmodificarproducto.h"

#include "preferencias.h"
#include "dproductos.h"
#include "mcategorias.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>

FormModificarProducto::FormModificarProducto( MProductos *mod, QWidget *parent) :
QDialog(parent), Ui::FormProductoBase()
{
    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    setObjectName( "FormModificarProducto" );
    setWindowTitle( "Modificando Producto" );

    connect( DSBCosto, SIGNAL( valueChanged( double ) ), this, SLOT( cambioPrecioCosto( double ) ) );

    this->modelo = mod;

    mapa = new QDataWidgetMapper( this );
    mapa->setModel( modelo );
    mapa->setOrientation( Qt::Horizontal );
    this->_anterior = modelo->editStrategy();
    this->modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
    mapa->addMapping( this->LECodigo     , modelo->fieldIndex( "codigo"       ) );
    mapa->addMapping( this->LENombre     , modelo->fieldIndex( "nombre"       ) );
    mapa->addMapping( this->LEDescripcion, modelo->fieldIndex( "descripcion"  ) );
    mapa->addMapping( this->LEModelo     , modelo->fieldIndex( "modelo"       ) );
    mapa->addMapping( this->LEMarca      , modelo->fieldIndex( "marca"        ) );
    mapa->addMapping( this->SBStock      , modelo->fieldIndex( "stock"        ) );
    mapa->addMapping( this->DSBCosto     , modelo->fieldIndex( "precio_costo" ) );
    mapa->addMapping( this->DSBVenta     , modelo->fieldIndex( "precio_venta" ) );
    mapa->addMapping( this->CBCategoria  , 1 );
    mapa->setItemDelegate( new DProductos( this ) );
    mapa->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );

    this->GBContenedor->setTitle( "Modificar Producto" );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito la categoria del producto si esta habilitado
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    if( ! p->value( "categorias" ).toBool() )
    {
      this->LCategoria->setVisible( false );
      this->CBCategoria->setVisible( false );
        _categorias = false;
    } else {
        // Cargo las cateogrías que haya
        _categorias = true;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito la descripcion si esta habilitado
    if( ! p->value( "descripcion" ).toBool() )
    {
        this->LDescripcion->setVisible( false );
        this->LEDescripcion->setVisible( false );
        _descripcion = false;
    } else { _descripcion = true; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito la marca del producto si esta habilitado
    if( ! p->value( "marcas" ).toBool() )
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
    if( ! p->value( "stock" ).toBool() ) {
       this->LStock->setVisible( false );
       this->SBStock->setVisible( false );
        _stock = false;
    } else { _stock = false; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solicito el modelo si esta habilitado
    if( ! p->value( "modelo" ).toBool() ) {
        this->LModelo->setVisible( false );
        this->LEModelo->setVisible( false );
        _modelo = false;
    } else { _modelo = true; }
    /// Cargo el recargo para hacer los calculos sin tener que consultarlo todas las veces
    _recargo = p->value( "ganancia", 10.0 ).toDouble();
    p->endGroup();
    p->endGroup();
    p=0;
}

void FormModificarProducto::cambioPrecioCosto( double precio ) {
    this->DSBVenta->setValue( ( 1.00 + ( _recargo / 100 ) ) * precio );
}

#include <QSqlRecord>
void FormModificarProducto::accept() {
    // Verificaciones de estado correcto
    if( this->LECodigo->text().isEmpty() ) {
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
    if( this->LEMarca->text().isEmpty() && _marca ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El código del producto no puede ser nulo. Por favor, ingrese un código para el producto" ) );
        return;
    }
    if( this->CBCategoria->currentIndex() == 0 && _categorias ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("La categoría del producto no puede ser nula. Por favor, seleccióne una categoría para el producto" ) );
        return;
    }
    if( this->DSBCosto->value() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El precio de costo del producto no puede ser cero. Por favor, ingrese un precio de costo para el producto" ) );
        return;
    }*/
    if( this->DSBVenta->value() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("El precio de venta del producto no puede ser nulo. Por favor, ingrese un precio de venta para el producto" ) );
        return;
    }
    // Dar de alta con stock cero?
    if( this->SBStock->value() == 0 && _stock ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8( "El stock inicial del producto no puede ser nulo. Por favor, ingrese un stock inicial para el producto" ) );
        return;
    }
    if( this->LECodigo->text() != this->_codigo_anterior ) {
        if( MProductos::existeCodigo( this->LECodigo->text() ) ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "El codigo ingresado %1 ya esta dado de alta" ).arg( this->LECodigo->text() ) );
            return;
        }
    }
    if( this->LENombre->text() != this->_nombre_anterior ) {
        if( MProductos::existeNombre( this->LENombre->text() ) ) {
            QMessageBox::warning( this, "Error", QString::fromUtf8( "El codigo ingresado %1 ya esta dado de alta" ).arg( this->LECodigo->text() ) );
            return;
        }
    }
    if( mapa->submit() ) {
        if( modelo->submitAll() ) {
            QMessageBox::information( this, "Correcto", "Los cambios fueron guardados correctamente" );
            this->modelo->setEditStrategy( _anterior );
            QDialog::accept();
            return;
        } else {
            QMessageBox::information( this, "Erroneo", "No se pudo hacer el submit del modelo" );
            qDebug( this->modelo->lastError().text().toLocal8Bit() );
            return;
        }
    } else {
        QMessageBox::information( this, "Erroneo", "No se pudo hacer el submit de los datos" );
        qDebug( this->modelo->lastError().text().toLocal8Bit() );
        return;
    }
}


void FormModificarProducto::setearProducto( const int row )
{
    if( row >= 0 ) {
        mapa->setCurrentIndex( row );
        this->_codigo_anterior = this->LECodigo->text();
        this->_nombre_anterior = this->LENombre->text();
    }
}
