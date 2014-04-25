//
// C++ Implementation: helpers
//
// Description: 
//
//
// Author: Matías Costa <mcc3@alu.um.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "helpers.h"

QString contractText(const QString &text, const QFontMetrics &fontMetrics, const QRect &rect, int *textWidth)
{
    // Chop and insert ellide into title if text is too wide
	QString title = text;
	int width = textWidth ? *textWidth : fontMetrics.width(text);
	QString ellipsis = QLatin1String("...");
	if (width > rect.width()) {
		QString leftHalf = title.left(title.size() / 2);
		QString rightHalf = title.mid(leftHalf.size() + 1);
		while (!leftHalf.isEmpty() && !rightHalf.isEmpty()) {
			leftHalf.chop(1);
			int width = fontMetrics.width(leftHalf + ellipsis + rightHalf);
			if (width < rect.width()) {
				title = leftHalf + ellipsis + rightHalf;
				width = width;
				break;
			}
			rightHalf.remove(0, 1);
			width = fontMetrics.width(leftHalf + ellipsis + rightHalf);
			if (width < rect.width()) {
				title = leftHalf + ellipsis + rightHalf;
				width = width;
				break;
			}
		}
	}
	if (textWidth)
		*textWidth = width;
	return title;
}
/**
 * rect: target area to paint
 * p: the p, really
 * pm: the pixmap to paint tiled
 * tSize: tile size
 * center: QBrush to paint the center with.
 */
void renderTiled( const QRect &rect, QPainter *p, const QPixmap &pm, const QBrush &tbrush, bool fill )
{
	p->save();
	
	// iw: Inner zone Width; ih: Inner zone Height
	const int tSize = pm.width() /3;
	const int iw = rect.width() -2*tSize, ih = rect.height() -2*tSize;
	QBrush brush( tbrush );
			
	// pmCorner refers to &pm corner, corner to &rect corner
	QRect pmCorner( 0, 0, tSize, tSize );
	QRect corner( rect.x(), rect.y(), tSize, tSize );
// 	corner.setTopLeft( rect.topLeft() );
	brush.setTexture( pm.copy( pmCorner ) );
// 	if( center != Qt::transparent )
// 		brush.setColor( center );
	// we start on top-left corner
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	// moving right => top 
	pmCorner.translate( tSize, 0 );
	brush.setTexture( pm.copy( pmCorner ) );
	p->fillRect( rect.left()+tSize, rect.top(), iw, tSize, brush );	
	// moving right => top-right corner
	pmCorner.translate( tSize, 0 );
	corner.translate( rect.width() -tSize, 0 );
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	// moving down => right
	pmCorner.translate( 0, tSize );
	corner.translate( 0, tSize );
	corner.setHeight( ih );
	brush.setTexture( pm.copy( pmCorner ) );
	p->fillRect( corner, brush );
	// moving down => bottom-right
	pmCorner.translate( 0, tSize );
	corner.translate( 0, ih );
	corner.setHeight( tSize );
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	//moving left => bottom
	pmCorner.translate( -tSize, 0 );
	corner.translate( -iw, 0);
	corner.setWidth( iw );	
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	// moving left => bottom-left
	pmCorner.translate( -tSize, 0 );
	corner.translate( -tSize, 0);
	corner.setWidth( tSize );	
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	//moving up => left
	pmCorner.translate( 0, -tSize );	
	corner.translate( 0, -ih );
	corner.setHeight( ih );
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	

	if( fill )
		p->fillRect( rect.x()+tSize, rect.y()+tSize, iw, ih, tbrush );
	
	p->restore();
}

void renderTiled( const QRect &rect, QPainter *p, const QPixmap &pm )
{
	p->save();
	
	// iw: Inner zone Width; ih: Inner zone Height
	const int tSize = pm.width() /3;
	const int iw = rect.width() -2*tSize, ih = rect.height() -2*tSize;
	QBrush brush;
			
	// pmCorner refers to &pm corner, corner to &rect corner
	QRect pmCorner( 0, 0, tSize, tSize );
	QRect corner( rect.x(), rect.y(), tSize, tSize );
// 	corner.setTopLeft( rect.topLeft() );
	brush.setTexture( pm.copy( pmCorner ) );
// 	if( center != Qt::transparent )
// 		brush.setColor( center );
	// we start on top-left corner
// 	qDebug() << "p->setBrushOrigin( " <<corner.topLeft() << ")";
	p->setBrushOrigin( corner.topLeft() );
	
	p->fillRect( corner, brush );
// 	p->drawPixmap( corner.topLeft(), pm.copy( pmCorner ) );
	
	// moving right => top 
	pmCorner.translate( tSize, 0 );
	brush.setTexture( pm.copy( pmCorner ) );
	p->fillRect( rect.left()+tSize, rect.top(), iw, tSize, brush );	
	// moving right => top-right corner
	pmCorner.translate( tSize, 0 );
	corner.translate( rect.width() -tSize, 0 );
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	// moving down => right
	pmCorner.translate( 0, tSize );
	corner.translate( 0, tSize );
	corner.setHeight( ih );
	brush.setTexture( pm.copy( pmCorner ) );
	p->fillRect( corner, brush );
	// moving down => bottom-right
	pmCorner.translate( 0, tSize );
	corner.translate( 0, ih );
	corner.setHeight( tSize );
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	//moving left => bottom
	pmCorner.translate( -tSize, 0 );
	corner.translate( -iw, 0);
	corner.setWidth( iw );	
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	// moving left => bottom-left
	pmCorner.translate( -tSize, 0 );
	corner.translate( -tSize, 0);
	corner.setWidth( tSize );	
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );
	//moving up => left
	pmCorner.translate( 0, -tSize );	
	corner.translate( 0, -ih );
	corner.setHeight( ih );
	brush.setTexture( pm.copy( pmCorner ) );
	p->setBrushOrigin( corner.topLeft() );
	p->fillRect( corner, brush );	
	p->restore();
}

