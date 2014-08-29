#ifndef UNIDADESPRODUCTOSPLUGIN_H
#define UNIDADESPRODUCTOSPLUGIN_H

#include <QAction>
#include <QObject>
#include <eplugin.h>

/**
 * @brief The UnidadesProductosPlugin class
 *
 * Permite mantener unidades de productos especificos para mejorar la calidad de el ingreso de estas.
 * @author Esteban Zeller <esteban.zeller@gmail.com>
 */
class UnidadesProductosPlugin : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )

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
    QAction *botonPantallaInicial() { return new QAction( 0 ); }
    void crearAccionesGlobales(QMainWindow *){}

signals:
    void agregarVentana( QWidget * );
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);

public slots:
    void seCierraGestotux();

};

#endif // UNIDADESPRODUCTOSPLUGIN_H
