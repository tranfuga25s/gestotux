#include "comprobanteestatico.h"

#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QDir>

ComprobanteEstatico::ComprobanteEstatico(QObject *parent) :
    QObject(parent)
{
    _valido = false;
}

/**
 * @brief ComprobanteEstatico::cargarArchivo
 * @param archivo ruta del archivo a cargar
 * @return
 */
bool ComprobanteEstatico::cargarArchivo( const QString archivo )
{
    QDir test( QApplication::applicationDirPath() );
    QFile *arch = new QFile( test.absoluteFilePath( archivo ) );
    if( !arch->open( QIODevice::ReadOnly ) ) {
        qWarning() << "No se puede abrir el archivo especificado";
        qWarning() << "Archivo: " << QFileInfo( *arch ).absoluteFilePath();
        return false;
    }

    if( !_doc.setContent( arch ) ) {
        qWarning() << "Internpretación incorrecta del svg";
        return false;
    }
    arch->close();
    delete arch;

    // Empiezo a buscar los datos propios del comprobante
    QDomElement raiz = _doc.documentElement();

    if( !raiz.hasChildNodes() ) {
        qWarning() << "Documento raíz vacío!";
        return false;
    }

    // Busco los nodos <defs>
    QDomNodeList lista = raiz.elementsByTagName( "defs" );

    if( lista.size() <= 0 ) {
        qWarning() << "Lista de nodos en defs vacía";
        return false;
    }

    // Primer elemento debe ser el que contiene los datos
    QDomElement defs = lista.at( 0 ).toElement();

    QDomNodeList lngestotux = defs.elementsByTagName("gestotux:comprobante");
    if( lngestotux.length() <= 0 ) {
        qWarning() << "No se encontró la definición de el comprobante de gestotux";
        return false;
    }

    QDomElement egestotux = lngestotux.at(0).toElement();

    /*qDebug() << egestotux.tagName();

    QDomNamedNodeMap lista2 = egestotux.attributes();
    for( unsigned int i=0; i<lista2.length(); i++ ) {
        qDebug() << lista2.item( i ).toAttr().name();
    }*/

    // Cantidad de copias
    if( !egestotux.attributes().contains( "cantidad-copias" ) ) {
        qWarning() << "El reporte no contiene la definicion de la cantidad de copias";
        return false;
    } else {
        int copias = egestotux.attribute( "cantidad-copias" ).toInt();
        switch( copias ) {
            case Original: {
                this->_cantidad_copias_soportadas = Original;
                qDebug() << "Seteado como original";
                break;
            }
            case Duplicado: {
                this->_cantidad_copias_soportadas = Duplicado;
                qDebug() << "Seteado como duplicado";
                break;
            }
            case Triplicado: {
                this->_cantidad_copias_soportadas = Triplicado;
                qDebug() << "Seteado como triplicado";
                break;
            }
            case Cuadruplicado: {
                this->_cantidad_copias_soportadas = Cuadruplicado;
                qDebug() << "Seteado como cuadruplicado";
                break;
            }
            default: {
                this->_cantidad_copias_soportadas = Duplicado;
                qDebug() << "Numero de copias desconocida";
                break;
            }
        }

    }
    // Version del comprobante
    if( egestotux.attributes().contains( "version" ) ) {
        this->_version = egestotux.attribute( "version" );
    } else {
        this->_version = 1;
    }
    qDebug() << "Seteada version " << this->_version;
    this->_valido = true;

    // Tipo de comprobante
    // Version del comprobante
    if( !egestotux.attributes().contains( "tipo_comprobante" ) ) {
        qWarning() << "El reporte no contiene la definicion del tipo de comprobante";
        this->_tipo_comprobante = Invalido;
        this->_valido = false;
    } else {
        switch( egestotux.attribute("tipo_comprobante").toInt() ) {
            case Factura: {
                this->_tipo_comprobante = Factura;
                qDebug() << "Seteado tipo a Factura";
                break;
            }
            case Recibo: {
                this->_tipo_comprobante = Recibo;
                qDebug() << "Seteado tipo a Recibo";
                break;
            }
            case Presupuesto: {
                this->_tipo_comprobante = Presupuesto;
                qDebug() << "Seteado tipo a presupuesto";
                break;
            }
            default: {
                this->_tipo_comprobante = Invalido;
                this->_valido = false;
                break;
            }
        }
    }

    return this->_valido;
}

/**
 * @brief ComprobanteEstatico::getCantidadCopias
 * @return
 */
int ComprobanteEstatico::getCantidadCopias()
{
    if( _valido ) {
        return (int)_cantidad_copias_soportadas;
    } else {
        return 0;
    }
}

