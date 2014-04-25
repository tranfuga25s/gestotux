/****************************************************************************
**  Copyright (C) 2008 by love4u.bluemoon
**  love4u.bluemoon@yahoo.com.cn
			
**  This  library is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this library; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
****************************************************************************/

#ifndef l4ustyle2_H
#define l4ustyle2_H

#include <QSet>
#include <QTime>
#include <QPlastiqueStyle>
#include <QTabBar>

class QPainterPath;

class L4uStyle2 : public QPlastiqueStyle
{
    Q_OBJECT
    typedef QPlastiqueStyle ParentStyle;
    
public:
    L4uStyle2();
    ~L4uStyle2();
    
    void unpolish( QWidget *widget );
    void polish( QWidget *widget );
    void drawPrimitive( PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget ) const;
    void drawControl( ControlElement control, const QStyleOption *option, QPainter *painter, const QWidget *widget ) const;
    void drawComplexControl( ComplexControl control, const QStyleOptionComplex *option,  QPainter *painter, const QWidget *widget = 0 ) const;
    QRect subControlRect( ComplexControl control, const QStyleOptionComplex * option, SubControl subControl, const QWidget * widget = 0 ) const;
    QSize sizeFromContents( ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget * widget = 0 ) const;
    int pixelMetric( PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0 ) const;
    int styleHint( StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0, QStyleHintReturn *returnData = 0 ) const;
    bool eventFilter( QObject * watched, QEvent * event );
    void timerEvent( QTimerEvent *event );
    
private:
    enum ContourHint 
    {
	DrawNone = 0x0000,
	RoundUpperLeft = 0x0001,
	RoundUpperRight = 0x0002,
	RoundLowerLeft = 0x0004,
	RoundLowerRight = 0x0008,
	DrawLeft = 0x0010,
	DrawTop = 0x0020,
	DrawRight = 0x0040,
	DrawBottom = 0x0080,
    };
  
    void drawCheckMark(  QPainter *painter, const QRect &rect, const QBrush &brush ) const;
    void drawAnyArrow( PrimitiveElement pe, QPainter *painter, const QRect &rect, const QBrush &brush, bool real = false ) const;
    void drawButton( QPainter *p, const QRect &rect, const QPalette &pal, State state) const;
    void drawButtonDropDown( QPainter *p, const QRect &rect, const QPalette &pal, State state ) const;
    void drawSlider( QPainter *p, const QRect &rect, const QPalette &pal, State state) const;
    void drawContour( QPainter *p,  int l, int t, int r, int b, int how = int(DrawLeft | DrawTop | DrawRight | DrawBottom | RoundUpperLeft | 
									      RoundUpperRight | RoundLowerLeft | RoundLowerRight) ) const; 
    void drawLinearGrad( QPainter *p, const QRect &fill, const QColor &first, const QColor &last, bool horz = true ) const;
    void fillButton( QPainter *p, const QRect &fill, const QPalette &pal, State flags ) const;
    QPainterPath roundRectPathF( const QRectF &fill, qreal radius = 4 ) const;
    QPainterPath roundRectPath( const QRect &fill, int radius = 4 ) const;
    QPainterPath roundTabPathF( const QRectF &fill, QTabBar::Shape shape, bool close = true ) const;
    QPainterPath roundTabPath( const QRect &fill, QTabBar::Shape shape ) const;
    void drawSurface( QPainter *p, const QRect &fill, const QColor &top, const QColor &bot, bool horz = true, bool bar = false ) const;
    void drawBoldPoint( QPainter  *p, const QColor &dark, const QColor &light, int x, int y, bool horz = FALSE ) const;
    bool isRnd( int w, int h ) const;
    
    QSet<QWidget *> bars;
    QTime clock;
    int timer;
};

#endif
