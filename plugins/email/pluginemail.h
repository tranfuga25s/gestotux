#ifndef PLUGINEMAIL_H
#define PLUGINEMAIL_H

#include <QObject>
#include "eplugin.h"
#include "einterfazemail.h"
#include <QLabel>

class PluginEmail : public QObject, public EPlugin, public EInterfazEmail
    {
        Q_OBJECT
        Q_INTERFACES( EPlugin EInterfazEmail )

        public:
            bool inicializar();
            bool verificarTablas( QStringList lista );
            double version() const;
            int tipo() const;
            QList< QActionGroup * > accionesBarra();
            QString nombre() const;
            QWidgetList formsPreferencias();
            void crearMenu(QMenuBar* m);
            void crearToolBar(QToolBar* t);
            QWidget * statusBarWidget();
            QAction *botonPantallaInicial();
            void crearAccionesGlobales(QMainWindow *);

        signals:
            void agregarVentana(QWidget* v);
            void agregarDockWidget( Qt::DockWidgetArea area, QDockWidget *widget );

        public slots:
            void seCierraGestotux();

        private:
            /*!
             * Etiqueta de la barra de estado
             */
            QLabel *_etiqueta;
           /*!
            * Enviador de emails.
            */
            /*Smtp *enviador;*/

        protected slots:
            void cambioEstado( const QString &texto );

};

#endif // PLUGINEMAIL_H
