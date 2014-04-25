/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QDate>
#include <QPair>
#include <QLocale>
#include <QTimer>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QDebug>

#include "mperiodoservicio.h"
#include "FormFacturarServicio.h"
#include "eactcerrar.h"
#include "mservicios.h"
#include "mrecargos.h"
#include "MTempClientesFacturarServicio.h"
#include "dsino.h"
#include "eregistroplugins.h"

FormFacturarServicio::FormFacturarServicio(QWidget *parent) :
EVentana(parent), _id_servicio(0)  {

    setupUi(this);

    this->setWindowTitle( "Facturacion de un servicio" );
    this->setWindowIcon( QIcon( ":/imagenes/facturar_servicio.png" ) );
    this->setObjectName( "facturaservicios" );

    ActCerrar = new EActCerrar( this );

    ActFacturar = new QAction( this );
    ActFacturar->setText( "Facturar" );
    ActFacturar->setStatusTip( "Factura el servicio con los clientes seleccionados" );
    ActFacturar->setIcon( QIcon( ":/imagenes/facturar_servicio.png" ) );
    connect( ActFacturar, SIGNAL( triggered() ), this, SLOT( facturar() ) );

    this->addAction( ActFacturar );
    this->addAction( ActCerrar );

    // Escondo el progreso
    this->GBProgreso->setVisible( false );
    this->PBCancelar->setText( "Cancelar" );

    this->setDisabled( true );

    this->LNombreServicio->setText( "Cargando..." );
    this->LPeriodo->setText( "Cargando..." );
    this->LPrecioBase->setText( "Cargando..." );

}

FormFacturarServicio::FormFacturarServicio( int id_servicio ) :
EVentana(0), _id_servicio( id_servicio )
{
    setupUi(this);

    this->setWindowTitle( "Facturacion de un servicio" );
    this->setWindowIcon( QIcon( ":/imagenes/facturar_servicio.png" ) );
    this->setObjectName( "facturaservicio" + MServicios::getNombreServicio( _id_servicio ) );

    ActCerrar = new EActCerrar( this );

    ActFacturar = new QAction( this );
    ActFacturar->setText( "Facturar" );
    ActFacturar->setStatusTip( "Factura el servicio con los clientes seleccionados" );
    ActFacturar->setIcon( QIcon( ":/imagenes/facturar_servicio.png" ) );
    connect( ActFacturar, SIGNAL( triggered() ), this, SLOT( facturar() ) );

    this->addAction( ActFacturar );
    this->addAction( ActCerrar );

    // Escondo el progreso
    this->GBProgreso->setVisible( false );
    this->PBCancelar->setText( "Cancelar" );

    this->setDisabled( true );

    this->LNombreServicio->setText( "Cargando..." );
    this->LPeriodo->setText( "Cargando..." );
    this->LPrecioBase->setText( "Cargando..." );

    QTimer::singleShot( 400, this, SLOT( cargar_datos_servicio() ) );
}

void FormFacturarServicio::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \fn FormFacturarServicio::setearServicio( const int id_servicio )
 *  Setea el id de servicio que se desea realizar la facturación
 * \param id_servicio ID del servicio
 */
void FormFacturarServicio::setearServicio(const int id_servicio)
{
    if( id_servicio > 0 ) {
        this->_id_servicio = id_servicio;
        QTimer::singleShot( 400, this, SLOT( cargar_datos_servicio() ) );
    } else {
        qWarning( "Servicios:FormFacturarServicio: Seteando un servicio < o = 0");
        return;
    }
}

/*!
 * \fn FormFacturarServicio::cargar_datos_servicio()
 * Carga los datos de un servicio seleccionado
 */
