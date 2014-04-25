#ifndef ETEXTBROWSER_H
#define ETEXTBROWSER_H

#include <QTextBrowser>
#include <QHelpEngine>

/*!
 * \brief Clase utilizada para la ayuda.
 *
 * Clase que reimplementa un metodo para que sepa utilizar el sistema de ayuda.
 */
class ETextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    ETextBrowser( QWidget *parent = 0 );
    QVariant loadResource( int type, const QUrl &url );
    void setHelpEngine( QHelpEngine *he );

private:
    QHelpEngine *_he;
};

#endif // ETEXTBROWSER_H
