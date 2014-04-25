#include "egarantiasvg.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPaintEngine>
#include <QVariant>
#include <QDebug>
#include <QSvgRenderer>
#include <QSqlError>
#include <QSqlField>
#include <QDate>
#include <QTextStream>
#include <QPrinter>

#include <QApplication>
#include <QDir>

EGarantiaSVG::EGarantiaSVG(QObject *parent) :
    QObject(parent)
{
    _nombre_archivo = QString();
    _valido = false;

    _mapa["FechaOriginalDia"] = "dia";
    _mapa["FechaOriginalMes"] = "mes";
    _mapa["FechaOriginalAno"] = "ano";
    _mapa["NumeroGarantiaOriginal"] = "numero_garantia";
    _mapa["ClienteOriginal"] = "razon_social";
    _mapa["NombreProductoOriginal"] = "nombre_producto";
    _mapa["NumeroSerieOriginal"] = "numero_serie";
    _mapa["NumeroComprobanteOriginal"] = "numero_comprobante";
    _mapa["FechaFinOriginal"] = "fecha_fin";
    _mapa["FechaInicioOriginal"] = "fecha_inicio";

    _mapa["FechaDuplicadoDia"] = "dia";
    _mapa["FechaDuplicadoMes"] = "mes";
    _mapa["FechaDuplicadoAno"] = "ano";
    _mapa["NumeroGarantiaDuplicado"] = "numero_garantia";
    _mapa["ClienteDuplicado"] = "razon_social";
    _mapa["NombreProductoDuplicado"] = "nombre_producto";
    _mapa["NumeroSerieDuplicado"] = "numero_serie";
    _mapa["NumeroComprobanteDuplicado"] = "numero_comprobante";
    _mapa["FechaFinDuplicado"] = "fecha_fin";
    _mapa["FechaInicioDuplicado"] = "fecha_inicio";

}

/*!
 * \brief EGarantiaSVG::setearNombreArchivo
 * \param nombre
 */
bool EGarantiaSVG::setearNombreArchivo( const QString nombre )
{
    if( !QFile::exists( nombre ) ) {
        qCritical() << "No se pudo encontrar el archivo " << nombre;
        return false;
    }
    this->_nombre_archivo = nombre;
    return true;
}

/*!
 * \brief EGarantiaSVG::agregarMapeo
 * \param nombre_svg
 * \param nombre_registro
 */
void EGarantiaSVG::agregarMapeo(QString nombre_svg, QString nombre_registro)
{
    if( _registro.isEmpty() ) {
        qDebug() << "El registro no está seteado!";
        return;
    }

    if( _registro.contains( nombre_registro ) ) {
        _mapa.insert( nombre_svg, nombre_registro );
    } else {
        qDebug() << "El registro " << nombre_registro << " no existe en el registro utilizado";
    }
}

/*!
 * \brief EGarantiaSVG::imprimir
 * \param impresora
 */
void EGarantiaSVG::imprimir( QPainter *impresora )
{
    if( _valido ) {
        QSvgRenderer rend( this->obtenerByteArray() );
        rend.render( impresora );
    } else {
        qDebug() << "Informe no valido!";
    }
}

/*!
 * \brief EGarantiaSVG::imprimir
 * \param impresora
 */
void EGarantiaSVG::imprimir( QPrinter *impresora )
{
    if( _valido ) {
        QSvgRenderer rend( this->obtenerByteArray() );

        QPainter pintor;
        pintor.begin( impresora );
        rend.render( &pintor );
        pintor.end();

    } else {
        qDebug() << "Informe no valido!";
    }
}

/*!
 * \brief EGarantiaSVG::obtenerByteArray
 * \return
 */
QByteArray EGarantiaSVG::obtenerByteArray()
{
    return( _domdoc.toByteArray() );
}

/*!
 * \brief EGarantiaSVG::hacerText
 * \param texto Texto a analizar
 * \param padre Padre al cual se le agregará
 * \param ancho Ancho maximo permitido
 * \param x Posicion X
 * \param y Posicion Y
 */
void EGarantiaSVG::hacerText( QString texto, QDomNode padre, const double ancho, double x, double y )
{
    int contador = 0;
    double total = 0;
    double posx = x;
    double posy = y;
    QString descripcion2 = texto;
    while( contador < descripcion2.count() )
    {
     // Busco si el caracter no es un fin de linea
     if( descripcion2.at( contador ) == '\n' )
     {
      //fuerzo el salto de linea
      contador++;
      total = ancho+1;
     }
     // sino sumo el ancho de ese caracter
     if( total < ancho )
     {
       total += anchoCaracter( descripcion2.at( contador ) );
       {
        contador++;
       }
     }
     else
     {
       QDomNode nodo = _domdoc.createElement( "text" );
       nodo.toElement().setAttribute( "x", posx );
       nodo.toElement().setAttribute( "y", posy );
       posy += 4.5;
       nodo.toElement().setAttribute( "font-size", "4" );
       nodo.toElement().setAttribute( "font-family", "Verdana" );
       nodo.toElement().setAttribute( "fill", "black" );
       // Veo hasta donde llege
       QString texto = descripcion2.left( contador );
       descripcion2.remove( descripcion2.left( contador ) );
       nodo.appendChild( _domdoc.createTextNode( texto ) );
       padre.appendChild( nodo );
       total = 0;
       contador = 0;
     }
    }
    if( total < ancho )
    {
     QDomNode nodo = _domdoc.createElement( "text" );
       nodo.toElement().setAttribute( "x", posx );
       nodo.toElement().setAttribute( "y", posy );
       posy += 4.5;
       nodo.toElement().setAttribute( "font-size", "4" );
       nodo.toElement().setAttribute( "font-family", "Verdana" );
       nodo.toElement().setAttribute( "fill", "black" );
       // Veo hasta donde llege
       QString texto = descripcion2.left( contador );
       descripcion2.remove( descripcion2.left( contador ) );
       nodo.appendChild( _domdoc.createTextNode( texto ) );
       padre.appendChild( nodo );
    }
    // ver si me queda algo que no se haya completado
}

