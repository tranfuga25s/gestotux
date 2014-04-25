#ifndef TRSISTEMAS_H
#define TRSISTEMAS_H


#include <QObject>
#include "eplugin.h"
#include "einfoprogramainterface.h"
#include <QtPlugin>
#include <QAction>

/*!
 * \brief Plugin de cliente para TR Sistemas Informaticos Integrados
 *
 * Clase que posee los datos necesarios para configurar el programa para TR Sistemas Informaticos Integrados
 * \author Esteban Zeller <tranfuga25s@gmail.com>
 */
class TRSistemas : public QObject, public EPlugin, public EInfoProgramaInterface
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
    void reporteParametros( int tipo, QString &nombre, ParameterList &parametros );
    QImage imagenPrograma() const;
    void crearAccionesGlobales(QMainWindow *){}

signals:
    void agregarVentana(QWidget* v);
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);

public slots:
    void seCierraGestotux();

};

#endif // TRSISTEMAS_H
