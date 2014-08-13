#include "einputdialog.h"

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QDialogButtonBox>

EInputDialog::EInputDialog(QWidget *parent) :
QInputDialog(parent)
{}

double EInputDialog::getImporte(QWidget *parent, const QString &title, const QString &label,
                               double value, double min, double max, int decimals, bool *ok,
                               Qt::WindowFlags flags)
{
    QDialog dialogo( parent, flags );
    dialogo.setWindowTitle( title );
    dialogo.setModal( false );

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

    QDialogButtonBox *buttonBox = new QDialogButtonBox( parent );
    buttonBox->setOrientation( Qt::Horizontal );
    buttonBox->addButton( QDialogButtonBox::Ok );
    buttonBox->addButton( QDialogButtonBox::Cancel );
    dialogo.connect( buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
    dialogo.connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

    QVBoxLayout *layout2 = new QVBoxLayout();
    layout2->addItem( layout );
    layout2->addWidget( buttonBox );
    dialogo.setLayout( layout2 );

    int ret = dialogo.exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        return spin->value();
    } else {
        return value;
    }
}