void FormFacturarServicio::cargar_datos_servicio()
{
    MServicios *m = new MServicios( this );
    this->LNombreServicio->setText( m->getNombreServicio( this->_id_servicio ) );
    this->_precio_base = m->precioBase( this->_id_servicio );
    this->LPrecioBase->setText( QString( "$ %L1" ).arg( this->_precio_base  ) );
    delete m;
    MPeriodoServicio *mp = new MPeriodoServicio();
    this->_periodo = mp->getPeriodoActual( this->_id_servicio, true );
    this->_ano = mp->getAnoActual( this->_id_servicio, true );
    this->_fecha_inicio = mp->getFechaInicioPeriodoActual( this->_id_servicio, true );
    this->_fecha_emision = mp->getFechaEmision( this->_id_servicio, this->_fecha_inicio );
    this->LFechaRecibo->setText( this->_fecha_emision.toString( Qt::SystemLocaleShortDate ) );
    this->LPeriodo->setText(
        QString( " %1/%2 desde %3 hasta %4 " )
                .arg( this->_periodo )
                .arg( this->_ano )
                .arg( _fecha_inicio.toString( Qt::SystemLocaleShortDate ) )
                .arg( mp->obtenerFechaFinPeriodo( this->_id_servicio, _fecha_inicio ).toString( Qt::SystemLocaleShortDate ) )
    );
    delete mp;
    // Cargo los clientes del servicio
    MTempClientesFacturarServicio *mc = new MTempClientesFacturarServicio( this );
    // Cargo los clientes
    this->TVClientes->setModel( mc );
    this->TVClientes->hideColumn( 2 );
    mc->cargarClientesDelServicioAFacturar( this->_id_servicio );
    this->TVClientes->setItemDelegateForColumn( 0, new DSiNo( this->TVClientes ) );
    this->TVClientes->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    this->TVClientes->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    this->TVClientes->horizontalHeader()->setMinimumSectionSize( 40 );
    // Cargo los recargos del servicio
    MRecargos *mr = new MRecargos( this, false );
    mr->setFilter( QString( "id_servicio = %1 " ).arg( this->_id_servicio ) );
    mr->setearPrecioBase( this->_precio_base );
    this->TVRecargos->setModel( mr );
    this->TVRecargos->hideColumn( 0 );
    this->TVRecargos->hideColumn( 1 );
    this->TVRecargos->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    // Arreglar problema con el item de si o no tamaño.
    mr->select();

    this->setEnabled( true );
}

#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QInputDialog>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include <QHash>
#include "EReporte.h"
#include "../pagos/mpagos.h"
#include "../CtaCte/mitemcuentacorriente.h"
#include "../CtaCte/mcuentacorriente.h"
#include "../ventas/MFactura.h"
#include "../ventas/mitemfactura.h"
#include "mcobroservicioclienteperiodo.h"
#include "mperiodoservicio.h"
/*!
 * \fn FormFacturarServicio::facturar()
 * Realiza la facturación efectiva del servicio. El usuario ya acepto el facturar y los datos.
 */
