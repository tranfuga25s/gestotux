#ifndef FORMMODIFICARPRESUPUESTO_H
#define FORMMODIFICARPRESUPUESTO_H

#include "eventana.h"
#include "ui_FormPresupuestoBase.h"
class QSqlQueryModel;
class MProductosTotales;
class QDataWidgetMapper;

class FormModificarPresupuesto : public EVentana, public Ui::FormPresupuestoBase
{
    Q_OBJECT
public:
    explicit FormModificarPresupuesto(QWidget *parent = 0);
    void setearIdPresupuesto( QModelIndex idx );

protected slots:
    void cancelar();
    void guardar( bool cerrar );
    void guardar();
    void agregarProducto();
    void eliminarProducto();
    void borrarTodoProducto();
    void cambioCliente( int id_combo );

private:
    int _id_presupuesto;
   /*!
    * Modelo para mostrar la lista de productos e items
    */
    MProductosTotales *m;
   /*!
    * Mapa para los datos cargados
    */
    QDataWidgetMapper *mapa;

};

#endif // FORMMODIFICARPRESUPUESTO_H
