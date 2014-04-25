#ifndef FEEDBACKPLUGIN_H
#define FEEDBACKPLUGIN_H

class QString;
class QStackedWidget;
class QMenuBar;
#include <QObject>
#include <QtPlugin>
#include <QList>
#include <QAction>
#include "../../src/eplugin.h"
class FeedbackSender;


class FeedbackPlugin : public QObject, EPlugin
{
 Q_OBJECT
 Q_INTERFACES(EPlugin)

public:
    QList<QActionGroup *> accionesBarra();
    bool inicializar();
    QWidgetList formsPreferencias();
    QString nombre() const;
    int tipo() const;
    void crearMenu( QMenuBar *m );
    bool verificarTablas( QStringList tablas );
    double version() const;
    void crearToolBar( QToolBar *t );
    QAction *botonPantallaInicial();
    void crearAccionesGlobales(QMainWindow *){};

public slots:
    void seCierraGestotux();
    void enviarFeedback();

signals:
    void agregarVentana( QWidget *v );
    void agregarDockWidget( Qt::DockWidgetArea area, QDockWidget *ventana );

private:
    FeedbackSender *enviador;

};

#endif // FEEDBACKPLUGIN_H
