#ifndef CUOTASPLUGIN_H
#define CUOTASPLUGIN_H

#include <QObject>
#include <eplugin.h>
#include <QtPlugin>
#include "mplancuota.h"

/**
 * \brief Plugin de Cuotas
 * Sistema de control para ventas en formato de cuotas.
 * Se emiten recibos por cada elemento vendido por medio de cuotas.
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class CuotasPlugin : public QObject, public EPlugin
{
    Q_OBJECT
    Q_INTERFACES(EPlugin)

    public:
        QList<QActionGroup *> accionesBarra();
        QString nombre() const;
        QWidgetList formsPreferencias();
        bool inicializar();
        bool verificarTablas( QStringList tablas );
        int tipo() const;
        void crearMenu( QMenuBar *m );
        double version() const;
        static QStackedWidget *tabs();
        void crearToolBar( QToolBar *t );
        bool publicidad() { return true; }
        QAction *botonPantallaInicial() { return ActVerCuotas; }
        void crearAccionesGlobales(QMainWindow *){}

    public slots:
        void seCierraGestotux();
        void generarPlanCuotas( int id_cliente, double total, MPlanCuota::TipoComprobante tipo );
        void planCuotasSetearIdFactura( int id_plan_cuota, int id_factura);

    signals:
        void agregarVentana( QWidget * );
        void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);
        void emitirPlanCuotaId( int id_plan_cuota );

    private slots:
        void verCuotas();
        void generarComprobantes();
        void simularCuotas();
        void resumenCuotas();

    private:
        QAction *ActVerCuotas;
        QAction *ActGenerarComprobantes;
        QAction *ActSimular;
        QAction *ActResumenCuotas;
        QAction *ActSep;

};

#endif // CUOTASPLUGIN_H
