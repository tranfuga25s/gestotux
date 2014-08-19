#include "dunidadproducto.h"
#include "ui_dunidadproducto.h"

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

    QDialog::accept();
}
