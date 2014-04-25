#ifndef FORMAGREGARPRODUCTO_H
#define FORMAGREGARPRODUCTO_H

#include "ui_formproductobase.h"
#include <QDialog>
#include <QDockWidget>

/*!
 * \brief Formulario para agregar un producto
 *
 * Clase que permite agregar un nuevo producto
 * \author Esteban Zeller
 */
class FormAgregarProducto : public QDialog, private Ui::FormProductoBase
{
    Q_OBJECT
    public:
        FormAgregarProducto( QWidget *parent = 0 );
        void setearPrecioCosto( const double precio );
        void setearNombre( const QString nombre );
        void setearStockInicial( const int cantidad );
        void setearNumeroAnterior( const int id_anterior ) { _id_anterior = id_anterior; }

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
        int _id_anterior;

    signals:
        void agregarVentana( QWidget * );
        void agregarDockWidget( Qt::DockWidgetArea, QDockWidget * );
        void agregarProducto( int anterior, int nuevo );
};

#endif // FORMAGREGARPRODUCTO_H
