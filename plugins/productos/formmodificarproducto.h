#ifndef FORMMODIFICARPRODUCTO_H
#define FORMMODIFICARPRODUCTO_H

#include "ui_formproductobase.h"
#include <QDialog>
#include <QDockWidget>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include "mproductos.h"

/*!
 * \brief Formulario para modificar un producto
 *
 *
 * \author Esteban Zeller
 */
class FormModificarProducto : public QDialog, public Ui::FormProductoBase
{
    Q_OBJECT
public:
    explicit FormModificarProducto( MProductos *mod, QWidget *parent = 0);
    void setearProducto( const int id );

signals:
    void agregarVentana( QWidget * );
    void agregarDockWidget( Qt::DockWidgetArea, QDockWidget * );

public slots:
    void accept();

private slots:
    void cambioPrecioCosto( double costo );

private:
    double _recargo;
    bool _categorias;
    bool _marcas;
    bool _descripcion;
    bool _stock;
    bool _modelo;
    QString _codigo_anterior;
    QString _nombre_anterior;
    MProductos *modelo;
    QDataWidgetMapper *mapa;
    QSqlTableModel::EditStrategy _anterior;

};

#endif // FORMMODIFICARPRODUCTO_H
