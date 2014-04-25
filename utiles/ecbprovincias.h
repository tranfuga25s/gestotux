#ifndef ECBPROVINCIAS_H
#define ECBPROVINCIAS_H

#include <QComboBox>
#include "ecbpaises.h"

/*!
 * \brief Clase para generar un combobox que cargue provincias
 *
 * Esta clase le permite obtener un combo box con el listado de clientes segun el filtro que poseea activo y le permite mappear id_cliente <-> combo directamente. \n
 * Al cargarse la clase lanza una señal que permite la carga de datos en diferido. Mientras el sistema este cargando los datos, no se podrá realizar acciones con el combobox y aparecera el texto de "Cargando datos...". \n
 * Una vez finalizada la carga se habilitará para su uso normal. \n
 * Para obtener el ID de base de datos del cliente que este seleccionado se utiliza el metodo \ref idClienteActual() , y se puede colocar como seleccionado un cliente mediante el método \ref setearId(). \n
 * La carga de los datos se hará respetando las clausulas de el filtro que exista. De manera predeterminada el filtro esta vacío, pero puede ser cambiado utilizando el método \ref setearFiltro(). \n
 * Cada vez que se cambie el filtro se disparará una recarga de los datos que contiene el combobox.
 */
class ECBProvincias : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY( int id_provincia READ idActual WRITE setearId USER true )
public:
    ECBProvincias(QWidget *parent = 0);
    ~ECBProvincias();
    void setearFiltro( const QString f );
    int idActual();
    void setearCBPais( ECBPaises *cb );
    void setearId( const int id_cliente );
    void setearPais( const int id_pais );

private slots:
    void verificarExiste();
    void inicializar();
    void cambioPais( int pos );

private:
   /*!
    * Guarda el ID de cliente segun la posicion del cb
    */
    QList<int> *ids;
   /*!
    * Filtro para la consulta de clientes
    */
    QString filtro;
   /*!
    * Puntero al cb de paises
    */
    ECBPaises *cbpaises;
   /*!
    * Identificador para retrasados
    */
    int _id_provincia;
    bool _inicializado;
};

#endif // ECBPROVINCIAS_H
