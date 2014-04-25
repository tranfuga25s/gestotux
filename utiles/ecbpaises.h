#ifndef ECBPAISES_H
#define ECBPAISES_H

#include <QComboBox>
#include <QCompleter>
#include <QLineEdit>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

/*!
 * \brief Clase para generar un combobox de selector de paises
 *
 * Esta clase le permite obtener un combo box con el listado de paises segun el filtro que poseea activo y le permite mappear id_paises <-> combo directamente. \n
 * Al cargarse la clase lanza una señal que permite la carga de datos en diferido. Mientras el sistema este cargando los datos, no se podrá realizar acciones con el combobox y aparecera el texto de "Cargando datos...". \n
 * Una vez finalizada la carga se habilitará para su uso normal. \n
 * Para obtener el ID de base de datos del pais que este seleccionado se utiliza el metodo \ref idActual() , y se puede colocar como seleccionado un pais mediante el método \ref setearId(). \n
 * La carga de los datos se hará respetando las clausulas de el filtro que exista. De manera predeterminada el filtro esta vacío, pero puede ser cambiado utilizando el método \ref setearFiltro(). \n
 * Cada vez que se cambie el filtro se disparará una recarga de los datos que contiene el combobox.
 **/
class ECBPaises : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY( int id_pais READ idActual WRITE setearId USER true )
public:
    explicit ECBPaises(QWidget *parent = 0);
    ~ECBPaises();
    void setearFiltro( const QString f );
    int idActual();
    void setearId( int id_pais );

private slots:
    void verificarExiste();
    void inicializar();

private:
   /*!
    * Guarda el ID de cliente segun la posicion del cb
    */
    QList<int> *ids;
   /*!
    * Filtro para la consulta de clientes
    */
    QString filtro;

    int _id_pais;

    bool _inicializado;
    
};

#endif // ECBPAISES_H
