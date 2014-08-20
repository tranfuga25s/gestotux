#include "dunidadproducto.h"
#include "ui_dunidadproducto.h"

#include <QMessageBox>


DUnidadProducto::DUnidadProducto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DUnidadProducto)
{
    ui->setupUi(this);
    modelo = 0;
}

DUnidadProducto::~DUnidadProducto()
{
    delete ui;
}

void DUnidadProducto::accept()
{
    if( ui->LENombre->text().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Ingrese un nombre para la unidad" );
        return;
    }
    if( ui->DSBMultiplicador->value() == 0.0 ) {
        QMessageBox::warning( this, "Error", "Ingrese un multiplicador distinto de cero para la unidad" );
        return;
    }

    // Guardo la unidad
    if( this->modelo == 0 ) {
        QMessageBox::warning( this, "Error", "Modelo no setteado" );
        return;
    }

    if( this->modelo->agregarUnidad( 0 /*ui->CBPadre->idActual()*/,
                                     ui->LENombre->text(),
                                     ui->DSBMultiplicador->value() ) ) {
        QMessageBox::information( this, "Correcto", "Unidad agregada correctamente" );
        QDialog::accept();
    } else {
        QMessageBox::warning( this, "Error", "No se pudo agregar el elemento" );
        return;
    }


}
