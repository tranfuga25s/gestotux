#ifndef FORMRANGOFECHAS_H
#define FORMRANGOFECHAS_H

#include "ui_FormRangoFechasBase.h"

class FormRangoFechas : public QDialog, private Ui::FormRangoFechasBase
{
    Q_OBJECT
    
public:
    FormRangoFechas( QWidget *parent = 0 );
    QDate fechaInicio() { return DEDesde->date(); }
    QDate fechaFin() { return DEHasta->date(); }

public slots:
    void accept();
    
protected:
    void changeEvent(QEvent *e);

};

#endif // FORMRANGOFECHAS_H