// Taken the look from konx
void renderGripper( QPainter *p, const QRect &rect,  bool horizontal )
{
	int x, y, w, h;
	rect.getRect( &x, &y, &w, &h );
	int cx = x + ( w/2 );
	int cy = y + ( h/2 );
	
	if ( horizontal ){
		// adjust length to keep centered
		const int l = 4 + ( h%2 );
		cy -= 2;
		// draw gripper

		p->drawLine( cx-3, cy, cx-3, cy+l   );
		p->drawLine( cx  , cy, cx  , cy+l );
		p->drawLine( cx+3, cy, cx+3, cy+l   );
	} else {// vertical
		// adjust length to keep centered
		const int l = 4 +( w%2 );
		cx -= 2;
		p->drawLine( cx, cy-3, cx+l,cy-3 );
		p->drawLine( cx, cy  , cx+l,cy   );
		p->drawLine( cx, cy+3, cx+l,cy+3 );
	}
}

void renderProgressLines( const QRect &rect, QPainter *p, bool vertical )
{
	if ( vertical ) {
		const int x1 = rect.left() +5;
		const int x2 = rect.right() -5;
		for ( int y = rect.bottom() -10; y > rect.top(); y -= 10 ) {
			p->drawLine( x1, y, x2, y );
		}
	} else {
		const int y1 = rect.top() +5;
		const int y2 = rect.bottom() -5;
		for ( int x = rect.left() +10; x < rect.right(); x += 10 ) {
			p->drawLine( x, y1, x, y2 );
		}
	}
}

/*
	   x1         x2
	y1 ..o..|....o. y1
	   .....|.....o
	   1....|......
	   .....|......
	   ------------
	   .....|......
	   .....|......
	   .....|.....o
	   .....|......
	   o....|......
	y2 .o...|..o... y2
	   x1         x2
	
*/
QPolygon buttonBorder( const QRect &rect )
{
	QPolygon polygon( 8 );
	// (top, left) and (right, bottom) coordinates of the border: (x1, y1) and (x2, y2)
	int x1, y1, x2, y2;
	rect.getCoords( &x1, &y1, &x2, &y2 );
	polygon.putPoints(0, 8, 
			  x1  ,y1+2,  x1+2,y1  ,
       			  x2-1,y1  ,  x2  ,y1+1,
	      		  x2  ,y2-3,  x2-3,y2  ,
	   		  x1+1,y2  ,  x1  ,y2-1 );
	return polygon;
}

/*
	   x1         x2
	y1 ..o..|....o. y1
	   .....|.....o
	   1....|......
	   .....|......
	   ------------
	   .....|......
	   .....|.....o
	   .....|......
	   .....|....o.
	   o....|...o..
	y2 .o...|.o.... y2
	   x1         x2
	
*/
QPolygon dockBaseBorder( const QRect &rect )
{
	QPolygon polygon( 10 );
	// (top, left) and (right, bottom) coordinates of the border: (x1, y1) and (x2, y2)
	int x1, y1, x2, y2;
	rect.getCoords( &x1, &y1, &x2, &y2 );
	polygon.putPoints(0, 10, 
			  x1  ,y1+2,  x1+2,y1,
			  x2-1,y1  ,  x2  ,y1+1,
			  x2  ,y2-5,  x2-1,y2-2,  x2-2,y2-1,  x2-5,y2,
			  x1+1,y2  ,  x1  ,y2-1
			 );
	return polygon;
}

