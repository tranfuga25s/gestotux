#include "paginacliente.h"

#include <QVBoxLayout>
#include <QLineEdit>

#include "mclientes.h"
#include "ordentrabajowizard.h"

PaginaCliente::PaginaCliente(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle( "Seleccione el cliente" );
    setSubTitle( QString::fromUtf8( "Este dialogo le ayudará en la creación de una nueva orden de trabajo." ) );
    //setPixmap( QWizard::WatermarkPixmap, QPixmap( ":/images/watermark.png" ) );

    arriba = new QLabel( this );
    arriba->setText( QString::fromUtf8( "Por favor, seleccione un cliente, para crear un nuevo cliente escriba un nombre directamente" ) );
    arriba->setWordWrap(true);

    CBClientes = new ECBClientes( this );
    CBClientes->setEditable( true );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(arriba);
    layout->addWidget(CBClientes);
    setLayout(layout);

    QLineEdit *temp = new QLineEdit( this );
    temp->setVisible( false );

    // Asocio el valor con el identificador del cliente
    registerField( "cliente.id_cliente", CBClientes, "idActual" );
    registerField( "cliente.texto_ingresado", temp );
}

/*!
 * \brief PaginaCliente::nextId
 * \return
 */
int PaginaCliente::nextId() const
{
    wizard()->setField( "cliente.texto_ingresado", CBClientes->currentText() );
    if( MClientes::existe( CBClientes->currentText(), QString() ) ) {
        return OrdenTrabajoWizard::Pagina_Equipamiento;
    } else {
        return OrdenTrabajoWizard::Pagina_ClienteNuevo;
    }
}
