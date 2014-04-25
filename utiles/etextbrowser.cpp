#include "etextbrowser.h"
#include <QHelpIndexModel>

ETextBrowser::ETextBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    _he = 0;
}

void ETextBrowser::setHelpEngine( QHelpEngine *he ) {
    _he = he;
    /*QUrl t = he->indexModel()->linksForKeyword( "*" ).constBegin().value().toString();
    QString _pre = "qthelp://" + he->currentFilter() + "/ "+ t.path();
    qWarning( _pre.toLocal8Bit() );*/
}

QVariant ETextBrowser::loadResource( int /*type*/, const QUrl &url )
{
    if( url.toString().contains( "http" ) || url.toString().contains( "ftp" ) ) {
        return QVariant( "" );
    } else {
        if( _he != 0 ) {
            /// @TODO: Hacer que tome los datos correctamente... no esta basofia.
            return QVariant( _he->fileData( "qthelp://tranfuga.gestotux.0.5/doc/" + url.toString() ) );
        } else {
            qWarning( "Please set Help Engine pointer first" );
            return QVariant( "" );
        }
    }
}
