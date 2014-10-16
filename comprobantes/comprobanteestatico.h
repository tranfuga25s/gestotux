#ifndef COMPROBANTEESTATICO_H
#define COMPROBANTEESTATICO_H

#include <QObject>

#include <QDomDocument>
#include <QDate>
#include <QSvgRenderer>

/**
 * @brief The ComprobanteEstatico class
 *
 * Contiene la información necesaria sobre un formulario estatico concreto
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class ComprobanteEstatico : public QObject
{
    Q_OBJECT
public:

    enum Copias {
        Original = 0,
        Duplicado = 1,
        Triplicado = 2,
        Cuadruplicado = 3,
        Quintuplicado = 4
    };

    ComprobanteEstatico( QObject *parent = 0 );

    bool cargarArchivo( const QString archivo );

    bool esValido() const { return _valido; }

    int getCantidadCopias();
    void setearCantidadCopias( Copias cantidad ) { this->_cantidad_copias_soportadas = cantidad; }

    QString version() { return this->_version; }

private:
    QString file;
    QDomDocument _doc;
    QSvgRenderer _renderizador;

    bool _valido;

    QString _name;
    QString _version;
    QDateTime _ultima_modificacion;

    Copias _cantidad_copias_soportadas;
    
};

#endif // COMPROBANTEESTATICO_H
