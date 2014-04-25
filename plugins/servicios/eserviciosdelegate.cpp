#include "eserviciosdelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>

EServiciosDelegate::EServiciosDelegate( QObject *parent ) :
QItemDelegate(parent)
{}

void EServiciosDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    switch( index.column() ) {
        /* Para todos los Combobox, la estructura ingresada es pos con valor, texto, itemdata valor */
        case 6: // Periodo
        case 7: // Dia de cobro
        case 8: // Forma de pago incompleto
        {
            QComboBox *c = qobject_cast<QComboBox *>(editor);
            // pongo el item que corresponda segun el dato
            c->setCurrentIndex( c->findData( index.data( Qt::EditRole ).toInt() ) );
            break;
        }
        default:
        {  QItemDelegate::setEditorData( editor, index ); break; }
    }

}

void EServiciosDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch( index.column() ) {
        /* Para todos los Combobox, la estructura ingresada es pos con valor, texto, itemdata valor */
        case 6: // Periodo
        case 7: // Dia de cobro
        case 8: // Forma de pago incompleto
        {
            QComboBox *c = qobject_cast<QComboBox *>(editor);
            // pongo el item que corresponda segun el dato
            model->setData( index, c->itemData( c->currentIndex() ).toInt() );
            break;
        }
        case 1: // Nombre del servicio
        {
            QLineEdit *c = qobject_cast<QLineEdit *>(editor);
            model->setData( index, c->text() );
            break;
        }
        case 2: // Descripcion
        {
            QTextEdit *c = qobject_cast<QTextEdit *>(editor);
            model->setData( index, c->toPlainText() );
            break;
        }
        case 3: // Fecha de Alta
        case 4: // Fecha de Baja
        {
            QDateEdit *c = qobject_cast<QDateEdit *>(editor);
            model->setData( index, c->date() );
            break;
        }
        case 5: // Importe base
        {
            QDoubleSpinBox *c = qobject_cast<QDoubleSpinBox *>(editor);
            model->setData( index, c->value() );
            break;
        }
        default:
        {  QItemDelegate::setEditorData( editor, index ); break; }
    }
}
