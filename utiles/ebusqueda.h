#ifndef EBUSQUEDA_H
#define EBUSQUEDA_H

#include <QDockWidget>
#include "ui_EBusquedaBase.h"
#include <QStringList>
#include <QSqlTableModel>

/*!
 * \brief Ventana de filtrado generica
 *
 * Esta clase sirve como sistema base de filtrado. Se agregará automaticamente todos los sistemas internos
 * para realizar los filtrados en el modelo.
 * Se inicializará y se deberá agregar los filtros mediante agregar filtro.
 * \author Esteban Javier Zeller <ezeller@gmail.com>
 */
class EBusqueda : public QDockWidget, private Ui::EBusquedaBase
{
    Q_OBJECT
public:
    EBusqueda( QWidget *parent, QSqlTableModel *modelo, QString titulo = QString() );
    void setearTitulo( QString titulo );
    void agregarFiltro( QString nombre, QString filtro );

protected:
    void changeEvent(QEvent *e);

public slots:
    void borrar();

protected slots:
    void filtrar();

signals:
    void cerrando();

private:
   /*!
    * Puntero al modelo del cual queremos aplicar los filtros
    */
    QSqlTableModel *_modelo;
   /*!
    * Lista de filtros asociados con la posicion en el combobox
    */
    QStringList filtros;
   /*!
    * Guarda el filtro del modelo antes de que fuera habilitada la busqueda
    */
    QString _filtro_anterior;
    
};

#endif // EBUSQUEDA_H
