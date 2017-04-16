#include "ebusqueda.h"

#include <QSqlQuery>
#include <QDebug>

EBusqueda::EBusqueda( QWidget *parent, QSqlTableModel *modelo, QString titulo ) :
    QDockWidget( parent ), Ui::EBusquedaBase()
{
    setupUi( this );
    setObjectName( "dock-busqueda" );
    if( titulo.isEmpty() || titulo.isNull() ) {
        this->setWindowTitle( "Buscador y filtrador" );
    } else { this->setWindowTitle( titulo ); }

    connect( CBTipo  , SIGNAL( currentIndexChanged( int ) ), this, SLOT( filtrar() ) );
    connect( LETexto , SIGNAL( textChanged( QString )     ), this, SLOT( filtrar() ) );
    connect( PBBorrar, SIGNAL( clicked()                  ), this, SLOT( borrar()  ) );

    _modelo = modelo;
    // Guardo el filtro anterior
    _filtro_anterior = _modelo->filter();

    this->setAttribute( Qt::WA_DeleteOnClose );
}

/*!
 * \brief EBusqueda::filtrar
 * Permite generar el filtro al modelo necesario y aplicarlo para el filtrado
 * \author Esteban Zeller
 */
void EBusqueda::filtrar()
{
    if( filtros.isEmpty() || CBTipo->currentIndex() == -1
        || LETexto->text().isEmpty() || LETexto->text().isNull() )
    { return; }
    _modelo->setFilter( filtros.at( CBTipo->currentIndex() ).arg( LETexto->text() ) );
    _modelo->select();
}

/*!
 * \brief EBusqueda::borrar
 * Borra todos los filtros que hayan aplicado.
 */
void EBusqueda::borrar()
{
    this->_modelo->setFilter( _filtro_anterior );
    this->_modelo->select();
    this->CBTipo->setCurrentIndex( -1 );
    this->LETexto->setText( "" );
}

/*!
 * \fn EBusqueda::agregarFiltro( QString nombre, QString filtro )
 * Funcion que agregará un filtro al sistema de filtrado automatico.
 * Este sistema colocará el nombre como item de la lista desplegable.
 * El parametro filtro deberà ser lo que le sigue al WHERE de una consulta select donde los %1 serán reemplazados por lo ingresado en el cuadro de texto.
 * \param nombre Nombre del filtro tal como aparecera en la lista deplegable.
 * \param filtro Filtro a a plicar a la consulta sql donde %1 será reemplazado por el texto ingresado por el usuario.
 */
void EBusqueda::agregarFiltro( QString nombre, QString filtro )
{
    if( nombre.isEmpty() || filtro.isEmpty() || nombre.isNull() || filtro.isNull() )
    {
        qWarning( "EBusqueda: esta intentando insertar un filtro con algún dato vacío." );
        return;
    }
    CBTipo->insertItem( CBTipo->count(), nombre );
    filtros.insert( filtro.size(), filtro );
}

/*!
 * \brief EBusqueda::changeEvent
 * \param e
 */
void EBusqueda::changeEvent( QEvent *e )
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange: {
            retranslateUi(this);
            break;
        }
        case QEvent::Hide:
        {
            borrar();
            emit cerrando();
            break;
        }
        default:
        { break; }
    }
}

/*!
 * \brief EBusqueda::closeEvent
 * \param event
 */
void EBusqueda::closeEvent( QCloseEvent * )
{
    //delete this->_modelo;
    //borrar();
    emit cerrando();
}

/*!
 * \brief EBusqueda::setearTitulo
 * Setea el titulo que aparecerá en la ventana de busqueda
 * \param titulo titulo a colocar
 */
void EBusqueda::setearTitulo( QString titulo )
{ this->setWindowTitle( titulo ); }
