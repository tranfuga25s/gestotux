#ifndef EINPUTDIALOG_H
#define EINPUTDIALOG_H

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QInputDialog>
#include <QDebug>
#include <QApplication>
#include <QDialogButtonBox>

class EInputDialogDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    EInputDialogDoubleSpinBox(QWidget *parent = 0)
        : QDoubleSpinBox(parent) {
        connect(lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(notifyTextChanged()));
        connect(this, SIGNAL(editingFinished()), this, SLOT(notifyTextChanged()));
    }

signals:
    void textChanged(bool);

private slots:
    void notifyTextChanged() { emit textChanged(hasAcceptableInput()); }

private:
    void keyPressEvent(QKeyEvent *event) {
        if ( (event->key() == Qt::Key_Return ||
              event->key() == Qt::Key_Enter)   &&
             !hasAcceptableInput() ) {
#ifndef QT_NO_PROPERTIES
            setProperty("value", property("value"));
#endif
        } else if( (event->key() == Qt::Key_Period
                    && event->modifiers().testFlag( Qt::KeypadModifier )  ) ) {
            // Convierto el ingreso en una coma
            QLocale locale;
            int pos_cursor = this->lineEdit()->cursorPosition();
            int pos_coma = this->lineEdit()->text().indexOf( "," );
            if( pos_coma != -1 && pos_cursor < pos_coma ) {
                this->lineEdit()->setText( this->lineEdit()->text().remove( pos_cursor, abs( pos_coma-pos_cursor ) ) );
            }
            this->lineEdit()->setText( this->lineEdit()->text().split( "," ).first().replace( ",", "" ).append( locale.decimalPoint() ) );
            event->accept();
        } else {
            QDoubleSpinBox::keyPressEvent(event);
        }
        notifyTextChanged();
    }

    void mousePressEvent(QMouseEvent *event) {
        QDoubleSpinBox::mousePressEvent(event);
        notifyTextChanged();
    }

};

class EInputDialog : public QInputDialog
{
    Q_OBJECT
public:
    EInputDialog( QWidget *parent = 0 );

    static double getImporte(QWidget *parent, const QString &title, const QString &label, double value = 0,
                                double minValue = -2147483647, double maxValue = 2147483647,
                                int decimals = 1, bool *ok = 0, Qt::WindowFlags flags = 0);
};

#endif // EINPUTDIALOG_H