/*!
 * \brief EGarantiaSVG::anchoCaracter
 * \param car
 * \return
 */
double EGarantiaSVG::anchoCaracter(QString car)
{
    // Si es mayusculas
     if( car.contains( QRegExp( "[A-Z]", Qt::CaseSensitive, QRegExp::RegExp ) ) )
     {
      return 4.3;
     }
     // Si es minusculas
     if( car.contains( QRegExp( "[a-z]", Qt::CaseSensitive, QRegExp::RegExp ) ) )
     {
      return 2.10;
     }
     // si es !
     if( car ==  "!" )
     {
      return 1.45;
     }
     // si es .
     if( car == "." )
     {
      return 1.28;
     }
     // si es espacio
     return 1.58;
}

/*!
 * \brief EGarantiaSVG::cargarDatos
 */
void EGarantiaSVG::cargarDatos()
{
    if( _valido )
        return; // Ya está cargado

    if( _nombre_archivo.isEmpty() || _nombre_archivo.isNull() ) {
        qDebug() << "Entra al predeterminado";
        QDir dir;
        dir.setPath( QApplication::applicationDirPath() );
        if( !dir.cd( "reportes" ) ) {
            qWarning( "NO se puede ingresar al directorio reportes" );
            return;
        }
        if( !setearNombreArchivo( dir.absoluteFilePath( "garantia.svg" ) ) ) {
            qWarning() << "NO se pudo encontrar el archivo garantia.svg en " << dir.absolutePath();
            return;
        }
        qDebug() << dir.absoluteFilePath( "garantia.svg" );
    }
    if( _registro.count() == 0 ) {
        qDebug() << "No se seteo ningún registro para extraer los datos";
        return;
    }

    QFile archivo( _nombre_archivo );
    if( !archivo.open( QIODevice::ReadOnly ) )
    {
      qCritical() << "No se puede encontrar el archivo original de recibo: "<<_nombre_archivo;
      return;
    }
    if( !_domdoc.setContent( &archivo ) )
    {
      qWarning() << "Error al pasar el contenido desde el archivo al documento dom";
      return;
    }
    archivo.close();

    // realizo los reemplazos necesarios
    QDomNodeList lista = _domdoc.elementsByTagName( "svg:text" );
     if( lista.isEmpty() )
     {
      qWarning() << "Error: no existen nodos de texto en el xml!";
      return;
     }

     for( int i = 0; i< lista.size(); i++ )
     {
       QString id = lista.item(i).toElement().attribute( "id" );

       if( _mapa.contains( id ) ) {

           // Veo que registro es
           QString nombre_registro = _mapa.value( id );
           if( !_registro.contains( nombre_registro ) ) {
               qWarning() << "El registro " << nombre_registro << " no existe!";
               return;
           }

           QVariant valor = _registro.value( nombre_registro );
           QString texto = QString();

           switch( _registro.field( nombre_registro ).type() ) {
               case QVariant::Bool:
               {
                   if( valor.toBool() ) {
                       texto = "Si";
                   } else { texto = "No"; }
                   break;
               }
               case QVariant::Date:
               case QVariant::DateTime:
               {
                   texto = valor.toDate().toString( Qt::SystemLocaleShortDate );
                   break;
               }
               /*case QVariant::DateTime:
               {
                   texto = valor.toDateTime().toString( Qt::SystemLocaleDate );
                   break;
               }*/
               case QVariant::Double:
               {
                   texto = QString( "%L1" ).arg( valor.toDouble(), 8, 'f', 2, QLatin1Char(' ') );
                   break;
               }
               default:
               {
                   texto = valor.toString();
                   break;
               }
           }
           if( id == "NumeroGarantiaOriginal" || id == "NumeroGarantiaDuplicado" ) {
               texto = generarNumeroGarantia( valor.toInt() );
           }
           if( id == "FechaOriginalAno" || id == "FechaDuplicadoAno" ) {
               texto = texto.remove( "20" );
           }
           texto = texto.trimmed();

           // Lo coloco en el elemento
           QDomNode nodo = lista.item(i);
           if( nodo.firstChild().isText() ) {
               nodo = nodo.firstChild();
           } else {
               nodo = nodo.firstChild().firstChild();
           }
           nodo.toText().setData( texto );
           qDebug() << "Reemplazado " << id << " con datos de " << nombre_registro << " con " << texto;
       }
     }

    _valido = true;

    /*QFile arch( QApplication::applicationDirPath() + QDir::separator() + "salida.svg" );
     arch.open( QIODevice::WriteOnly );
     QTextStream s( &arch );
     s << _domdoc.toByteArray();
     arch.flush();
     arch.close();*/

}

/*!
 * \brief EGarantiaSVG::generarNumeroGarantia
 * \param num_recibo
 * \return
 */
QString EGarantiaSVG::generarNumeroGarantia( int num_recibo )
{
  QString t1 = QString::number( num_recibo );
  if( t1.length() < 6 )
  {
   for( int i = t1.length(); i<=6; i++ )
   {
    t1.prepend( "0" );
   }
   return t1;
  }
  else
  {
   for( int i = t1.length(); i<=10; i++ )
   {
    t1.prepend( "0" );
   }
   t1.insert( 4, '-' );
   return t1;
  }
}
