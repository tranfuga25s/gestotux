#include "ELECuitCuil.h"

ELECuitCuil::ELECuitCuil(QWidget *parent) :
    QLineEdit(parent)
{
    this->setInputMask( "99-99999999-9" );
}

/*!
 * \fn ELECuitCuil::verificar()
 * Función llamada cuando es necesario verificar que el cuit/cuil ingresado sea correcto
 * Funcion basada en http://www.taringa.net/posts/apuntes-y-monografias/2766561/Algoritmo-C_U_I_T_-_-C_U_I_L_-Argentino.html
 * \return Verdadero si el numero ingresado es correcto.
 */
bool ELECuitCuil::verificar()
{
    if( this->text().isEmpty() || this->text() == "--" )
        return true;
    QString texto = this->text();
    // Separo el codigo de verificación
    QStringList partes = texto.split( "-" );
    if( partes.count() < 2 )
        return false;
    // 1 -  primera cifra
    // 2 -  numero de doc o ref
    // 3 -  verificacion
    int verf = partes.at( 2 ).toInt();
    QString seg = partes.at( 0 );
    seg.append( partes.at( 1 ) );
    int a[10] = { 5, 4, 3, 2, 7, 6, 5, 4, 3, 2 };
    int res = 0;
    for( int i = 0; i<seg.size(); i++ ) {
        res += seg.at(i).digitValue() * a[i];
    }
    int resto = res % 11;
    if( resto == 0 ) {
        if( verf == resto ) {
            return true;
        } else {
            return false;
        }
    } else  if( resto == 1 ) {
        if( verf == 9 ) {
            return true;
        } else {
            return false;
        }
    } else  if( ( 11 - resto ) == verf ) {
        return true;
    } else {
        return false;
    }
}

QString ELECuitCuil::text() const
{
    if( QLineEdit::text() == "--" ) {
        return QString();
    } else { return QLineEdit::text(); }
}
