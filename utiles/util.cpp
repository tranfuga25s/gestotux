#include "util.h"

#include <QString>
#include <QSqlDatabase>
#include <QDate>
#include <QDateTime>

QString Util::comparacionSQL( const QString texto )
{
    QString ret( texto );
    ret = ret.toUpper();
    ret = ret.trimmed();
    ret.replace( " ", "" );
    return ret;
}

QString Util::funcionComparacionSQL( const QString nombre_campo )
{

    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {
        // Devuelvo el grupo de funciones anidadas que cumplan con la función idem a la anterior

        return QString( "upper( trim( replace( %1, ' ', '' ) ) )" ).arg( nombre_campo );

    } else if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {

        return QString( "UPPER( TRIM( REPLACE( %1, ' ', '' ) ) )" ).arg( nombre_campo );

    } else {
        qDebug( "Driver desconocido" );
        return nombre_campo;
    }
}

/*!
 * \brief Util::formateoFechas
 *  Devuelve el formato correcto para fechas sql en comparaciones sql según la base de datos
 * \param fecha Fecha a convertir
 * \return String con el formateo correcto
 * \return
 */
QString Util::formateoFechas( QDate fecha ) {
    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {

        return fecha.toString( Qt::ISODate );

    } else if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {
        /// @TODO: Verificar si esto se cumple o hay que agregar un DATE()
        return fecha.toString( Qt::ISODate );

    } else {
        qDebug( "Driver desconocido" );
        return fecha.toString( Qt::ISODate );
    }
}

/*!
 * \brief Util::formateoFechas
 *  Devuelve el formato correcto para fechas sql en comparaciones sql según la base de datos
 * \param fecha Fecha a convertir
 * \return String con el formateo correcto
 */
QString Util::formateoFechas( QDateTime fecha )
{
    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {

        return fecha.toString( Qt::ISODate );

    } else if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {
        /// @TODO: Verificar si esto se cumple o hay que agregar un DATE()
        return fecha.toString( Qt::ISODate );

    } else {
        qDebug( "Driver desconocido" );
        return fecha.toString( Qt::ISODate );
    }
}

/*!
 * \brief Util::importeATexto
 * \param importe Importe a pasar a texto
 * \return
 */
QString Util::importeATexto( double x )
{
    QString salida;
    if( x > 1000000000.0 ) {
        if( x > 2000000000.0 ) {
            salida.append( numeroATexto( x/1000000000.0 ) ).append( " BILLONES " );
            x-=( (int) (x/1000000000) )*1000000000.0;
        } else {
            salida.append( numeroATexto( x/1000000000.0 ) ).append( " BILLON " );
            x-=1000000000.0;
        }

    }
    if( x > 1000000.0 ) {
        if( x >= 2000000.0 ) {
            salida.append( numeroATexto( x/1000000.0 ) ).append( " MILLONES " );
            x-=( (int)(x/1000000.0))*1000000.0;
        } else {
            salida.append( numeroATexto( x/1000000.0 ) ).append( " MILLON " );
            x-=1000000.0;
        }

    }
    salida.append( numeroATexto( x ) );
    // Parte de los centavos
    double temp = (int)x;
    x-=temp;
    x*=100;
    if( x > 0 ) {
        salida.append( " CON " ).append( numeroATexto( x ) ).append( " CENTAVOS " );
    } else {
        salida.append( " CON CERO CENTAVOS " );
    }
    return salida;
}

QString Util::numeroATexto( double x ) {
    QString ret;
        if(x>=900.0)	{ret.append( "NOVECIENTOS " )	;x-=900.0;}
   else if(x>=800.0)	{ret.append( "OCHOCIENTOS " )	;x-=800.0;}
   else if(x>=700.0)	{ret.append( "SETECIENTOS " )	;x-=700.0;}
   else if(x>=600.0)	{ret.append( "SEISCIENTOS " )	;x-=600.0;}
   else if(x>=500.0)	{ret.append( "QUINIENTOS "  )	;x-=500.0;}
   else if(x>=400.0)	{ret.append( "CUATROCIENTOS " )	;x-=400.0;}
   else if(x>=300.0)	{ret.append( "TRESCIENTOS " )	;x-=300.0;}
   else if(x>=200.0)	{ret.append( "DOSCIENTOS ")	;x-=200.0;}
   else if(x>100.0)	{ret.append( "CIENTO " )	;x-=100.0;}
   else if(x==100.0)	{ret.append( "CIEN" )		;x-=100.0;}
        if(x>90.0)	{ret.append( "NOVENTA Y " )	;x-=90.0; }
        if(x==90.0)	{ret.append( "NOVENTA" )	;x-=90.0; }
        if(x>80.0)	{ret.append( "OCHENTA Y " )	;x-=80.0; }
        if(x==80.0)	{ret.append( "OCHENTA" )	;x-=80.0; }
        if(x>70.0)	{ret.append( "SETENTA Y " )	;x-=70.0; }
        if(x==70.0)	{ret.append( "SETENTA" )	;x-=70.0; }
        if(x>60.0)	{ret.append( "SESENTA Y " )	;x-=60.0; }
        if(x==60.0)	{ret.append( "SESENTA" )	;x-=60.0; }
        if(x>50.0)	{ret.append( "CINCUENTA Y " )	;x-=50.0; }
        if(x==50.0)	{ret.append( "CINCUENTA " )	;x-=50.0; }
        if(x>40.0)	{ret.append( "CUARENTA Y " )	;x-=40.0; }
        if(x==40.0)	{ret.append( "CUARENTA" )	;x-=40.0; }
        if(x>30.0)	{ret.append( "TREINTA Y " )	;x-=30.0; }
        if(x==30.0)	{ret.append( "TREINTA" )	;x-=30.0; }
        if(x>20.0)	{ret.append( "VEINTI" )		;x-=20.0; }
        if(x==20.0)	{ret.append( "VEINTE" )		;x-=20.0; }
        if(x>=16.0)	{ret.append( "DIECI" )		;x-=10.0; }
   else if(x==15.0)	{ret.append( "QUINCE" )		;x-=15.0; }
   else if(x==14.0)	{ret.append( "CATORCE" )	;x-=14.0; }
   else if(x==13.0)	{ret.append( "TRECE" )		;x-=13.0; }
   else if(x==12.0)	{ret.append( "DOCE" )		;x-=12.0; }
   else if(x==11.0)	{ret.append( "ONCE" )		;x-=11.0; }
   else if(x==10.0)	{ret.append( "DIEZ" )		;x-=10.0; }
        if(x==9.0)	{ret.append( "NUEVE" )		;x-=9.0;  }
        if(x==8.0)	{ret.append( "OCHO" )		;x-=8.0;  }
        if(x==7.0)	{ret.append( "SIETE" )		;x-=7.0;  }
        if(x==6.0)	{ret.append( "SEIS" )		;x-=6.0;  }
   else if(x==5.0)	{ret.append( "CINCO" )		;x-=5.0;  }
   else if(x==4.0)	{ret.append( "CUATRO" )		;x-=4.0;  }
   else if(x==3.0)	{ret.append( "TRES" )		;x-=3.0;  }
   else if(x==2.0)	{ret.append( "DOS" )		;x-=2.0;  }
   else if(x==1.0)	{ret.append( "UNO" )		;x-=1.0;  }
   return ret;
}
