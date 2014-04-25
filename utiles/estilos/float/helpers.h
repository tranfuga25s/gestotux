//
// C++ Interface: helpers
//
// Description: 
//
//
// Author: Matías Costa <mcc3@alu.um.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qglobal.h>
#include <QtGui>

QString contractText(const QString &text, const QFontMetrics &fontMetrics, const QRect &rect, int *textWidth = 0);
void renderTiled( const QRect &rect, QPainter *p, const QPixmap &pm, const QBrush &tbrush, bool fill = false );
void renderTiled( const QRect &rect, QPainter *p, const QPixmap &pm );
void renderGripper( QPainter *p, const QRect &rect, bool horizontal = false );
QPolygon buttonBorder( const QRect &rect );
QPolygon dockBaseBorder( const QRect &rect );
QPolygon dockTitleBorder( const QRect &rect );
QPolygon groupboxBorder( const QRect &rect );
void renderProgressLines( const QRect &rect, QPainter *p, bool vertical );
void drawArrow(const QStyle *style, const QStyleOptionToolButton *toolbutton,
		      const QRect &rect, QPainter *painter, const QWidget *widget = 0);
void renderSplitter(const QStyleOption *option, QPainter *painter, bool horizontal);

static const int menuItemFrame        =  2; // menu item frame width
static const int menuItemHMargin      =  3; // menu item hor text margin
static const int menuItemVMargin      =  2; // menu item ver text margin
static const int menuRightBorder      = 15; // right border on windows

inline QRect box( const QRect &r, bool frame = false ) 
{ 
	return r.adjusted( 3, frame? 3:1, -4, -4 ); 
}

// inline QRect unbox( const QRect &r ) { return r.adjusted( 0, 3, 0, 0 ); }

inline void drawPixmapCentered( const QRect &r, QPainter *p, const QPixmap &pm )
{
	p->drawPixmap( r.left() +(r.width() -pm.width())/2, r.top() +(r.height() -pm.height())/2, pm );
}

inline QColor outlineColor( const QColor c ) 
{ 
	QColor r= c.toHsv();
	r.setHsv( r.hue(), qMin(r.saturation()+26, 255), int( r.value()*0.56 ) );
	return r;
}

inline QColor outlineColor( const QStyleOption *o ) {
	return outlineColor( o->palette.button().color() );
}

inline QColor alternateColor( const QColor c ) 
{ 
	QColor r= c.toHsv();
	r.setHsv( r.hue(), qMin(r.saturation()+11, 255), int( r.value()*0.9 ) );
	return r;
}

inline QColor alternateColor2( const QColor c ) 
{ 
	QColor r= c.toHsv();
	r.setHsv( r.hue(), qMin(r.saturation()+20, 255), 255 );
	return r;
}

inline QColor alphaMix(QColor a, QColor b, float p = 0.5) 
{ 
	return QColor (  (int)( a.red()*p + b.red()*(1-p) ),
			  (int)( a.green()*p + b.green()*(1-p) ),
			  (int)( a.blue()*p + b.blue()*(1-p) ) );
}

