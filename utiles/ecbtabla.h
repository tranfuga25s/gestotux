#ifndef ECBTABLA_H
#define ECBTABLA_H

#include <QComboBox>

/*!
 * \brief Combobox con mappeo item <-> id de la DB
 *
 * Esta clase le permite obtener un combo box con el listado de clientes segun el filtro que poseea activo y le permite mappear id_cliente <-> combo directamente. \n
 * Al cargarse la clase lanza una señal que permite la carga de datos en diferido. Mientras el sistema este cargando los datos, no se podrá realizar acciones con el combobox y aparecera el texto de "Cargando datos...". \n
 * Una vez finalizada la carga se habilitará para su uso normal. \n
 * Para obtener el ID de base de datos del cliente que este seleccionado se utiliza el metodo \ref idClienteActual() , y se puede colocar como seleccionado un cliente mediante el método \ref setearId(). \n
 * La carga de los datos se hará respetando las clausulas de el filtro que exista. De manera predeterminada el filtro esta vacío, pero puede ser cambiado utilizando el método \ref setearFiltro(). \n
 * Cada vez que se cambie el filtro se disparará una recarga de los datos que contiene el combobox.
 * La señal cambioIdCliente( int id_cliente ) será emitida cada vez que se seleccione un cliente distinto.
 * \author Esteban Zeller
 */
class ECBTabla : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY( int id READ idActual WRITE setearId USER true )
public:
    ECBTabla( QWidget *parent = 0 );
    ECBTabla( QWidget *parent, QString tabla, QString tid, QString texto, QString orden = QString() );
    ~ECBTabla();

    void setearFiltro( const QString f, const bool inmmediate = false );
    void setearTexto( const QString t );
    void setearId( const int id );
    void setearTabla( QString tabla );
    void setearCampoId( QString nombre );
    void setearCampoTexto( QString nombre );
    void setearCampoOrden( QString texto );
    void setearCampoBusquedaExtra( QString campo );

    int idActual();

    /*!
     * \brief getListaIDs
     *  Función auxiliar para obtener los IDs cargados en el combobox
     * \return QList<int> * Con la lista de IDs en el combo box
     */
    QList<int> *getListaIDs() { return ids; }



signals:
    void cambioId( int id );
    void sinDatos();

private slots:
    void verificarExiste();
    void inicializar();
    void cambioIdSeleccionado( int ) { emit cambioId( idActual() ); }

private:
   /*!
    * Guarda el ID de cliente segun la posicion del cb
    */
    QList<int> *ids;
   /*!
    * Guarda el codigo extra para las busquedas
    */
    QList<int> *codigos;
   /*!
    * Filtro para la consulta
    */
    QString filtro;
   /*!
    * Id seleccionado
    */
    int _id;
   /*!
    *
    */
    QString _texto;
    /*!
     * Guarda si el modelo está inicializado o no
     */
    bool _inicializado;
    /*!
     * Nombre de la tabla o vista a utilizar
     */
    QString _tabla;
    /*!
     * Texto sql para el identificador del campo
     */
    QString _campo_id;
    /*!
     * Texto sql para el texto que aparecerá en el combobox
     */
    QString _campo_texto;
    /*!
     * Texto sql para el ORDER BY
     */
    QString _campo_orden;
    /*!
     * Texto sql para el campo de busqueda extra
     */
    QString _campo_busqueda;
    /*!
     * Indica si la busqueda por campo extra está habilitada
     */
    bool _busqueda;

};

#endif // ECBTABLA_H
