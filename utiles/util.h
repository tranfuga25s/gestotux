#ifndef UTIL_H
#define UTIL_H

class QString;
class QDate;
class QDateTime;
/*!
 * \brief The Util class
 * Colección de métodos estaticos varios que no requieren una clase específica.
 * \author Esteban Zeller
 */
class Util
{
public:
    static QString comparacionSQL(const QString texto );
    static QString funcionComparacionSQL( const QString nombre_campo );
    static QString formateoFechas( QDate fecha );
    static QString formateoFechas( QDateTime fecha );
    static QString importeATexto( double importe );
    static QString numeroATexto( double importe );

};

#endif // UTIL_H
