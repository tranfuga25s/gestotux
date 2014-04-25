#ifndef BSCOMPUTACION_H
#define BSCOMPUTACION_H


#include <QObject>
#include "eplugin.h"
#include "einfoprogramainterface.h"
#include <QtPlugin>
#include <QAction>

/*!
 * \brief Plugin de cliente para BSComputacion
 *
 * Clase que posee los datos necesarios para configurar el programa para BSComputacion
 * \author Esteban Zeller <tranfuga25s@gmail.com>
 */
class BSComputacion : public QObject, public EPlugin, public EInfoProgramaInterface
{
    Q_OBJECT
    Q_INTERFACES( EPlugin EInfoProgramaInterface )
public:
    bool inicializar();
    bool verificarTablas( QStringList tablas );
    double version() const;
    int tipo() const;
    QList< QActionGroup * > accionesBarra();
    QString nombre() const;
    QWidgetList formsPreferencias();
    void crearMenu(QMenuBar* m);
    void crearToolBar(QToolBar* t);
    QAction *botonPantallaInicial() { return new QAction( this ); }
    QString nombrePrograma() const;
    QIcon iconoPrograma() const;
    QString directorioBackup() const;
    QString directorioActualizaciones() const;
    QString empresa() const;
    QString companeros();
    bool publicidad();
    QString reporte( int tipo );
    QImage imagenPrograma() const;
    void reporteParametros(int tipo, QString &nombre, ParameterList &parametros);
    void crearAccionesGlobales(QMainWindow *) {}

signals:
    void agregarVentana(QWidget* v);
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);

public slots:
    void seCierraGestotux();

};

#endif // BSCOMPUTACION_H
