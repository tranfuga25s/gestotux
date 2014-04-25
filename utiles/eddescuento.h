#ifndef EDDESCUENTO_H
#define EDDESCUENTO_H

#include <QBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QDialog>
#include <QEvent>
#include "ecbtabla.h"

class EDDescuento : public QDialog
{
    Q_OBJECT
    
public:
    EDDescuento( QWidget *parent = 0 );

signals:
    void agregarDescuento( QString, double );

public slots:
    void accept();

protected slots:
    void cargarPorcentaje( int id );
    void sinDatosPorcentaje();
    
protected:
    QVBoxLayout *verticalLayout;
    QLabel *LDescuento;
    QHBoxLayout *horizontalLayout;
    ECBTabla *CBDescuento;
    QDoubleSpinBox *dSBPorcentaje;
    QDialogButtonBox *buttonBox;
};

#endif // EDDESCUENTO_H
