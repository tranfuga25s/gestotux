#ifndef FACTURA_H
#define FACTURA_H

#include "comprobanteestatico.h"

class Factura : public ComprobanteEstatico
{
    Q_OBJECT
public:

    /**
     * @brief The TipoComprobanteFactura enum
     */
    enum TipoComprobanteFactura {
        FacturaA = 11,       /** Factura Tipo A **/
        FacturaB = 12,       /** Factura Tipo B **/
        FacturaB1 = 13,      /** Factura Tipo B1 **/
        FacturaB2 = 14,      /** Factura Tipo B2 **/
        FacturaC = 15,       /** Factura tipo C **/
        FacturaX = 16,       /** Factura en negro **/
        FacturaOtro = 17,    /** Otro tipo de factura no conocido **/
        FacturaInvalida = -2 /** No es una factura o fallo a carga **/
    };

    Factura( QObject *parent = 0 );
    
};

#endif // FACTURA_H
