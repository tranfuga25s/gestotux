#ifndef ESERVICIOSDELEGATE_H
#define ESERVICIOSDELEGATE_H

#include <QItemDelegate>

/*!
 * \brief Delegate para edición de servicios
 *
 * Delegate para la edición de los campos que componen al formulario de servicios.
 */
class EServiciosDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    EServiciosDelegate( QObject *parent = 0 );
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
};

#endif // ESERVICIOSDELEGATE_H
