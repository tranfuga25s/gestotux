#include "einputdialog.h"

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>

EInputDialog::EInputDialog(QWidget *parent) :
QInputDialog(parent)
{}

double EInputDialog::getDouble(QWidget *parent, const QString &title, const QString &label,
                               double value, double min, double max, int decimals, bool *ok,
                               Qt::WindowFlags flags)
{
    QDialog dialogo( parent, flags );
    dialogo.setWindowTitle(title);

    QLabel *_label = new QLabel( parent );
    _label->setText( label );

    EInputDialogDoubleSpinBox *spin = new EInputDialogDoubleSpinBox( parent );
    spin->setDecimals( decimals );
    spin->setRange( min, max );
    spin->setValue( value );
    spin->setPrefix( "$ " );

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( _label );
    layout->addWidget( spin );

    dialogo.setLayout( layout );

    int ret = dialogo.exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        return spin->value();
    } else {
        return value;
    }
}
