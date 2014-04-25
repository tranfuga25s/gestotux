#ifndef ELECUITCUIL_H
#define ELECUITCUIL_H

#include <QLineEdit>

/*!
 * \brief Clase para ingreso y verificacion de CUIT/CUIL
 *
 * Esta clase permite obtener un QLineEdit que posee la máscara y el metodo para verificar de que el CUIT/CUIL ingresado son correctos. \n
 * El algoritmo utilizado para verificar el número ingresado fue obtenido de \ref http://www.taringa.net/posts/apuntes-y-monografias/2766561/Algoritmo-C_U_I_T_-_-C_U_I_L_-Argentino.html. \n
 * \author Esteban Zeller
 */
class ELECuitCuil : public QLineEdit
{
    Q_OBJECT
public:
    explicit ELECuitCuil(QWidget *parent = 0);
    bool verificar();
    QString text() const;

};

#endif // ELECUITCUIL_H