void FormFacturarServicio::facturar()
{
    // Verifico que haya algun cliente
    if( TVClientes->model()->rowCount() <= 0 ) {
        QMessageBox::information( this, "Sin clientes", QString::fromUtf8( "No existen clientes para este servicio. No se factuará nada." ), QMessageBox::Ok );
        return;
    }
    // Deshabilito los elementos editables
    this->GBClientes->setEnabled( false );
    this->GBRecargos->setEnabled( false );
    // Genero una barra de progreso y sus botones y demas
    this->GBProgreso->setVisible( true );
    LIndicador->setText( "Iniciando...");
    PBProgreso->setValue( 0 );
    connect( PBCancelar, SIGNAL( clicked() ), this, SLOT( cancelar() ) );
    this->_cancelar = false;

    // Cuento la cantidad de recibos a realizar
    int cantidad_total = qobject_cast<MTempClientesFacturarServicio *>(this->TVClientes->model())->rowCount();
    // usar para calcular cantidad de operaciones mostradas x recibo
    int multiplicador_pasos = 4;
    this->PBProgreso->setRange( 0, cantidad_total * multiplicador_pasos );


    // Inicializo el modelo  de las facturas o recibos
    QSqlTableModel *mr;
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
        mr = new MPagos();
    } else {
        mr = new MFactura();
    }
    MTempClientesFacturarServicio *mtemp = qobject_cast<MTempClientesFacturarServicio *>(this->TVClientes->model());

    // Inicializo los valores que voy a ir refrescando
    int id_cliente = -1;
    QString nombre_cliente = "";
    QList<int> comprobantes; // Guarda el paso con el id del recibo guardado

    // Genero la transación en la base de datos ( total )
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();

    //////////////////////////////////////////////////////////////////////////////
    // Genero los datos de cuando estoy cobrando
    MPeriodoServicio *mp = new MPeriodoServicio();
    int id_periodo_servicio = mp->agregarPeriodoAFacturarNuevo( this->_id_servicio );
    if( id_periodo_servicio == -1 ) {
        qDebug( "El metodo de agregar periodo a facturar nuevo devolvio error." );
        LIndicador->setText( "Error - No se pudo agregar el registro del periodo a facturar" );
        PBProgreso->setRange( 0, 1 );
        PBProgreso->setValue( 1 );
        QMessageBox::warning( this, "Cancelado", QString::fromUtf8( "No se guardo ninguna facturación para este servicio." ) );
        QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
        delete mr;
        return;
    } else if( id_periodo_servicio == -2 ) {
        // Ya se facturó este servicio.
        LIndicador->setText(  QString::fromUtf8( "No se emitió ningun comprobante" ) );
        PBProgreso->setRange( 0, 1 );
        PBProgreso->setValue( 1  );
        delete mr;
        return;
    }

    //////////////////////////////////////////////////////////////////////
    // Itero por cada uno de los clientes para este cobro de servicio
    for( int i = 0; i<cantidad_total; i++ ) {
        //qDebug( "<----" );
        // Veo si el elemento esta para ser facturado
        if( !mtemp->data( mtemp->index( i, 0 ), Qt::EditRole ).toBool() ) {
            qDebug() << "Item " << i << ": No se facturara";
            // Avanzo la cantidad de pasos que sería por facturarle
            PBProgreso->setValue( PBProgreso->value() + multiplicador_pasos );
            continue;
        }
        /// Estos serían los datos de "servicios_clientes"
        // Cargo el dato correspondiente a esa fila
        id_cliente = mtemp->data( mtemp->index( i, 2 ), Qt::DisplayRole ).toInt();
        //qDebug( QString( "Item %1-ID Cliente: %2." ).arg( i ).arg( id_cliente ).toLocal8Bit() );
        nombre_cliente = mtemp->data( mtemp->index( i, 1 ), Qt::DisplayRole ).toString();
        //qDebug( QString( "Item %1-Nombre Cliente: %2." ).arg( i ).arg( nombre_cliente ).toLocal8Bit() );

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Paso 1 - Genero la factura con los items actuales mas los anteriores segun corresponda /
        ///////////////////////////////////////////////////////////////////////////////////////////
        int id_factura = -1;
        if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
            LIndicador->setText( QString( "( %1 de %2 ) Generando recibo ..." ).arg( i +1 ).arg( cantidad_total ) );
            id_factura = qobject_cast<MPagos *>(mr)->agregarRecibo( id_cliente,
                                                                    this->_fecha_emision,
                                                                    QString( "%1 periodo %2/%3" ).arg( MServicios::getNombreServicio( this->_id_servicio ) ).arg( this->_periodo ).arg( this->_ano ),
                                                                    this->_precio_base,
                                                                    false, // No efectivo y no pagado para que quede para despues
                                                                    false,
                                                                    QString( "%2 periodo %1" ).arg( this->LPeriodo->text() ).arg( this->LNombreServicio->text() ) );
        } else {
            LIndicador->setText( QString( "Generando factura ( %1 de %2 )..." ).arg( i +1 ).arg( cantidad_total ) );
            id_factura = qobject_cast<MFactura *>(mr)->agregarFactura( id_cliente,
                                             QDateTime::currentDateTime(),
                                             MFactura::CuentaCorriente,
                                             this->_precio_base,
                                             false ); // Con este ultimo parametro no registra la operación de cuenta corriente, porque lo hago manualmente mas tarde.
        }
        if( id_factura == -1 ) {
            QMessageBox::warning( this, "Error", "No se pudo generar la factura para el cliente requerido - se cancelara toda la facturacion del servicio" );
            qDebug( "Error al generar la factura - id erroneo" );
            LIndicador->setText( QString( "Error - No se pudo agregar la factura  %1 para el cliente %2" ).arg( i+1 ).arg( nombre_cliente ) );
            PBProgreso->setRange( 0, 1 );
            PBProgreso->setValue( 1 );
            QMessageBox::warning( this, "Cancelado", QString::fromUtf8( "No se guardo ninguna facturación para este servicio. <br /> Consulte el error indicado y contacte al servicio tecnico para ayuda." ) );
            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
            delete mr;
            return;
        } else {
            // Si se genero correctamente ingreo el id en la cola para impresion luego.
            comprobantes.insert( i, id_factura );
            //qDebug( "Comprobante guardado correctamente - agregado a la cola para post impresion" );
        if( ! ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
            // Genero los items para el comprobante
            MItemFactura *fact = new MItemFactura();
            if( !fact->agregarItemFactura( id_factura,
                                           1,
                                           QString( "%1 periodo %2/%3" ).arg( MServicios::getNombreServicio( this->_id_servicio ) ).arg( this->_periodo ).arg( this->_ano ),
                                           this->_precio_base,
                                           -1 ) ) { // Coloco el ultimo parametro en -1 para que no registre ningun producto
                qDebug( "Error al intentar agregar el item de factura para el servicio del mes facturado" );
                LIndicador->setText( QString( "Error - No se pudo agregar el item de la factura %1 para el cliente %2" ).arg( i+1 ).arg( nombre_cliente ) );
                PBProgreso->setRange( 0, 1 );
                PBProgreso->setValue( 1 );
                QMessageBox::warning( this, "Cancelado", QString::fromUtf8( "No se guardo ninguna facturación para este servicio. <br /> Consulte el error indicado y contacte al servicio tecnico para ayuda." ) );
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                delete mr;
                return;
            }
        }

        }
        PBProgreso->setValue( PBProgreso->value() + 1 );
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // Paso 2 - Genero la entra en cobro-servicio-cliente-servicio con el id de factura correspondiente /
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        if( !MCobroServicioClientePeriodo::agregarCobro( id_periodo_servicio, this->_id_servicio, id_cliente, id_factura ) ) {
            qDebug( "Error al generar la entrada de cobro para un servicio-cliente-cobro" );
            LIndicador->setText( QString( "Error: No se pudo agregar la entrada servicio<->cliente<->cobro nº %1 para el cliente %2" ).arg( i+1 ).arg( nombre_cliente ) );
            PBProgreso->setRange( 0, 1 );
            PBProgreso->setValue( 1 );
            QMessageBox::warning( this, "Cancelado", QString::fromUtf8( "No se guardo ninguna facturación para este servicio. <br /> Consulte el error indicado y contacte al servicio tecnico para ayuda." ) );
            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
            delete mr;
            return;
        }
        PBProgreso->setValue( PBProgreso->value() + 1 );
        /////////////////////////////////////////////////////////////////
        // Paso 3 - Genero la entrada en la cuenta corriente del cliente
        /////////////////////////////////////////////////////////////////
        LIndicador->setText( QString( "( %2 de %3 ) Actualizando cuenta corriente del cliente %1  ..." ).arg( nombre_cliente ).arg( i + 1 ).arg( cantidad_total ) );
        // Intento agregar el numero de operación
        QString id_ctacte = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
        int id_op_ctacte = -1;
        if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
            // Esta operación ya se agrega automaticamente cuando se pone el recibo en formato "a Pagar luego"
            /*id_op_ctacte = MItemCuentaCorriente::agregarOperacion( id_ctacte,
                                                                   MPagos::buscarNumeroComprobantePorId( id_factura ),
                                                                   id_factura,
                                                                   MItemCuentaCorriente::Recibo,
                                                                   this->_fecha_emision,
                                                                   QString( "%2 periodo %1" ).arg( this->LPeriodo->text() ).arg( this->LNombreServicio->text() ),
                                                                   this->_precio_base * (-1.0) ); */
        } else {
             id_op_ctacte = MItemCuentaCorriente::agregarOperacion( id_ctacte,
                                                                    MFactura::obtenerComprobante( id_factura ),
                                                                    id_factura,
                                                                    MItemCuentaCorriente::Factura,
                                                                    this->_fecha_emision,
                                                                    QString( "%2 periodo %1" ).arg( this->LPeriodo->text() ).arg( this->LNombreServicio->text() ),
                                                                    this->_precio_base );

            if( id_op_ctacte == -1 ) {
                // Error al guardar el  item de cuenta corriente
                LIndicador->setText( QString( "Error: No se pudo actualizar la cuenta corriente para el cliente %2" ).arg( i+1 ).arg( nombre_cliente ) );
                PBProgreso->setRange( 0, 1 );
                PBProgreso->setValue( 1 );
                QMessageBox::warning( this, "Cancelado", QString::fromUtf8( "No se guardo ninguna facturación para este servicio. <br /> Consulte el error indicado y contacte al servicio tecnico para ayuda." ) );
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                delete mr;
                return;
                break;
            }
        }
        PBProgreso->setValue( PBProgreso->value() + 1 );
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Paso 4 - Actualizo la entrada en cobro-servicio-cliente-servicio con el  numero de operación de la cuenta corriente /
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if( !MCobroServicioClientePeriodo::setearIDCtaCte( id_periodo_servicio, this->_id_servicio, id_cliente, id_op_ctacte ) ) {
            qDebug( "Error al actualizar el indice de cuenta corriente en el cobro de servicio para el servicio cliente" );
            LIndicador->setText( QString( "Error: No se pudo guardar el identificador de ctacte para el cobro %1, cliente %2" ).arg( i+1 ).arg( nombre_cliente ) );
            PBProgreso->setRange( 0, 1 );
            PBProgreso->setValue( 1 );
            QMessageBox::warning( this, "Cancelado", QString::fromUtf8( "No se guardo ninguna facturación para este servicio. <br /> Consulte el error indicado y contacte al servicio tecnico para ayuda." ) );
            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
            delete mr;
            return;
        }
        PBProgreso->setValue( PBProgreso->value() + 1 );
        ///////////////////////////////////////
        // Fin bucle para cada cliente
        ///////////////////////////////////////
        //qDebug( "------------------------------> fin bucle " );
    }
    // Guardo todos los datos
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit();
    LIndicador->setText( QString::fromUtf8( "Correcto! - Iniciando impresión de comprobantes" ) );
    PBProgreso->setRange( 0, 1 );
    PBProgreso->setValue( 1 );

    // Inicializo el reporter
    EReporte *reporte = new EReporte( 0 );
    // Genero los parametros
    ParameterList lista;
    lista.append( "id_servicio", this->_id_servicio );
    lista.append( "fecha_inicio", this->_fecha_inicio );
    lista.append( "precio_base", this->_precio_base );

    if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
        reporte->recibo();
        reporte->especial( "Recibo-hicomp-venc", ParameterList() );

        QSqlQuery cola;
        if( cola.exec( QString( "SELECT cant_dias, recargo, porcentaje FROM recargos WHERE id_servicio = %1" ).arg( this->_id_servicio ) ) ) {
            int i = 1;
            while( cola.next() ) {
                if( cola.record().isNull( 1 ) ) {
                    lista.append( QString( "recargo%1" ).arg( i ), cola.record().value(2).toDouble() );
                    lista.append( QString( "total%1" ).arg( i ), this->_precio_base * ( 1 + ( cola.record().value(2).toDouble() / 100 ) ));
                } else {
                    lista.append( QString( "recargo%1" ).arg( i ), cola.record().value(1).toDouble() );
                    lista.append( QString( "total%1" ).arg( i ), this->_precio_base + cola.record().value(1).toDouble() );
                }
                lista.append( QString( "fecha%1" ).arg( i ), this->_fecha_inicio.addDays( cola.record().value("cant_dias" ).toInt() ).toString( "dd/MM/yyyy" ) );
                i++;
            }
            if( i < 4 ) {
                for( int j = i; j <= 4; j++ ) {
                    lista.append( QString( "recargo%1" ).arg( j ), "" );
                    lista.append( QString( "total%1" ).arg( j ), "" );
                    lista.append( QString( "fecha%1" ).arg( j ), "" );
                }
            }
        } else {
            qDebug( "HiComp::ReporteParametros::Recibo:: Error de exec de recargos" );
            qDebug() << cola.lastError().text();
            return;
        }
    } else {
        reporte->factura();
    }

    // Genero la configuraciòn de la impresora
    QPrinter *imp = new QPrinter( QPrinterInfo::defaultPrinter() );
    QPrintDialog *di = new QPrintDialog( imp, this );
    if( di->exec() ) {
        reporte->setearImpresora( imp );
    } else {
        QMessageBox::information( this, "Listo", QString::fromUtf8("Ha elegido no imprimir ningun comprobante ahora.<br /><b>Aun así, han quedado emitidos. Puede imprimirlos luego desde la lista de recibos" ) );
        return;
    }

    int contador = 0;
    PBProgreso->setRange( 0, comprobantes.size() );
    PBProgreso->setValue( 0 );
    while( comprobantes.size() > 0 ) {
        QApplication::processEvents();
        // Paso 3
        // Imprimir recibo
        lista.clear();
        int id_comp = comprobantes.takeFirst();
        if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
            lista.append( "id_recibo", id_comp );
            LIndicador->setText( QString::fromUtf8( "Imprimiendo recibo Nº %1 ( %2 de %3 )" ).arg( MPagos::buscarNumeroComprobantePorId( id_comp ).aCadena() ).arg( contador+1 ).arg( cantidad_total ) );
        } else {
            lista.append( "id_factura", id_comp );
            LIndicador->setText( QString::fromUtf8( "Imprimiendo factura Nº %1 ( %2 de %3 )" ).arg( MFactura::obtenerComprobante( id_comp ).aCadena() ).arg( contador+1 ).arg( cantidad_total ) );
        }
        if( !reporte->hacer( lista, false, false ) ) {
            qDebug() << "No se pudo hacer el reporte " << contador;
        }
        QApplication::processEvents();
        PBProgreso->setValue( PBProgreso->value() + 1 );
        // Actualizo indices y reinicio valores
        nombre_cliente = "";
        contador++;
    } // Fin for recibos


    // Pregunto si los recibos se imprimieron bien
    int ret = QMessageBox::question( this, QString::fromUtf8("Impresión"), QString::fromUtf8( "¿Se imprimieron correctamente <b>TODOS</b> los comprobantes?" ), QMessageBox::Yes, QMessageBox::No );

    if( ret == QMessageBox::No ) {
       // Si no, pregunto que numero de recibo falta y lo mando a imprimir
        ret = true;
        while( ret ) {

            // Pregunto el numero del recibo y doy opcion a cancelar cuando se acabaron
            bool ok = false;
            int ret2 = QInputDialog::getInt( this, "Numero de recibo", "Ingrese el numero del comprobante que se imprimio incorrectamente, si no hay mas comprobantes mal impresos, presione cancelar", 0, 0,2147483647, 1, &ok );
            if( ok == true ) {
                // Ingreso un recibo mal impreso, lo reimprimo
                ParameterList lista;
                if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
                    lista.append( "id_recibo", ret2 );
                } else {
                    lista.append( "id_factura", ret2 );
                }
                reporte->hacer( lista );
            } else {
                // No hay mas recibos fallados
                ret = false;
            }
        }
    }

    // Libero modelos
    delete reporte;
    delete mr;
    reporte = 0;
    mr = 0;
    // Cierro el formulario
    this->close();
    return;
}