/*
	   x1         x2
	y1 ..o..|....o. y1
	   .....|.....o
	   1....|......
	   .....|......
	   ------------
	   .....|......
	   .....|.....o
	   .....|......
	   .....|....o.
	   .....|...o..
	   .....|..o...
	y2 o....|o.....y2
	   x1         x2
	
*/
QPolygon dockTitleBorder( const QRect &rect )
{
	QPolygon polygon( 10 );
	// (top, left) and (right, bottom) coordinates of the border: (x1, y1) and (x2, y2)
	int x1, y1, x2, y2;
	rect.getCoords( &x1, &y1, &x2, &y2 );
	polygon.putPoints(0, 10, 
			  x1  ,y1+2,  x1+2,y1,
			  x2-1,y1  ,  x2  ,y1+1,
			  x2  ,y2-6,  x2-1,y2-3, x2-2,y2-2, x2-4,y2-1,  x2-6,y2,
			  x1,y2
			 );
	return polygon;
}

/*
	   x1         x2
	y1 ....o|...o... y1
	   ..o..|.......
	   .o...|.....o.
	   .....|.......
	   1....|.......
	   -------------
	   .....|.....o.
	   .....|.......
	   o....|....o..
	   .....|...o...
	   ..o..|.o.....  
	y2 .....|.......y2
	   x1         x2
	
*/
QPolygon groupboxBorder( const QRect &rect )
{
	QPolygon polygon( 12 );
	// (top, left) and (right, bottom) coordinates of the border: (x1, y1) and (x2, y2)
	int x1, y1, x2, y2;
	rect.getCoords( &x1, &y1, &x2, &y2 );
	polygon.putPoints(0, 12, 
			  x1  ,y1+4,  x1+1,y1+2,  x1+2,y1+1,  x1+4,y1,
     			  x2-3,y1  ,  x2-1,y1+2,
     			  x2-1,y2-6,  x2-2,y2-3,  x2-3,y2-2,  x2-6,y2-1,
			  x1+2,y2-1,  x1  ,y2-3
			 );
	return polygon;
}

void drawArrow(const QStyle *style, const QStyleOptionToolButton *toolbutton,
		      const QRect &rect, QPainter *painter, const QWidget *widget)
{
	QStyle::PrimitiveElement pe;
	switch (toolbutton->arrowType) {
		case Qt::LeftArrow:
			pe = QStyle::PE_IndicatorArrowLeft;
			break;
		case Qt::RightArrow:
			pe = QStyle::PE_IndicatorArrowRight;
			break;
		case Qt::UpArrow:
			pe = QStyle::PE_IndicatorArrowUp;
			break;
		case Qt::DownArrow:
			pe = QStyle::PE_IndicatorArrowDown;
			break;
		default:
			return;
	}
	QStyleOption arrowOpt;
	arrowOpt.rect = rect;
	arrowOpt.palette = toolbutton->palette;
	arrowOpt.state = toolbutton->state;
	style->drawPrimitive(pe, &arrowOpt, painter, widget);
}

// Taken from Qt
static const char * const float_simple_toolbarhandle[] = {
	"3 3 4 1",
 "       c None",
 ".      c #C5C5C5",
 "+      c #EEEEEE",
 "@      c #FAFAFA",
 ".. ",
 ".+@",
 " @@"};


void renderSplitter(const QStyleOption *option, QPainter *painter, bool horizontal) {
	QRect rect = option->rect;
	const QColor borderColor = option->palette.background().color().dark(178);
	const QColor alphaCornerColor = alphaMix(option->palette.background().color(), borderColor);

	QImage handle(float_simple_toolbarhandle);
// 	qDebug() << "rSpl" << handle.isNull() ;
	handle.setColor(1, alphaCornerColor.rgba());
	handle.setColor(2, alphaMix(alphaCornerColor, option->palette.base().color()).rgba());
	handle.setColor(3, option->palette.base().color().rgba());

	const int spacing = 2;

	if ( horizontal ) {
		rect.setWidth( qMin( rect.width() * 2/3, 40 ) );
		rect.setHeight( 3 );
		rect.moveCenter( option->rect.center() );
		int nchunks = rect.width() / (handle.width() + spacing);
		for (int i = 0; i < nchunks; ++i)
			painter->drawImage(QPoint(rect.left() + i * (handle.width() + spacing), rect.top()), handle);
	} else {
		rect.setWidth( 3 );
		rect.setHeight( qMin( rect.height() * 2/3, 40 ) );
		rect.moveCenter( option->rect.center() );
		int nchunks = rect.height() / (handle.height() + spacing);
		for (int i = 0; i < nchunks; ++i) {
			painter->drawImage(QPoint(rect.left(), rect.top() + i * (handle.height() + spacing)), handle);
		}
	}
	
	if ((option->state & QStyle::State_Enabled) && (option->state & QStyle::State_MouseOver))
		painter->fillRect(option->rect, QColor(255, 255, 255, 128));
	
	
}
