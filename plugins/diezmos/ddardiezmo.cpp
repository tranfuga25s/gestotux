#include "ddardiezmo.h"

#include <QDateEdit>
#include <QMessageBox>
#include <QSqlDatabase>

#include "mdiezmos.h"

DDarDiezmo::DDarDiezmo(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setObjectName( "ingreso_diezmo" );
    this->setWindowTitle( "Ingresar recibo de diezmo" );
    this->setWindowIcon( QIcon( ":/imagenes/ingreso_diezmo.png" ) );

    DSBMonto->setMinimum( 0.0 );
    DEFecha->setDate( QDate::currentDate() );

    this->adjustSize();
}

/*!
 * \brief DDarDiezmo::accept
 *
 */
void DDarDiezmo::accept()
{
    if( !DEFecha->date().isValid() ) {
        QMessageBox::information( this,
                                  "Incorrecto",
                                  "La fecha ingresada es incorrecta" );
        return;
    }
    if( !QSqlDatabase::database().transaction() ) {
        QMessageBox::warning( this,
                              "Error",
                              "No se pudo inicar la transacion. Intente nuevamente." );
        return;
    }
    MDiezmos *modelo = new MDiezmos( this );
    if( !modelo->agregarReciboDiezmo( DEFecha->date(), DSBMonto->value() ) ) {
        QMessageBox::warning( this,
                              "Incorrecto",
                              "No se pudo ingresar el diezmo" );
        return;

    }
    if( !QSqlDatabase::database().commit() ) {
        QMessageBox::warning( this,
                              "Incorrecto",
                              "No se pudo hacer el guardado de los cambios" );
        return;
    }
    QMessageBox::information( this,
                              "Correcto",
                              "El diezmo fue agregado correctamente" );
    return QDialog::accept();
}

/*!
 * \brief DDarDiezmo::changeEvent
 * \param e
 */
void DDarDiezmo::changeEvent(QEvent *e)
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
