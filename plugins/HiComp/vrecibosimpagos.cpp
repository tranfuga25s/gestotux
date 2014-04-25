#include "vrecibosimpagos.h"

#include "mrecibosimpagos.h"
#include "eactcerrar.h"
#include "dsino.h"
#include "DPagarRecibo.h"

#include <QTableView>
#include <QMessageBox>

VRecibosImpagos::VRecibosImpagos( QWidget *parent ) :
EVLista( parent )
{
    this->setWindowTitle( "Recibos Impagos" );
    this->setWindowIcon( QIcon( ":/imagenes/recibos.png" ) );
    this->setObjectName( "recibos_impagos" );

    this->modelo = new MRecibosImpagos( this );
    this->vista->setModel( this->modelo );
    this->vista->hideColumn( 0 );
    this->vista->setItemDelegateForColumn( this->modelo->fieldIndex( "cancelado" ), new DSiNo( this->vista ) );
    this->modelo->select();

    ActPagar = new QAction( this );
    ActPagar->setText( QString::fromUtf8( "Pagar" ) );
    ActPagar->setStatusTip( "Pagar recibos seleccionados" );
    connect( ActPagar, SIGNAL( triggered() ), this, SLOT( pagar() ) );

    this->addAction( ActPagar );
    this->addAction( new EActCerrar( this ) );
}

/*!
 * \brief VRecibosImpagos::pagar
 */
void VRecibosImpagos::pagar()
{
    if( this->vista->selectionModel()->selectedRows().size() <= 0 ) {
        QMessageBox::information( this, "Error", QString::fromUtf8( "Por favor, seleccione algún recibo para pagar" ) );
        return;
    }

    foreach( QModelIndex idx, this->vista->selectionModel()->selectedRows( 0 ) ) {
        int id_recibo = this->modelo->data( this->modelo->index( idx.row(), 0 ), Qt::EditRole ).toInt();
        // Largo el dialogo para pagar
        DPagarRecibo *diag = new DPagarRecibo( this );
        if( diag->setearIdRecibo( id_recibo ) ) {
            diag->exec();
        }
    }
    this->modelo->select();
}
