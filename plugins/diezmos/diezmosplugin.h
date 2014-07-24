#ifndef DIEZMOSPLUGIN_H
#define DIEZMOSPLUGIN_H

#include <QObject>
#include <eplugin.h>

/**
 * @brief The DiezmosPlugin class
 *
 * @author Esteban Zeller
 */
class DiezmosPlugin : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )
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
     QAction *botonPantallaInicial() { return ActDiezmos; }
     void crearAccionesGlobales(QMainWindow *){}

private:
    QAction *ActDarDiezmo;
    QAction *ActDiezmos;

protected slots:
    void darDiezmo();
    void ver_diezmos();

signals:
    void agregarVentana( QWidget *v );
    void agregarDockWidget( Qt::DockWidgetArea area, QDockWidget *ventana );

public slots:
    void seCierraGestotux();
};

#endif // DIEZMOSPLUGIN_H
