#ifndef EGARANTIASVG_H
#define EGARANTIASVG_H

#include <QObject>
#include <QPainter>
#include <QDomDocument>
#include <QMap>
#include <QSqlRecord>

class EGarantiaSVG : public QObject
{
    Q_OBJECT
public:
    EGarantiaSVG( QObject *parent = 0 );
    bool setearNombreArchivo( const QString nombre );
    void setearRegistro( QSqlRecord reg ) { _registro = reg; }
    void cargarDatos();
    void agregarMapeo( QString nombre_svg, QString nombre_registro );

public slots:
    void imprimir( QPainter *impresora );
    void imprimir( QPrinter *impresora );
    
private:
    QString _nombre_archivo;
    QDomDocument _domdoc;
    QSqlRecord _registro;
    QMap<QString,QString> _mapa;
    bool _valido;

    QByteArray obtenerByteArray();
    void hacerText( QString texto, QDomNode padre, const double ancho, double x, double y );
    double anchoCaracter( QString car );
    QString generarNumeroGarantia( int num_recibo );

};

#endif // EGARANTIASVG_H
