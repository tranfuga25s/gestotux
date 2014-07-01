#include "eddescuento.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

EDDescuento::EDDescuento(QWidget *parent) :
QDialog(parent)
{
    setObjectName(QString::fromUtf8("EDDescuento"));
    resize(400, 98);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    LDescuento = new QLabel(this);
    LDescuento->setObjectName(QString::fromUtf8("LDescuento"));

    verticalLayout->addWidget(LDescuento);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    CBDescuento = new ECBTabla(this);
    CBDescuento->setObjectName(QString::fromUtf8("CBDescuento"));
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(CBDescuento->sizePolicy().hasHeightForWidth());
    CBDescuento->setSizePolicy(sizePolicy);
    CBDescuento->setEditable(true);

    horizontalLayout->addWidget(CBDescuento);

    dSBPorcentaje = new QDoubleSpinBox(this);
    dSBPorcentaje->setObjectName(QString::fromUtf8("dSBPorcentaje"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(dSBPorcentaje->sizePolicy().hasHeightForWidth());
    dSBPorcentaje->setSizePolicy(sizePolicy1);
    dSBPorcentaje->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    dSBPorcentaje->setDecimals(3);
    dSBPorcentaje->setMaximum(1e+07);

    horizontalLayout->addWidget(dSBPorcentaje);

    verticalLayout->addLayout(horizontalLayout);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    verticalLayout->addWidget(buttonBox);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);

    CBDescuento->setearTabla( "tipo_descuento" );
    CBDescuento->setearCampoId( "id_tipo_descuento" );
    CBDescuento->setearCampoTexto( "nombre" );
    CBDescuento->setearCampoOrden( "nombre" );
    CBDescuento->setearCampoBusquedaExtra( "codigo" );

    connect( CBDescuento, SIGNAL( cambioId( int ) ), this, SLOT( cargarPorcentaje( int ) ) );
    connect( CBDescuento, SIGNAL( sinDatos() ), this, SLOT( sinDatosPorcentaje() ) );
}

void EDDescuento::accept()
{
    emit agregarDescuento( CBDescuento->currentText(), dSBPorcentaje->value() );
    QDialog::accept();
}

void EDDescuento::cargarPorcentaje( int id )
{
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT cantidad FROM tipo_descuento WHERE id_tipo_descuento = %1" ).arg( id ) ) ) {
      cola.next();
      dSBPorcentaje->setValue( cola.record().value(0).toDouble() );
  } else {
      qDebug( "Error al ejecutar la cola de obtención de porcentaje de un tipo de descuento" );
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
}

void EDDescuento::sinDatosPorcentaje()
{
    CBDescuento->setEditable( true );
    CBDescuento->setEnabled( true );
}
