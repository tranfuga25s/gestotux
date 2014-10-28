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
        Original = 1,
        Duplicado = 2,
        Triplicado = 3,
        Cuadruplicado = 4,
        Quintuplicado = 5
    };

    /**
     * @brief The TipoComprobante enum
     */
    enum TipoComprobante {
        Factura = 6,     /** Tipo Factura **/
        Recibo = 7,      /** Tipo Recibo **/
        Remito = 8,      /** Tipo Remito **/
        Presupuesto = 9, /** Tipo presupuesto **/
        Otro = 10,       /** Otro tipo de comprobante **/
        Invalido = -1    /** Tipo de comprobante invalido **/
    };



    /**
     * @brief The TipoComprobanteRecibo enum
     */
    enum TipoComprobanteRecibo {
        ReciboR = 18, /** Recibo normal **/
        ReciboX = 19, /** Recibo en negro **/
        ReciboOtro = 20, /** Recibo no definido **/
        ReciboInvalido = -3 /** No es un recibo o fallo la carga **/
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
    TipoComprobante _tipo_comprobante;
    
};

#endif // COMPROBANTEESTATICO_H
