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

#include "l4ustyle2.h"
#include "misc.h"
#include <QStringList>
#include <QString>
#include <QTimerEvent>
#include <QtGui>

static const bool UsePixmapCache = true;
static const int updateTime = 50;

class L4uStyle2Plugin : public QStylePlugin
{
public:
    L4uStyle2Plugin() {}
    ~L4uStyle2Plugin() {}
    
    QStringList keys() const
    {
	return QStringList() << QString::fromUtf8( "l4ustyle2" );
    }
    
    QStyle* create( const QString& key )
    {
	if ( key == QString::fromUtf8("l4ustyle2") )
	    return new L4uStyle2();
	
	return 0;
    }
};

Q_EXPORT_PLUGIN2(l4ustyle2, L4uStyle2Plugin)
	
L4uStyle2::L4uStyle2()
{
    timer  = 0;
    QPixmapCache::setCacheLimit( 320 );
}

L4uStyle2::~L4uStyle2()
{ 
}

inline bool L4uStyle2::isRnd( int w, int h ) const
{
    int ext = pixelMetric( PM_ScrollBarExtent );
    bool ret;
    
    if ( (w > ext) && (h > ext) )
	ret = true;
    else
	ret =  false;
    
    return ret;  
}

void  L4uStyle2::polish( QWidget *widget )
{
    if ( qobject_cast<QPushButton *>(widget) || qobject_cast<QComboBox *>(widget) ||  qobject_cast<QAbstractSpinBox *>(widget) ||
	  qobject_cast<QCheckBox *>(widget) || qobject_cast<QRadioButton *>(widget) || qobject_cast<QTabBar *>(widget) || 
	  qobject_cast<QScrollBar *>(widget) || qobject_cast<QHeaderView *>(widget) || qobject_cast<QSlider *>(widget) ) {
	widget->setAttribute( Qt::WA_Hover, true );
    }
    else if ( QLCDNumber * lcd = qobject_cast<QLCDNumber *>(widget) ) {
	lcd->setFrameShape( QFrame::StyledPanel );
	lcd->setSegmentStyle(QLCDNumber::Flat);
    }
    else if ( QFrame * frame = qobject_cast<QFrame *>(widget) ) {
	if ( (QFrame::Box == frame->frameShape()) || (QFrame::Panel == frame->frameShape()) || (QFrame::WinPanel == frame->frameShape()) )
	    frame->setFrameShape( QFrame::StyledPanel );
	else if ( (QFrame::HLine ==  frame->frameShape()) || (QFrame::VLine ==  frame->frameShape()) )
	    frame->installEventFilter( this );
    }
    else if ( QGroupBox *gbox = qobject_cast<QGroupBox *>(widget) ) {
	if ( !gbox->isFlat() )
	    gbox->setAlignment( Qt::AlignHCenter );
    }
    else if ( QProgressBar *pb = qobject_cast<QProgressBar *>(widget) ) {
	if ( pb->minimum() == 0 && pb->maximum() == 0 && !widget->inherits("StatusBarSpaceInfo") ) {
	    pb->installEventFilter( this );
	}
    }
    else if ( QDockWidget* dw = qobject_cast<QDockWidget *>(widget) ) {
	dw->installEventFilter( this );
    }
    
    if ( QAbstractItemView *view = qobject_cast<QAbstractItemView *>(widget) ) 
	view->viewport()->setAttribute( Qt::WA_Hover );

    
//    ParentStyle::polish( widget );
}

void L4uStyle2::unpolish( QWidget *widget )
{
    if ( qobject_cast<QFrame *>(widget) ) {
	widget->removeEventFilter( this );
    }
    else if ( QProgressBar *pb = qobject_cast<QProgressBar *>(widget) ) {
	if ( pb->minimum() == 0 && pb->maximum() == 0 && !widget->inherits("StatusBarSpaceInfo") ) {
	    pb->removeEventFilter( this );
	}
    }
    else if ( QDockWidget* dw = qobject_cast<QDockWidget *>(widget) ) {
	dw->removeEventFilter( this );
    }
    
//    ParentStyle::unpolish( widget );
}

void L4uStyle2::drawPrimitive( PrimitiveElement element, const QStyleOption *option,
			       QPainter *painter, const QWidget *widget ) const
{
    if ( !option ) return;
    
    int left, right, top, bottom, width, height;
    bool isEnabled = ( QStyle::State_Enabled & option->state );
    option->rect.getCoords( &left, &top, &right, &bottom );
    width = option->rect.width(); 
    height = option->rect.height();
    
    switch( element ) {
    case PE_PanelButtonCommand:
    case PE_PanelButtonBevel:
	{
	    if ( const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option) ) {
		if ( ((QStyleOptionButton::Flat | QStyleOptionButton::CommandLinkButton) & button->features) 
		    &&  !(State_MouseOver & button->state) )
		    painter->fillRect( button->rect, painter->background() );
		else
		    drawButton( painter, button->rect, button->palette, button->state );
	    }
	    break;
	}
    case PE_FrameDefaultButton:
	{ 
	    QColor hl = option->palette.highlight().color();
	    hl.setAlphaF( 0.7 );
	    painter->setPen( hl );
	    painter->setBrush( Qt::NoBrush );
	    
	    if ( isRnd( width, height) ) {
		painter->setRenderHint( QPainter::Antialiasing, true );
		painter->drawPath( roundRectPath( option->rect.adjusted(1, 1, 0, 0), 5 ) );
		painter->setRenderHint( QPainter::Antialiasing, false );
	    }
	    else {	
		drawContour( painter, left + 1, top + 1, right - 1, bottom - 1 );
		painter->setPen( hl );
		painter->drawRect( option->rect.adjusted(1, 1, -2, -2) );
	    }
	    
	    break;
	}
    case PE_PanelButtonTool:
    case PE_IndicatorButtonDropDown:
	{   	    
	    if ( isEnabled ) {
		State state = option->state;
		bool bar = false;
		bool horz = true;
		
		if ( widget && widget->parentWidget() ) {
		    if ( QToolBar *tb = qobject_cast<QToolBar *>(widget->parentWidget()) ) {
			if ( Qt::Vertical == tb->orientation() )
			    horz = false;
		    }
		    else if ( QDockWidget *dw = qobject_cast<QDockWidget *>(widget->parentWidget()) ) {
			if ( QDockWidget::DockWidgetVerticalTitleBar & dw->features() )
			    horz = false;
		    }
		   
		    if ( qobject_cast<QTabBar *>(widget->parentWidget()) ) {
			bar = true;
			painter->fillRect( option->rect.adjusted(0, 0, 0, -2 ), option->palette.window() );
			top += 1, height -= 2;
		    }
		    else if ( qobject_cast<QMenu *>(widget->parentWidget()) ) {
			state &= ~(State_On | State_Sunken);
			state |= State_Raised;
		    }
		}
	
		if ( !bar )
		    drawButton( painter, QRect( left, top, width, height), option->palette,  
				horz ? (state | State_Horizontal) : (state & ~State_Horizontal) );
		else
		    painter->fillRect( option->rect, option->palette.window() );
	    } 
	    break;
	}
    case PE_FrameFocusRect:
	{  
	    if ( (State_MouseOver & option->state) || (State_Selected & option->state) )
		return;
	    
	    painter->save();
	    painter->setRenderHint( QPainter::Antialiasing );
	    painter->setPen( Qt::red );
	    painter->setBrush( Qt::NoBrush );
	    painter->drawPath( roundRectPath(option->rect.adjusted(1, 1, 0, 0), 4) );
	    painter->restore();
	    break; 
	}
    case PE_PanelLineEdit:
	{
	    if ( const QStyleOptionFrame *lineEdit = qstyleoption_cast<const QStyleOptionFrame *>(option) ) {
		int lw = lineEdit->lineWidth;
		bool clipping = ( lineEdit->lineWidth > 0 && isRnd(width, height) );
		int tmp = ( lw ? 2 : 0 );
		 
		if (clipping )
		    painter->setClipPath(roundRectPath(lineEdit->rect, 4) );
		
		if ( isEnabled ) {
		    QColor c1 = option->palette.color( QPalette::Midlight );
		    QColor c2 = option->palette.color( QPalette::Base );
		    QLinearGradient linearGrad;
		    linearGrad.setStart( left + tmp, top + tmp );
		    linearGrad.setFinalStop( left + tmp, bottom - tmp );
		    linearGrad.setColorAt( 0.0, c1 );
		    linearGrad.setColorAt( 0.5, c1.lighter( 105 ) );
		    linearGrad.setColorAt( 1.0, c2 );
		    painter->fillRect( lineEdit->rect.adjusted(tmp, tmp,  tmp, -tmp), QBrush(linearGrad) );
		}
		else
		    painter->fillRect( lineEdit->rect.adjusted(tmp, tmp,  -tmp, -tmp), lineEdit->palette.base() );
		
		if ( clipping )
		    painter->setClipping( false );
		
		if ( lw > 0 )
		    drawPrimitive( PE_FrameLineEdit, option, painter, widget );
	    }
	    break;
	}
    case PE_IndicatorArrowUp:
    case PE_IndicatorArrowDown:
    case PE_IndicatorArrowRight:
    case PE_IndicatorArrowLeft:
	{
	    if ( PE_IndicatorArrowUp == element || PE_IndicatorArrowDown == element ) {
            QRect aRect( left + (width - 9) / 2, top + (height - 7) / 2, 9, 5 );
            drawAnyArrow( element, painter, aRect, option->palette.buttonText() );
        } else {
            QRect aRect( left + (width - 7) / 2, top + (height - 9) / 2, 7, 9 );
            drawAnyArrow( element, painter, aRect, option->palette.buttonText() );
	    }
	    break;
	}
    case PE_IndicatorSpinUp:
    case PE_IndicatorSpinDown:
	{	
	    QRect aRect( left + (width - 9) / 2, top + (height - 7) / 2, 9, 7 );
	    drawAnyArrow( element, painter, aRect, (isEnabled ? (State_MouseOver & option->state ? option->palette.highlight() :
								 option->palette.buttonText()) : option->palette.dark()) );
	    break;
	}
    case PE_IndicatorSpinPlus:
    case PE_IndicatorSpinMinus:
	{
	    QPen oldPen = painter->pen();
	    QRect rect = option->rect.adjusted( 0, 1, 0, 0 );
	    painter->setPen( QPen(option->palette.color(isEnabled ? (State_MouseOver & option->state ? QPalette::Highlight :
								 QPalette::ButtonText) : QPalette::Dark), 2) );
	    painter->drawLine( rect.center().x() - 3, rect.center().y(),  rect.center().x() + 3, rect.center().y() );
	    
	    if ( PE_IndicatorSpinPlus == element )
        painter->drawLine( rect.center().x(), rect.center().y() - 3, rect.center().x(), rect.center().y() + 3 );
	    
	    painter->setPen( oldPen );
	    break;
	}
    case PE_IndicatorItemViewItemCheck:
    case PE_IndicatorCheckBox:
	{
	    painter->save();
	    painter->setRenderHint( QPainter::Antialiasing, true );
	    State state = option->state;
	    
	    if ( isEnabled ) {
		int cw = pixelMetric( PM_IndicatorWidth, option, widget );
		int ch = pixelMetric( PM_IndicatorHeight, option, widget );
		QString pmkey;
		QPixmap pixmap( cw, ch );
		QColor c1 = option->palette.color( QPalette::Base );
		QColor c2 = option->palette.color( QPalette::Mid );
		
		if ( State_Sunken & state ) {
		    c1 = alphaBlendColors( c1, option->palette.color(QPalette::Mid).lighter(105), 175 );
		    c2 = alphaBlendColors( c2, option->palette.color(QPalette::Mid).darker(111), 175 );
	   
		}
		else if ( State_MouseOver & state ) {
		    c1 = alphaBlendColors( c1, option->palette.highlight().color().lighter(130), 211 );
		    c2 = alphaBlendColors( c2, option->palette.highlight().color().lighter(130), 211 );
		}
		
		pmkey.sprintf( "cc:%x-%x-%x-%x-%s", c1.rgb(), c2.rgb(), cw, ch, "checkbox" );
		
		if ( !UsePixmapCache || !QPixmapCache::find(pmkey, pixmap) ) {
		    pixmap.fill( Qt::transparent );
		    QRect pr = pixmap.rect();
		    QPainterPath rc = roundRectPath( pr, 4 );
		    QPainter pp( &pixmap );
		    QRadialGradient radialGrad;
		    radialGrad.setCenter( pr.center() );
		    radialGrad.setFocalPoint( pr.center() );
		    radialGrad.setRadius( pr.width() / 2 );
		    radialGrad.setColorAt( 0.0, c1 );
		    radialGrad.setColorAt( 0.1, c1 );
		    radialGrad.setColorAt( 1.0, c2.lighter(117) );
		   
		    pp.setRenderHint( QPainter::Antialiasing, true );
		    pp.setClipPath( rc  );
		    pp.fillPath( rc, radialGrad );
		    pp.setClipping( false );
		    pp.setBrush( Qt::NoBrush );
		    pp.setPen( option->palette.color(QPalette::Mid) );
		    pp.drawPath( roundRectPathF((QRectF(pr)).adjusted(0.5, 0.5, -0.5, -0.5), 4) );
		    pp.setPen( option->palette.color(QPalette::Mid).lighter(120) );
		    pp.drawPath( roundRectPathF((QRectF(pr)).adjusted(1.5, 1.5, -1.5, -1.5), 4) );
		    pp.end();
		    
		    if ( UsePixmapCache )
			QPixmapCache::insert( pmkey, pixmap );
		}
		
		painter->drawPixmap( option->rect.adjusted(1, 1, -1, -1), pixmap );
	    }
	    else {
		painter->setPen( option->palette.color(QPalette::Mid) );
		painter->setBrush( option->palette.midlight() );
		painter->drawPath( roundRectPathF((QRectF(option->rect)).adjusted(1.5, 1.5, -1.5, -1.5), 4) );
	    }
	    
	    if ( (QStyle::State_On | QStyle::State_NoChange) &  state  ) {
		QRect rect = QRect( left + (width - 13) / 2, top + (height - 13) / 2, 13, 13 );
		QPainterPath mark;
		QPoint cp = rect.center();
		mark.moveTo( rect.left() + 2, rect.top() + 2 );
		mark.lineTo( cp.x(), cp.y() - 1.4 );
		mark.lineTo( rect.right() - 2, rect.top() + 2 );
		mark.lineTo( cp.x() + 1.4, cp.y() );
		mark.lineTo( rect.right() - 2, rect.bottom() - 2 );
		mark.lineTo( cp.x(), cp.y() + 1.4 );
		mark.lineTo( rect.left() + 2, rect.bottom() - 2 );
		mark.lineTo( cp.x() - 1.4, cp.y() );
		mark.closeSubpath();
		painter->setPen( Qt::NoPen );
		painter->setBrush( QStyle::State_On & state ? option->palette.buttonText() : option->palette.color(QPalette::Dark).lighter(120) );
		painter->drawPath( mark );
	    }
	    
	    painter->restore();
	    break;
	}
    case PE_IndicatorRadioButton:
	{
	    painter->save();
	    painter->setRenderHint( QPainter::Antialiasing, true );
	    State state = option->state;
	    
	    if ( isEnabled ) {
		int rw = pixelMetric( PM_ExclusiveIndicatorWidth, option, widget );
		int rh = pixelMetric( PM_ExclusiveIndicatorHeight, option, widget );
		QString pmkey;
		QPixmap pixmap( rw, rh );
		QColor c1 = option->palette.color( QPalette::Base );
		QColor c2 = option->palette.color( QPalette::Mid );
		
		if ( State_Sunken & state ) {
		    c1 = alphaBlendColors( c1, option->palette.color(QPalette::Mid).lighter(105), 175 );
		    c2 = alphaBlendColors( c2, option->palette.color(QPalette::Mid).darker(111), 175 );
	   
		}
		else if ( State_MouseOver & state ) {
		    c1 = alphaBlendColors( c1, option->palette.highlight().color().lighter(130), 211 );
		    c2 = alphaBlendColors( c2, option->palette.highlight().color().lighter(130), 211 );
		}
		
		pmkey.sprintf( "cc:%x-%x-%x-%x-%s", c1.rgb(), c2.rgb(), rw, rh,  "radiobutton" );
		
		if ( !UsePixmapCache || !QPixmapCache::find(pmkey, pixmap) ) {
		    pixmap.fill( Qt::transparent );
		    QRect pr = pixmap.rect();
		    QPainter pp( &pixmap );
		    pp.setRenderHint( QPainter::Antialiasing, true );
		    
		    QRadialGradient radialGrad;
		    radialGrad.setCenter( pr.center().x(), pr.center().y() );
		    radialGrad.setFocalPoint( pr.center().x(), pr.center().y() );
		    radialGrad.setRadius( pr.width() / 2 );
		    radialGrad.setColorAt( 0.0, c1 );
		    radialGrad.setColorAt( 0.9, c2.lighter(121) );
		    radialGrad.setColorAt( 1.0, c2.darker(111) );
		    pp.setPen( option->palette.color(QPalette::Mid) );
		    pp.setBrush( radialGrad );
		    pp.drawEllipse( pr );
		    pp.end();
		     
		    if ( UsePixmapCache )
			QPixmapCache::insert( pmkey, pixmap );
		}
		
		painter->drawPixmap( option->rect.adjusted(1, 1, -1, -1), pixmap );
	    }
	    else {
		painter->setPen( option->palette.color(QPalette::Mid) );
		painter->setBrush( option->palette.midlight() );
		painter->drawEllipse( option->rect.adjusted(1, 1, -1, -1) );
	    }
	    
	    if ( State_On & state ) {
		painter->setPen( Qt::NoPen );
		painter->setBrush( option->palette.buttonText() );
		painter->drawEllipse( left +  6, top + 6,  width - 12, height - 12 );
	    }
		 
	    painter->restore();
	    break;
	}
    case PE_IndicatorDockWidgetResizeHandle:
	{
	    QPen oldPen = painter->pen();
	    painter->fillRect( option->rect, painter->background() );
	    	    
	    if ( width < height ) {	
		int  centerY= option->rect.center().y();
		
		for ( int i = centerY - 6; i <  centerY + 6; i += 4 )
		    drawBoldPoint( painter, option->palette.color(QPalette::Base), 
				   option->palette.color(QPalette::Mid), left + 1, i );
	    }
	    else {
		int centerX = option->rect.center().x();
		
		for ( int i = centerX - 6; i <  centerX + 6; i += 4 )
		    drawBoldPoint( painter, option->palette.color(QPalette::Base), 
				   option->palette.color(QPalette::Mid), i, top + 1 );
	    }
	    
	    painter->setPen( oldPen );
	    break;
	}
    case PE_FrameTabWidget:
	{
	    QPen oldPen = painter->pen();
	    painter->setBrush( Qt::NoBrush );
	    painter->setPen( option->palette.color(QPalette::Dark)) ;
	    painter->drawRect( option->rect.adjusted(0, 0, -1, -1) );
	    painter->setPen( option->palette.color(QPalette::Midlight) );
	    painter->drawRect( option->rect.adjusted(1, 1, -2, -2) );
	    painter->setPen( option->palette.color(QPalette::Mid) ) ;
	    
	    if ( widget ) {
		if ( const QTabWidget *tw = qobject_cast<const QTabWidget *>(widget) ) {
		    
		    switch( tw->tabPosition() ) {
		    case QTabWidget::North:
			{
			    painter->drawLine( left + 1, bottom - 1, right - 1, bottom - 1 );
			    break;
			}
		    case QTabWidget::South:
			{
			    painter->drawLine( left + 1, top + 1, right - 1, top + 1 );
			    break;
			}
		    case QTabWidget::West:
			{
			    painter->drawLine( right - 1, top + 1, right - 1, bottom - 1 );
			    break;
			}
		    case QTabWidget::East:
			{
			    painter->drawLine( left + 1, top + 1, left + 1, bottom - 1 );
			    break;
			}
		    }
		}
	    }
	   
	    painter->setPen( oldPen );
	    break;
	}
    case PE_FrameWindow:
    case PE_Widget:
    case PE_FrameDockWidget:
	{
	    QPen oldPen = painter->pen();
	    painter->setPen( option->palette.color(QPalette::Midlight) );
	    drawContour( painter, left, top, right, bottom );
	    painter->setPen( option->palette.color(QPalette::Mid) );
	    drawContour( painter, left + 1, top + 1, right - 1, bottom - 1 );
	    painter->setPen( oldPen );
	    break;
	}
    case PE_FrameGroupBox:
	{
	    if ( const QStyleOptionFrameV2 *frame2 = qstyleoption_cast<const QStyleOptionFrameV2 *>(option) ) {
		
		if ( QStyleOptionFrameV2::Flat & frame2->features ) {
		    drawLinearGrad( painter, QRect(left, top, width, 2), option->palette.color(QPalette::Mid), painter->background().color() );
		    break;
		}
	    }
	}
    case PE_Frame:
	{
	    if ( const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option) ) {
		QWidget *parent = (widget ? widget->parentWidget() : NULL);
		bool isGroupbox = (PE_FrameGroupBox == element);
		
		if ( !isGroupbox ) 
		    painter->save();
		
		if ( parent && parent->inherits( "QComboBox" ) ) {
		    painter->setBrush( Qt::NoBrush );
		    painter->setPen( frame->palette.mid().color() );
		    painter->drawRect( frame->rect.adjusted(0, 0, -1, -1) );
		}
		else {
		    painter->setRenderHint( QPainter::Antialiasing, true );
		    bool out = false, lcd = false;
		    State state = frame->state;
		    int lw = frame->lineWidth;
		    int r = 5;
		   
		    if ( width < 12 || height < 12 ) 
			r = 2;
		
		    if ( !isGroupbox ) {
			if ( qobject_cast<const QAbstractScrollArea *>(widget) ) {
			    out = true;
			}
			else if ( qobject_cast<const QLCDNumber *>(widget) && !((State_Sunken | State_Raised) & state) ) {
			    lcd = true;
			    out = true;
			}
			else if ( parent && parent->inherits("QFrame") ) {
			    out = true;
			}
			else {
			    while ( parent ) {
				if ( parent->inherits("KColorDialog") ) {
				    out = true;
				    break;
				}	
				else
				    parent = parent->parentWidget();
			    }
			}
		    }
		    else if ( parent && parent->inherits("QGroupBox") )
			out = true;
		
		    if ( !isGroupbox && !lcd )  { 
			painter->setBrush( Qt::NoBrush );
			painter->setPen( QPen(frame->palette.window(), 2) );
			painter->drawRect( frame->rect.adjusted(0, 0, -1, -1) );
		    }
		    
		    QPainterPath rc = roundRectPath( frame->rect, r );
		    QRegion region = painter->clipRegion();
		    painter->setClipPath( rc, Qt::IntersectClip );
		    
		    if ( lcd ) 
			painter->fillRect( frame->rect.adjusted(1, 1, -1, -1), frame->palette.color(QPalette::Base ) );
		    
		    if ( (!out && ((State_Sunken | State_Raised) & state)) || isGroupbox ) {
			int hh = (height - 2) / 2, ww = (width - 2) / 2;
			
			if ( State_Sunken & state ) {
			    QColor win =frame->palette.color( QPalette::Window ).darker( 110 );
			    win.setAlphaF( 0.2 );
			    painter->fillRect( frame->rect.adjusted(1, 1, -1, -1), win );
			}
			else {
			    QColor c1 = frame->palette.color( QPalette::Window );
			    QColor c2 = frame->palette.color( QPalette::Midlight );
			    QLinearGradient lg;
			    lg.setStart( left + lw, top + lw );
			    lg.setFinalStop( left + lw, bottom - lw );
			    
			    if ( isGroupbox  ) {
				lg.setColorAt( 0.0, c1 );
				lg.setColorAt( 0.5, c1.lighter( 104 ) );
				lg.setColorAt( 1.0, c2 );
			    }
			    else {
				lg.setColorAt( 0.0, c2 );
				lg.setColorAt( 0.5, c1.lighter(104) );
				lg.setColorAt( 1.0, c1 );
			    }
			   
			    painter->fillPath( rc, lg );
			}
			
			if ( (State_Raised & state) || isGroupbox ) {
			    drawLinearGrad( painter, QRect(left + 1, top + 1, 2, hh), frame->palette.color(QPalette::Midlight), 
					    frame->palette.color(QPalette::Base), false );  
			    drawLinearGrad( painter, QRect(left + 1, top + hh, 2, height - hh), frame->palette.color(QPalette::Base), 
					    frame->palette.color(QPalette::Midlight), false );  
			    drawLinearGrad( painter, QRect(left + 1, top + 1, ww, 2), frame->palette.color(QPalette::Midlight), 
					    frame->palette.color(QPalette::Base) ); 
			    drawLinearGrad( painter, QRect(left + ww, top + 1, width - ww, 2), frame->palette.color(QPalette::Base), 
					    frame->palette.color(QPalette::Midlight) ); 
			}
			
			if ( (State_Sunken & state) || isGroupbox) {
			    drawLinearGrad( painter, QRect(right - 2, top + 1, 2, hh), frame->palette.color(QPalette::Midlight), 
					    frame->palette.color(QPalette::Base), false );  
			    drawLinearGrad( painter, QRect(right - 2, top + hh, 2, height - hh), frame->palette.color(QPalette::Base), 
					    frame->palette.color(QPalette::Midlight), false );  
			    drawLinearGrad( painter, QRect(left + 1, bottom - 2, ww, 2), frame->palette.color(QPalette::Midlight), 
					    frame->palette.color(QPalette::Base) ); 
			    drawLinearGrad( painter, QRect(left + ww, bottom - 2, width - ww, 2), frame->palette.color(QPalette::Base), 
					   frame->palette.color(QPalette::Midlight) ); 
			}
		    }
		    else {
			QColor dark = frame->palette.color( (State_HasFocus & state ? QPalette::Highlight : QPalette::Dark) );
			
			for ( int i = 0; i < 4; ++i ) {
			    dark.setAlphaF( (4 - i) * 0.2 );
			    painter->setPen( dark );
			    painter->drawLine( left, top + i, right, top + i );
			}
		    }
		    
		    painter->setPen( frame->palette.color(QPalette::Mid) );
		    painter->setBrush( Qt::NoBrush );
		    
		    if ( isGroupbox ) {
			painter->setClipRegion( region );
			painter->drawPath( rc );
		    }
		    else {
			painter->setClipping( false );
			painter->drawPath( roundRectPathF((QRectF(option->rect)).adjusted(0.5, 0.5, -0.5, -0.5), r) );
		    }
		}
		
		if ( isGroupbox )
		    painter->setRenderHint( QPainter::Antialiasing, false );
		else
		    painter->restore();
	    }
	    break;
	}
    case PE_FrameLineEdit:
	{
	    QPen oldPen = painter->pen();
	    QColor border;
	    
	    if ( State_Enabled ) {
		if ( State_HasFocus & option->state )
		    border = option->palette.highlight().color();
		else {
		    border = option->palette.dark().color();
		    border.setAlphaF( 0.7 );
		}
	    }
	    else
		border =  option->palette.mid().color();
		
	    if ( isRnd(width, height) ) {	
		painter->setPen( border );
		painter->setRenderHint( QPainter::Antialiasing, true );
		painter->setBrush( Qt::NoBrush );
		painter->drawPath( roundRectPathF((QRectF(option->rect)).adjusted(0.5, 0.5, -0.5, -0.5), 4) );
		painter->setRenderHint( QPainter::Antialiasing, false );
	    } 	
	    else {
		painter->setPen( option->palette.color(QPalette::Midlight) );
		drawContour( painter, left + 1, top + 1, right - 1, bottom - 1 );	
		painter->setPen( border );
		drawContour( painter, left, top, right, bottom );
	    }
	    
	    painter->setPen( oldPen );
	    break;
	}
    case PE_FrameMenu:
	{
	    painter->setBrush( Qt::NoBrush );
	    painter->setRenderHint( QPainter::Antialiasing, true );
	    painter->setPen( QPen(option->palette.color(QPalette::Window), 2) );
	    painter->drawPath( roundRectPath(option->rect, 4) );
	    painter->setPen( option->palette.color(QPalette::Mid) );
	    painter->drawPath( roundRectPathF((QRectF(option->rect)).adjusted(1, 1, -1, -1), 4) );
	    painter->setRenderHint( QPainter::Antialiasing, false );
//	    drawContour( painter, left + 1, top + 1, right - 1, bottom - 1 );	
//	    painter->setPen( option->palette.color(QPalette::Mid) );
//	    drawContour( painter, left, top, right, bottom );
	    
	    break;
	}
    case PE_FrameTabBarBase:
	{
	    if ( const QStyleOptionTabBarBase *base = qstyleoption_cast<const QStyleOptionTabBarBase *>(option) ) {
		QRegion clip( base->rect );
		bool reverse = ( Qt::RightToLeft == option->direction );
		painter->save();
		painter->fillRect( base->rect.adjusted((reverse ? 2 : 1), 0, (reverse ? -2 : -1), 0), option->palette.window() );
		
		if ( QTabBar::RoundedNorth == base->shape || QTabBar::TriangularNorth == base->shape ||
		     QTabBar::RoundedSouth == base->shape || QTabBar::TriangularSouth == base->shape ) {  
		    if ( !reverse )
			clip -= base->selectedTabRect.adjusted( 0, 0, -4, 0 );
		    else
			clip -= base->selectedTabRect.adjusted( 6, 0, 0, 0 );
		}
		else {
		    clip -= base->selectedTabRect.adjusted( 0, 0, 0, -4 );
		}

		painter->setClipRegion( clip );
		painter->setPen( base->palette.color(QPalette::Dark) );
		
		switch( base->shape ) {
		case QTabWidget::North:
		    {
			painter->drawLine( left, top, right, top );
			break;
		    }
		case QTabWidget::South:
		    {
			painter->drawLine( left, bottom, right, bottom );
			break;
		    }
		case QTabWidget::West:
		    {
			painter->drawLine( left, top, left, bottom );
			break;
		    }
		case QTabWidget::East:
		    {
			painter->drawLine( right, top, right, bottom );
			break;
		    }
		default: break;
		}
		
		painter->restore();
	    }
	    break;
	}
    case PE_PanelStatusBar:
	{
	    drawSurface( painter, QRect(left, top, width, height / 4), 
			 option->palette.color(QPalette::Normal, QPalette::Base), 
			 option->palette.color(QPalette::Normal, QPalette::Window) );
	    break;
	    
	}
    case PE_IndicatorToolBarSeparator:
    case PE_Q3Separator:
    case PE_Q3DockWindowSeparator:
	{
	    QPen oldPen = painter->pen();
	    painter->setPen( option->palette.color(QPalette::Mid) );
	    
	    if ( width > height )
		painter->drawLine( left + 2, top, right - 2, top );
	    else
		painter->drawLine( left, top + 2, left, bottom - 2 );
	    
	    painter->setPen( option->palette.color(QPalette::Base) );
	    
	    if ( width > height )
		painter->drawLine( left + 2, top + 1, right - 2, top + 1 );
	    else
		painter->drawLine( left + 1, top + 2, left + 1, bottom - 2 );
	    
	    painter->setPen( oldPen );
	    break;
	}
    case PE_FrameStatusBar:
	{
	    QPen oldPen = painter->pen();
	    painter->setPen( option->palette.color(QPalette::Mid) );
	    painter->drawLine( left, top, right, top );
	    painter->drawLine( left, bottom, right, bottom );
	    painter->setPen( option->palette.color(QPalette::Midlight) );
	    painter->drawLine( left, top + 1, right, top + 1 );
	    painter->drawLine( left, bottom - 1, right, bottom - 1 );
	    painter->setPen( oldPen );
	    break;
	}
    case PE_PanelMenuBar:
    case PE_PanelToolBar:
	{
	    QPen oldPen = painter->pen();
	    painter->setPen( option->palette.color(QPalette::Mid) );
	    painter->drawLine( left, bottom, right, bottom );
	    painter->setPen( option->palette.color(QPalette::Base) );
	    painter->drawLine( left, top, right, top );
	    painter->setPen( oldPen );
	    break;
	}
    case PE_IndicatorHeaderArrow:
	{
	    if ( const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option) ) {
		PrimitiveElement pe;
		
		if ( QStyleOptionHeader::SortUp & header->sortIndicator )
		    pe = PE_IndicatorArrowUp;
		else if ( QStyleOptionHeader::SortDown & header->sortIndicator )
		    pe = PE_IndicatorArrowDown;
		else
		    break;
		
		QRect aRect = header->rect;
		aRect.adjust( 0, 2, 0, 0 );
		drawAnyArrow( pe, painter, QRect(aRect.center().x() - 5, aRect.center().y() - 4, 11, 9), option->palette.windowText(), true );
	    }
	    break;
	}
    case PE_IndicatorMenuCheckMark:
	{
	    drawCheckMark( painter, option->rect, option->palette.windowText() );
	    break;
	}
    case PE_IndicatorBranch:
	{
	    if ( State_Children & option->state ) {
		painter->save();
		painter->setRenderHint( QPainter::Antialiasing, true );
		QPoint cp = option->rect.center();
		QRect arrow = QRect( cp.x() - 4, cp.y() - 5, 9, 11 );
		QMatrix matrix( arrow.width() / 2, 0, 0, arrow.height() / 2, 0, 0 );
		QPainterPath aPath;
		aPath.moveTo( -0.8, -1.0 );
		aPath.lineTo( 0.8, 0 );
		aPath.lineTo( -0.8, 1.0 );
		aPath.closeSubpath();
		painter->translate( cp );
		
		if ( State_Selected & option->state ) {
		    painter->setBrush( option->palette.color(QPalette::Base) );
		    painter->setPen( Qt::NoPen );
		}
		
		if ( State_Open & option->state ) {
		    if ( !(State_Selected & option->state) ) {
			painter->setBrush( option->palette.color(QPalette::Text) );
			painter->setPen( Qt::NoPen );
		    }
		    
		    painter->rotate( 90.0 );
		}
		else if ( !(State_Selected & option->state) ) {
		    painter->setPen( option->palette.color(QPalette::Text) );
		    painter->setBrush( option->palette.color(QPalette::Base) );
		}
		
		painter->drawPath( matrix.map(aPath) );
		painter->restore();
	    }
	    
	    break;
	}
    case PE_PanelTipLabel:
	{
	    painter->setPen( option->palette.color(QPalette::Mid) );
	    drawContour( painter, left, top, right, bottom );
	    painter->fillRect( left + 1, top + 1, width - 2, height - 2, option->palette.toolTipBase() );
	    break;
	}
    case PE_PanelItemViewItem:
	{
	    if ( const QStyleOptionViewItemV2 *v2 = qstyleoption_cast<const QStyleOptionViewItemV2 *>(option) ) {
		if ( QStyleOptionViewItemV2::Alternate & v2->features )
		    painter->fillRect( v2->rect, v2->palette.alternateBase() );
	    }
	    else
		painter->fillRect( option->rect, v2->palette.base() );
	    
	    
	    if ( State_Selected & option->state ) {
		QColor highlight = option->palette.color( QPalette::Highlight ); 
		QLinearGradient linearGrad;
		linearGrad.setStart( left, top );
		linearGrad.setFinalStop( left, bottom );
		linearGrad.setColorAt( 0.0, highlight.lighter(102) );
		linearGrad.setColorAt( 0.5, highlight.lighter(108)  );
		linearGrad.setColorAt( 1.0, highlight.lighter(114) );
		painter->fillRect( option->rect, linearGrad );
	    }
	    else if ( isEnabled && (State_MouseOver & option->state) ) {
		QColor highlight = option->palette.color( QPalette::Active, QPalette::Highlight );
		highlight.setAlpha( 40 );
		painter->fillRect( option->rect, highlight );
	    }
	    
	    break;
	}
    case PE_IndicatorToolBarHandle:
	{
	    QPen oldPen = painter->pen();
	    painter->fillRect( option->rect, option->palette.background() );
	    
	    if ( width > height ) {	
		int startY = option->rect.center().y() - 2;
		
		for ( int i = left + 2; i <=  right - 2; i += 4 )
		    drawBoldPoint( painter, option->palette.color(QPalette::Base), 
				   option->palette.color(QPalette::Mid), i, startY );
	    }
	    else {
		int startX = option->rect.center().x() - 2;
		
		for ( int i = top + 2; i <= bottom - 2; i += 4 )
		    drawBoldPoint( painter, option->palette.color(QPalette::Base), 
				   option->palette.color(QPalette::Mid), startX, i );
	    }
	    
	    painter->setPen( oldPen );
	    break;
	}
    default:
	{
	    ParentStyle::drawPrimitive( element, option, painter, widget );
	    break;
	}
    }
}

void L4uStyle2::drawControl( ControlElement control, const QStyleOption *option, 
			     QPainter *painter, const QWidget *widget ) const
{
    if ( !option ) return;
    
    int left, right, top, bottom, width, height;
    bool isEnabled = ( QStyle::State_Enabled & option->state );
    bool reverse = ( Qt::RightToLeft == option->direction );
    option->rect.getCoords( &left, &top, &right, &bottom );
    width = option->rect.width(); 
    height = option->rect.height();
    
    switch( control ) {
    case CE_PushButton:
	{
	    if ( const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option) ) {
		drawControl( CE_PushButtonBevel, button, painter, widget );
		
		QStyleOptionButton content = *button;
		content.state &= ~State_HasFocus;
		content.rect = subElementRect( SE_PushButtonContents, button, widget );
		drawControl( CE_PushButtonLabel, &content, painter, widget );
	    }
	    break;
	}
    case CE_PushButtonBevel:
	{
	    if ( const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option) ) {
		QStyleOptionButton butOpt = *button;
		bool horz = true;
		
		if ( widget && widget->parentWidget() ) {	
		    if ( QDockWidget *dw = qobject_cast<QDockWidget *>(widget->parentWidget()) ) {
			if ( QDockWidget::DockWidgetVerticalTitleBar & dw->features() )
			    horz = false;
		    }
		    else if ( QToolBar *tb = qobject_cast<QToolBar *>(widget->parentWidget()) ) {
			if ( Qt::Vertical == tb->orientation() )
			    horz = false;
		    }
		}
		
		if ( horz )
		    butOpt.state |= State_Horizontal;
		else
		    butOpt.state &= ~State_Horizontal;
		
		butOpt.rect = button->rect.adjusted( 0, 1, 0, -1 ); 
		drawPrimitive( PE_PanelButtonCommand, &butOpt, painter, widget );
		  
		if ( (QStyleOptionButton::DefaultButton & button->features) 
		    && !(State_HasFocus & butOpt.state) && !((State_Sunken | State_On) & butOpt.state) ) {
		    drawPrimitive( PE_FrameDefaultButton, &butOpt, painter, widget );
		}
		
		if ( butOpt.features &  QStyleOptionButton::HasMenu ) {
		    drawAnyArrow( PE_IndicatorArrowDown, painter, QRect(right - pixelMetric(PM_MenuButtonIndicator, button, widget), 
									top + (height - 7) / 2, 9, 7),  butOpt.palette.buttonText(), true );
		}
		
	    }
	    break;
	}
    case CE_DockWidgetTitle:
	{
	    painter->save();	
	    
	    if ( const QStyleOptionDockWidget *dw = qstyleoption_cast<const QStyleOptionDockWidget *>(option) ) {
		const QStyleOptionDockWidgetV2 *dw2 = qstyleoption_cast<const QStyleOptionDockWidgetV2 *>( dw );
		QRect subRect = subElementRect( SE_DockWidgetTitleBarText, dw, widget );
		bool horz = true;
		
		if ( dw2 && dw2->verticalTitleBar )  horz = false;
		
		painter->setRenderHint( QPainter::Antialiasing, true );
		painter->fillPath( roundTabPath(dw->rect, horz ? QTabBar::RoundedNorth : QTabBar::RoundedWest), dw->palette.mid() );
		painter->setRenderHint( QPainter::Antialiasing, false );
		
		if ( !horz ) {
		    QTransform m;
		    QPointF c = subRect.center();
		    m.translate( c.x(), c.y() );
		    m.rotate( -90.0 );
		    m.translate( -c.x(), -c.y() );
		    painter->setTransform( m, true );
		    subRect = m.mapRect( subRect );
		    horz = false;
		}
		
		if ( !dw->title.isEmpty() ) {
		    QFont font = painter->font();
		    font.setBold( true );
		    painter->setFont( font );
		    painter->setPen( dw->palette.color(QPalette::WindowText) );
		    painter->drawText( subRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextShowMnemonic, 
				       painter->fontMetrics().elidedText(dw->title, Qt::ElideRight, subRect.width()) );
		}
	    }
	    
	    painter->restore();
	    break;
	}
    case CE_Splitter:
	{
	    drawPrimitive( PE_IndicatorDockWidgetResizeHandle, option, painter, widget );
	    break;
	}
    case CE_Q3DockWindowEmptyArea:
	{
	    painter->fillRect( option->rect, painter->background() );
	    break;
	}
    case CE_TabBarTab:
	{
	    if ( const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option) ) {
		QStyleOptionTab tabOpt = *tab;
		
		if ( QTabBar::RoundedNorth == tabOpt.shape || QTabBar::TriangularNorth == tabOpt.shape ||
		     QTabBar::RoundedSouth == tabOpt.shape || QTabBar::TriangularSouth == tabOpt.shape ) { 
		    if ( !reverse )
			tabOpt.rect.adjust( 0, 0, -4, 0 );
		    else
			tabOpt.rect.adjust( 4, 0, 0, 0 );
		}
		else
		    tabOpt.rect.adjust( 0, 0, 0, -4 );
		
		drawControl(CE_TabBarTabShape, &tabOpt, painter, widget);
		drawControl(CE_TabBarTabLabel, &tabOpt, painter, widget);
	    }
	    break;
	}
    case CE_TabBarTabShape:
	{
	    if ( const QStyleOptionTab *tabOpt = qstyleoption_cast<const QStyleOptionTab *>(option) ) {
		painter->save();
		int how = DrawNone;
		QTabBar::Shape shape = tabOpt->shape;
		State state = tabOpt->state;
		QColor c1 = tabOpt->palette.color( QPalette::Window ).lighter( 105 );
		QColor c2 = tabOpt->palette.color( QPalette::Window ).darker( 111 );
		QColor c3 = tabOpt->palette.color( QPalette::Window );
		QColor dark = tabOpt->palette.color( QPalette::Dark );
			
		if ( (isEnabled && (state & State_MouseOver)) && !(State_Selected & state) ) {
		    c1 = alphaBlendColors( c1,  tabOpt->palette.color(QPalette::Highlight).lighter(150), 200 );
		    c2 = alphaBlendColors( c2,  tabOpt->palette.color(QPalette::Highlight).lighter(150), 200 );
		}
		
		switch( shape ) {
		case QTabBar::RoundedNorth:
		case QTabBar::TriangularNorth:
		    {
			how = DrawLeft | DrawTop | DrawRight | RoundUpperLeft | RoundUpperRight;
			QRect tab = tabOpt->rect.adjusted( 0, 1, 0, -1 );
			int hh = (tab.height() - 2) / 2;
			
			if ( QTabBar::RoundedNorth == shape )
			    painter->setClipPath( roundTabPath( tabOpt->rect.adjusted(0, 1, 0, 1), shape ) );
			
			if ( State_Selected & state ) {
			    if ( isEnabled ) {
				c1 = alphaBlendColors( c1,  tabOpt->palette.color(QPalette::Highlight).lighter(130), 200 );
				drawSurface( painter, QRect(tab.left() + (reverse ? 3 : 1), tab.top() + 1, tab.width() - 2, hh), c1, c3 );
				painter->fillRect( tab.left() + (reverse ? 3 : 1), tab.top() + 1 + hh, tab.width() - 2, tab.height() - hh, c3 );
			    }
			    else
				painter->fillRect( tab.left() + (reverse ? 3 : 1), tab.top() + 1, tab.width() - 2, tab.height(), c1 );
			    
			    if ( QTabBar::RoundedNorth != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left() + (reverse ? 2 : 0), tab.top(), tab.right(), tab.bottom(), how );
			    }
			    
			    if ( reverse && (QStyleOptionTab::Beginning == tabOpt->position || QStyleOptionTab::OnlyOneTab == tabOpt->position) 
				&& !(QStyleOptionTab::LeftCornerWidget & tabOpt->cornerWidgets) )
				painter->drawPoint( right, bottom );
			}
			else {
			    if ( isEnabled ) {
				drawSurface( painter, QRect(tab.left() + (reverse ? 4 : 2), tab.top() + 2, tab.width() - 4, tab.height() - 3),  c1, c2 );
			    }
			    else 
				painter->fillRect( tab.left() + (reverse ? 4 : 2), tab.top() + 2, tab.width() - 4, tab.height() - 3, c3.darker(107) );
			    
			    painter->setPen( c1 );
			    drawContour( painter, tab.left() + (reverse ? 3 : 1), tab.top() + 1, tab.right() - 1 , tab.bottom() - 1, how );	
			    
			    if ( QTabBar::RoundedNorth != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left() + (reverse ? 2 : 0), tab.top(), tab.right() , tab.bottom(), how | DrawBottom );
			    }
			}	
			
			if ( QTabBar::RoundedNorth == shape ) {
			    painter->setClipping( false );
			    painter->setRenderHint( QPainter::Antialiasing, true );
			    painter->setBrush( Qt::NoBrush );
			    painter->setPen( dark);
			    painter->drawPath( roundTabPathF((QRectF(tab)).adjusted(reverse ? 1.5 : 0.5, 
										    reverse ? 1.5 : 0.5, -0.5, -0.5), shape, false) );
			}
			   
			break;
		    }
		case QTabBar::RoundedSouth:
		case QTabBar::TriangularSouth:
		    {
			how = DrawLeft  | DrawBottom | DrawRight | RoundLowerLeft | RoundLowerRight;
			QRect tab = tabOpt->rect.adjusted( 0, 1, 0, -1 );
			int hh = (tab.height() - 2) / 2;
			
			if ( QTabBar::RoundedSouth == shape )
			    painter->setClipPath( roundTabPath(tabOpt->rect.adjusted( 0, -1, 0, -1 ), shape) );
			
			if ( State_Selected & state ) {
			    if ( isEnabled ) {		
				c1 = alphaBlendColors( c1,  tabOpt->palette.color(QPalette::Highlight).lighter(130), 200 );
				painter->fillRect( tab.left() + (reverse ? 3 : 1), tab.top() - 1, tab.width() - 2, hh, c3 );
				drawSurface( painter, QRect(tab.left() + (reverse ? 3 : 1), tab.top() - 1 + hh, 
							    tab.width() - 2, tab.height() - hh), c3, c1 );
			    }
			    else
				painter->fillRect( tab.left() + (reverse ? 3 : 1), tab.top() - 1, tab.width() - 2, tab.height(), c1 );
			    
			    if ( QTabBar::RoundedSouth != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left() + (reverse ? 2 : 0), tab.top(), tab.right(), tab.bottom(), how );
			    }
			    
			    if ( reverse && (QStyleOptionTab::Beginning == tabOpt->position || QStyleOptionTab::OnlyOneTab == tabOpt->position) 
				&& !(QStyleOptionTab::LeftCornerWidget & tabOpt->cornerWidgets) )
				painter->drawPoint( right, top );
			}
			else {
			    if ( isEnabled ) {			
				drawSurface( painter, QRect(tab.left() + (reverse ? 4 : 2), tab.top() + 1, tab.width() - 4, tab.height() - 3),  c2, c1 );
			    }
			    else
				painter->fillRect( tab.left() + (reverse ? 4 : 2), tab.top() + 1, tab.width() - 4, tab.height() - 3, c3.darker(107) );
			   
			    painter->setPen( c1 );
			    drawContour( painter, tab.left() + (reverse ? 3 : 1), tab.top() + 1, tab.right() - 1 , tab.bottom() - 1, how );
			    
			    if ( QTabBar::RoundedSouth != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left() + (reverse ? 2 : 0), tab.top(), tab.right() , tab.bottom(), how | DrawTop );		
			    }
			}	
			
			if ( QTabBar::RoundedSouth == shape ) {
			    painter->setClipping( false );
			    painter->setRenderHint( QPainter::Antialiasing, true );
			    painter->setBrush( Qt::NoBrush );
			    painter->setPen( dark);
			    painter->drawPath( roundTabPathF((QRectF(tab)).adjusted(reverse ? 1.5 : 0.5, 
										    reverse ? 1.5 : 0.5, -0.5, -0.5), shape, false) );
			}
			
			break;
		    }
		case QTabBar::RoundedWest:
		case QTabBar::TriangularWest:
		    {
			how = DrawLeft | DrawTop | DrawBottom | RoundUpperLeft | RoundLowerLeft;
			QRect tab = tabOpt->rect.adjusted( 1, 0, -1, 0 );
			int ww = (tab.width() - 2) / 2;
			
			if ( QTabBar::RoundedWest == shape )
			    painter->setClipPath( roundTabPath(tabOpt->rect.adjusted(1, 0, 1, 0), shape) );
			
			if ( State_Selected & state ) {
			    if ( isEnabled ) {
				c1 = alphaBlendColors( c1,  tabOpt->palette.color(QPalette::Highlight).lighter(130), 200 );
				drawSurface( painter, QRect(tab.left() + 1, tab.top() + 1, ww, tab.height() - 2), c1, c3, false );
				painter->fillRect( tab.left() + 1 + ww, tab.top() + 1, tab.width() - ww , tab.height() - 2, c3 );
			    }
			    else
				painter->fillRect( tab.left() + 1, tab.top() + 1, tab.width(), tab.height() - 2, c1 );
			    
			    if ( QTabBar::RoundedWest != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left(), tab.top(), tab.right(), tab.bottom(), how );
			    }
			}
			else {
			    if ( isEnabled ) {
				drawSurface( painter, QRect(tab.left() + 2, tab.top() + 2, tab.width() - 3, tab.height() - 4),  c1, c2, false );
			    }
			    else
				painter->fillRect( tab.left() + 2, tab.top() + 2, tab.width() - 3, tab.height() - 4, c3.darker(107) );
			    
			    painter->setPen( c1 );
			    drawContour( painter, tab.left() + 1, tab.top() + 1, tab.right() - 1, tab.bottom() - 1, how );
			
			    if ( QTabBar::RoundedWest != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left(), tab.top(), tab.right() , tab.bottom(), how | DrawRight );	
			    }
			}
			
			if ( QTabBar::RoundedWest == shape ) {
			    painter->setClipping( false );
			    painter->setRenderHint( QPainter::Antialiasing, true );
			    painter->setBrush( Qt::NoBrush );
			    painter->setPen( dark);
			    painter->drawPath( roundTabPathF((QRectF(tab)).adjusted(0.5, 0.5, -0.5, -0.5), shape, false) );
			}
			break; 
		    }
		case QTabBar::RoundedEast:
		case QTabBar::TriangularEast:
		    {	
			how =  DrawTop | DrawRight | DrawBottom | RoundUpperRight | RoundLowerRight;
			QRect tab = tabOpt->rect.adjusted( 1, 0, -1, 0 );
			int ww = (tab.width() - 2) / 2;
			
			if ( QTabBar::RoundedEast == shape )
			    painter->setClipPath( roundTabPath(tabOpt->rect.adjusted(-1, 0, -1, 0), shape)  );
			
			if ( State_Selected & state ) {
			    if ( isEnabled ) {
				c1 = alphaBlendColors( c1,  tabOpt->palette.color(QPalette::Highlight).lighter(130), 200 );
				painter->fillRect( tab.left() - 1, tab.top() + 1, ww, tab.height() - 2,  c3 );
				drawSurface( painter, QRect(tab.left() - 1 + ww, tab.top() + 1, tab.width() - ww, tab.height() - 2), c3, c1, false );
			    }
			    else
				painter->fillRect( tab.left() - 1, tab.top() + 1, tab.width(), tab.height() - 2, c1 );
			    
			    if ( QTabBar::RoundedEast != shape ) {
				painter->setPen( dark );
				drawContour( painter, tab.left(), tab.top(), tab.right(), tab.bottom(), how );
			    }
			}
			else {
			    if ( isEnabled ) {	
				drawSurface( painter, QRect(tab.left() + 1, tab.top() + 2, tab.width()  - 3, tab.height() - 4), c2, c1, false );
			    }
			    else
				painter->fillRect( tab.left() + 1, tab.top() + 2, tab.width() - 3, tab.height() - 4, c3.darker(107) );
			    
			    painter->setPen( c1 );
			    drawContour( painter, tab.left() + 1, tab.top() + 1, tab.right() - 1, tab.bottom() - 1, how  );
			    
			    if ( QTabBar::RoundedEast != shape ) {	
				painter->setPen( dark );
				drawContour( painter, tab.left(), tab.top(), tab.right() , tab.bottom(), how | DrawLeft );
			    }
			}
			
			if ( QTabBar::RoundedEast == shape ) {
			    painter->setClipping( false );
			    painter->setRenderHint( QPainter::Antialiasing, true );
			    painter->setBrush( Qt::NoBrush );
			    painter->setPen( dark);
			    painter->drawPath( roundTabPathF((QRectF(tab)).adjusted(0.5, 0.5, -0.5, -0.5), shape, false) );
			}	
			
			break; 
		    }
		}
		painter->restore();
	    }
	    break;
	}
  case CE_ProgressBarGroove:
	{
	    QPen oldPen = painter->pen();
	    const QStyleOptionProgressBar *pbar = qstyleoption_cast<const QStyleOptionProgressBar *>( option ); 
	    
	    if ( pbar && (pbar->minimum == 0 && pbar->maximum == 0) ) {
		painter->fillRect( pbar->rect.adjusted(1, 1, -1, -1), QColor(Qt::black) );
		painter->setPen( option->palette.color(QPalette::Dark) );
		drawContour( painter, left, top, right, bottom );
	    }
	    else {
		if ( width > 4 && height > 4 ) {
		    if ( isEnabled ) {
			const QStyleOptionProgressBarV2 *pbar2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>( option );
			
			if ( widget && widget->inherits("StatusBarSpaceInfo") ) {
			    if ( pbar2 && (Qt::Vertical == pbar2->orientation) ) {
				int ww = (width - 2) / 2;
				
				drawSurface( painter, QRect(left + 1, top + 1, ww, height - 2), option->palette.color(QPalette::Midlight).lighter(105), 
					     option->palette.color(QPalette::Midlight).darker(107), false );
				painter->fillRect( left + ww + 1, top + 1, width - ww - 2, height - 2, option->palette.color(QPalette::Midlight).darker(111) );
				
			    }
			    else {
				int hh = (height - 2) / 2;
				
				drawSurface( painter, QRect(left + 1, top + 1, width - 2, hh), option->palette.color(QPalette::Midlight).lighter(105), 
					     option->palette.color(QPalette::Midlight).darker(107) );
				painter->fillRect( left + 1, top + hh + 1, width - 2, height - hh - 2, option->palette.color(QPalette::Midlight).darker(111) );
			    }
			}
			else {
			    if ( pbar2 && (Qt::Vertical == pbar2->orientation) ) {
				int h2 = (height - 2) / 2;
				drawSurface( painter, QRect(left + 1, top + 1, width - 2, h2), 
					     option->palette.color(QPalette::Base).darker(106), option->palette.color(QPalette::Base).darker(102) );
				drawSurface( painter, QRect(left + 1, top + h2 + 1, width - 2, height - h2 - 2),
					     option->palette.color(QPalette::Base).darker(102), option->palette.color(QPalette::Base).darker(106) );
			    }
			    else {
				int w2 = (width - 2) / 2;
				drawSurface( painter, QRect(left + 1, top + 1, w2, height - 2), 
					     option->palette.color(QPalette::Base).darker(106), 
					     option->palette.color(QPalette::Base).darker(102), false );
				drawSurface( painter, QRect(left + w2 + 1, top + 1, width - w2 - 2, height - 2),
					     option->palette.color(QPalette::Base).darker(102), 
					     option->palette.color(QPalette::Base).darker(106), false );
			    }
			}
		    }
		    else
			painter->fillRect( left + 1, top + 1, width - 2, height - 2,  
					   QBrush(option->palette.color(QPalette::Window).lighter(110)) );
		}
		
		painter->setPen( option->palette.color(QPalette::Mid) );
		drawContour( painter, left, top, right, bottom );
		painter->setPen( option->palette.color(QPalette::Midlight) );
		drawContour( painter, left + 1, top + 1, right - 1, bottom - 1, DrawLeft | DrawRight | DrawTop | DrawBottom );
	    }
	   
	    painter->setPen( oldPen );
	    break;
	}	
    case CE_ProgressBarLabel:
	{
	    if ( const QStyleOptionProgressBar *pbar = qstyleoption_cast<const QStyleOptionProgressBar *>(option) ) {
		if ( pbar->text.isEmpty() )
		    return;
		
		painter->save();
		
		if ( const QStyleOptionProgressBarV2 *pbar2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option) ) {
		    if ( pbar2->orientation == Qt::Vertical ) {
			painter->translate( pbar2->rect.center() );
			painter->rotate( (pbar2->bottomToTop ? -90 : 90) );
			painter->translate( -pbar2->rect.center() );
		    }
		}	
		
		QFont boldFont( painter->font() );
		boldFont.setBold( true );
		painter->setFont( boldFont );
		painter->setPen( pbar->palette.color(QPalette::Base) );
		painter->drawText( pbar->rect.adjusted(-1, 0, -1, 0), Qt::AlignCenter | Qt::AlignAbsolute, pbar->text );
		painter->drawText( pbar->rect.adjusted(1, 0, 1, 0), Qt::AlignCenter  | Qt::AlignAbsolute, pbar->text );
		painter->drawText( pbar->rect.adjusted(0, -1, 0, -1), Qt::AlignCenter  | Qt::AlignAbsolute, pbar->text );
		painter->drawText( pbar->rect.adjusted(0, 1, 0, 1), Qt::AlignCenter  | Qt::AlignAbsolute, pbar->text );
		painter->setPen( pbar->palette.color(QPalette::WindowText) );
		painter->drawText( pbar->rect, Qt::AlignCenter | Qt::AlignAbsolute, pbar->text) ;
		
		painter->restore();
	    }
	    break;
	}
    case CE_ProgressBarContents:
	{
	    if ( const QStyleOptionProgressBar *pbar = qstyleoption_cast<const QStyleOptionProgressBar *>(option) ) {
		if ( width > 4 && height > 4 ) {
		    painter->save();
		    bool vertical = false;
		    
		    if ( const QStyleOptionProgressBarV2 *pbar2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option) ) {
			if ( vertical = (pbar2->orientation == Qt::Vertical) ) {
			    QRect rect = QRect( left, top, height, width );
			    QTransform m;
			    m.translate( rect.height() - 1, 0 );
			    m.rotate( 90.0 );
			    painter->setTransform( m, true );
			    left = rect.left(), top = rect.top(), right = rect.right(), bottom = rect.bottom();
			    width = rect.width(), height = rect.height();
			}
		    }
		    
		    left  += 2, top += 2, right -= 2, bottom -= 2, width -= 4, height -= 4;
		    qint64 curVal = ( qMax<qint64>( pbar->progress, pbar->minimum ) -  qint64(pbar->minimum) );
		    qint64 maxVal = qint64( pbar->maximum ) - qint64( pbar->minimum );
		    
		    if ( widget && widget->inherits("StatusBarSpaceInfo") ) {
			QColor green = QColor( Qt::green );
			int barLen = int( curVal / qMax(double(1.0), double(maxVal)) * width );
			int hh = (height - 2) / 2;
			
			if ( (barLen > 2) && (hh > 2) ) {
			    drawSurface( painter, QRect((reverse ? right - barLen + 1 : left + 1), top + 1, barLen - 2, hh), 
					 green.lighter(105), green.darker(107) );
			    painter->fillRect( (reverse ? right - barLen + 1 : left + 1), top + 1 + hh, barLen - 2, height -  2 - hh, green.darker(111) );
			}
			
			painter->setPen( green );
			drawContour( painter, (reverse ? right - barLen : left), top, (reverse ? right : left + barLen), bottom );
			
		    }
		    else if ( !(pbar->minimum == 0 && pbar->maximum == 0) ) {
			int barLen = int( curVal / qMax(double(1.0), double(maxVal)) * width );
			int hw = (barLen - 2) / 2, hh = (height - 2) / 2;
			int cx= ( vertical ? option->rect.center().y() : option->rect.center().x() );
			
			if ( (hw > 0)  && (hh > 0) ) {
			    if ( isEnabled ) {
				int i = 0;
				int tmp = hw;
				QColor hl = option->palette.color( QPalette::Highlight );
				
				if ( hw > 120 ) {
				    hw -= 120;
				    drawSurface( painter, QRect(cx - hw, top + 1, hw + hw, hh), hl.lighter(165), hl.lighter(130), true, true );
				    drawSurface( painter, QRect(cx - hw, top + 1 + hh, hw + hw, height - hh - 2), hl.lighter(130), hl.lighter(145), true, true );
				    i = hw;
				}
				else 
				    hw = 0;
				
				const int step = 10;
				QColor hl2 = hl;
				
				for ( int j = 1 ; i < tmp; i += step  ) {
				    int lt =  (cx - ( tmp > step ? i + step : tmp ));
				    int ltLen = ( tmp > step ? step : tmp );
				    int rt = (cx  +  ( tmp > step ? i : 0 ));
				    int rtLen =  ( tmp > step ? step : tmp + (barLen % 2) );
				    double value = (tmp - hw) / step;
				    value = qMax(double(1.0), double(value));
				    double mul = 30 / value;
				    QColor hl2 =  hl.lighter( int(130  -  mul * j) );
				    
				    if ( (tmp > step) && (i + step > tmp) ) {
					lt = cx - tmp;
					ltLen = tmp  % step;
					rtLen = (tmp % step) + (barLen % 2);
				    }
				    
				    drawSurface( painter, QRect(lt, top + 1, ltLen, hh), hl.lighter(165), hl2, true, true );
				    drawSurface( painter, QRect(lt, top + 1 + hh, ltLen, height - 2 - hh), hl2, hl.lighter(145), true, true );
				    
				    drawSurface( painter, QRect( rt, top + 1, rtLen, hh), hl.lighter(165), hl2, true, true );
				    drawSurface( painter, QRect( rt, top + 1 + hh, rtLen, height - 2 - hh), hl2, hl.lighter(145), true, true );
				    
				    ++j;
				}
				
				hw = tmp;
			    }
			    else
				painter->fillRect( cx  - hw, top + 1, barLen - 2 , height - 2, 
						   option->palette.color(QPalette::Window).darker(110) );
			}
			
			painter->setPen( isEnabled ? option->palette.color(QPalette::Highlight) : option->palette.color(QPalette::Mid) );
			drawContour( painter, cx - hw, top, cx + hw, bottom );
		    }
		    else  {
			const int barSize = 10;
			int progress = (clock.elapsed() / updateTime % (width * 2));
			int tp = top + (height - barSize) / 2;
			int mn = 2;
			
			if ( width > (barSize + 2 * mn) )
			    mn = mn + (width % (barSize + 5 )) / 2;
			else if ( width > barSize )
			    mn = (width % 10) / 2;
			else
			    mn = 0;
			
			if ( progress > width )
			    progress = width - ( progress - width );
			
			painter->setRenderHint( QPainter::Antialiasing, true );
			
			for ( int sp = left + mn; sp < right - mn; sp += 15 ) {
			    QRect rect(sp, tp, barSize, barSize);
			    painter->setBrush( (rect.contains(left + progress, tp) ? QColor(Qt::red) : QColor(Qt::yellow)) );
			    painter->drawEllipse( rect );
			}	
		    }
		    painter->restore();
		}
	    }
	    break;
	}
    case CE_MenuItem:
	{
	    const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>( option );
	    int tw = mi->tabWidth, iw = mi->maxIconWidth, marginX = 6;
	    tw = ( tw < 0 ) ? 0 : tw;
	    iw = ( iw < 16 ) ? 16 : iw;
	    
	    painter->fillRect( option->rect, option->palette.brush(QPalette::Normal, QPalette::Window) );
	    QColor mid =  option->palette.color( QPalette::Mid );
	    mid.setAlphaF( 0.25 );
	    painter->setPen( mid );
	    
	    if ( !reverse )
		painter->drawLine( left + 2 * marginX + iw, top, left + 2 * marginX + iw, bottom );
	    else
		painter->drawLine( right - 2 * marginX - iw, top,  right - 2 * marginX -  iw, bottom );
	    
	    painter->setPen( option->palette.color(QPalette::Normal, QPalette::Midlight) );
	    
	    if ( !reverse )
		painter->drawLine( left + 2 * marginX + iw + 2, top, left + 2 * marginX + iw + 2, bottom );
	    else
		painter->drawLine( right - 2 * marginX  - iw - 1, top, right - 2 * marginX - iw - 1, bottom );
	    
	    if ( mi->menuItemType == QStyleOptionMenuItem::Separator ) {
		painter->setPen( mid );
		
		if ( !reverse ) {
		    painter->drawLine( left + 2 * marginX + 1 + iw, top + (height - 2) / 2, right, top + (height - 2) / 2 );
		    painter->setPen( option->palette.color(QPalette::Normal, QPalette::Midlight) );
		    painter->drawLine( left + 2 * marginX + 1 + iw, top +  (height - 2) / 2 + 1, right, top + (height - 2) / 2 + 1 );
		}
		else {
		    painter->drawLine( left, top, right - 2 * marginX - 1 - iw, top );
		    painter->setPen( option->palette.color(QPalette::Normal, QPalette::Midlight) );
		    painter->drawLine( left, top + 1, right - 2 * marginX - 1 - iw, top + 1 );
		}
		left += iw + marginX * 3 + 2;
	    }
	    else {
		if ( option->state & QStyle::State_Selected ) {
		    QPainterPath rc = roundRectPath( option->rect, 4 );
		    painter->save();
		    painter->setRenderHint( QPainter::Antialiasing, true );
		
		    if ( isEnabled ) {
			QColor highlight = option->palette.color( QPalette::Highlight ); 
			QLinearGradient linearGrad;
			linearGrad.setStart( left, top );
			linearGrad.setFinalStop( left, bottom );
			linearGrad.setColorAt( 0.0, highlight.lighter(102) );
			linearGrad.setColorAt( 0.5, highlight.lighter(108)  );
			linearGrad.setColorAt( 1.0, highlight.lighter(114) );
			painter->setPen( Qt::NoPen );
			painter->setBrush( linearGrad );
			painter->drawPath( rc );
		    }
		    else {
			painter->setPen( option->palette.color(QPalette::Dark) );
			painter->setBrush( Qt::NoBrush );
			painter->drawPath( rc );
		    }
		    
		    painter->restore();
		}
		
		
		if ( !mi->icon.isNull() ) {
		    QRect ir = visualRect( option->direction, option->rect, QRect( left + marginX, top, iw, height ) );
		    QIcon::Mode m = ( isEnabled && (option->state & QStyle::State_Selected) ) ? QIcon::Active :  
				    ( isEnabled ? QIcon::Normal  : QIcon::Disabled );
		    
		    QPixmap pm;
		    pm = mi->icon.pixmap( pixelMetric(PM_SmallIconSize, option, widget), m, 
					  (mi->checked ? QIcon::On : QIcon::Off) );
		    painter->drawPixmap( ir.x() + (iw - pm.width()) / 2, ir.y() + (height - pm.height()) / 2,
					 pm.width(), pm.height(), pm );
		}
		else if ( QStyleOptionMenuItem::NotCheckable != mi->checkType && mi->checked ) {
		    if ( QStyleOptionMenuItem::NonExclusive & mi->checkType ) {
			QRect checkRect = visualRect( option->direction, option->rect, 
						      QRect( left + marginX + (iw - 9) / 2, top + (height - 9) / 2 , 9, 9 ) );
			drawCheckMark( painter, checkRect,
				       (option->state & QStyle::State_Selected ? option->palette.highlightedText() :
					option->palette.windowText()) );
		    }
		    else if ( QStyleOptionMenuItem::Exclusive & mi->checkType ) {
			QPainterPath circle;
			QRect radioRect = visualRect( option->direction, option->rect, 
						      QRect(left + marginX + (iw - 7) / 2, top + ( height - 7) / 2, 7, 7) ); 
			circle.addEllipse( radioRect );
			
			if ( isEnabled && (option->state & QStyle::State_Selected) ) {
			    painter->setPen( option->palette.color(QPalette::HighlightedText) );
			    painter->setBrush( option->palette.highlightedText() );
			}
			else {
			    painter->setPen( option->palette.color(QPalette::WindowText) );
			    painter->setBrush( option->palette.windowText() );
			}
			painter->setRenderHint( QPainter::Antialiasing, true );
			painter->drawPath( circle );
			painter->setRenderHint( QPainter::Antialiasing, false );
		    }  
		}
		
		left += iw + marginX * 3 + 2;
		
		if ( !mi->text.isEmpty() ) {
		    QString text = mi->text;
		    
		    painter->setPen( ((isEnabled && (option->state & QStyle::State_Selected)) ? 
				      option->palette.color(QPalette::HighlightedText) : option->palette.color(QPalette::WindowText)) );
		    
		    if ( QStyleOptionMenuItem::DefaultItem == mi->menuItemType ) {
			QFont font = mi->font;
			font.setBold( true );
			painter->setFont( font );
		    }
		    
		    int accel = text.indexOf('\t');
		    int textFlags  = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
		    
		    QRect subRect = visualRect( option->direction, option->rect, QRect( right - tw - 28, top, tw, height) );
		    
		    if ( accel != - 1)  
			painter->drawText( subRect, textFlags, text.mid(accel + 1) );
		    
		    subRect = visualRect( option->direction, option->rect, QRect( left, top, width - iw - tw - 4 * marginX - 22, height) );
		    painter->drawText( subRect, textFlags, text.left(accel) );
		}
		
		if ( mi->menuItemType == QStyleOptionMenuItem::SubMenu ) {
		    QRect subRect =  visualRect( option->direction, option->rect, 
						 QRect( right - 22, top + (height - 9) / 2, 7, 9 ) );
		    drawAnyArrow( (!reverse ? PE_IndicatorArrowRight : PE_IndicatorArrowLeft), painter, subRect, 
				  (option->state & QStyle::State_Selected ? option->palette.highlightedText() : option->palette.windowText()), true );
		}
	    }
	   
	    break;
	}
    case CE_MenuBarItem:
	{
	     if ( const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>( option ) ) {
		QPen oldPen = painter->pen();	    
		const QMenuBar  *mb = qobject_cast<const QMenuBar *>( widget );
		
		if ( mb ) {
		    QRect pr = mb->rect();
		    drawSurface( painter, QRect( left, pr.top(), width, pr.height() / 2),  mi->palette.color(QPalette::Normal, QPalette::Base), 
				 mi->palette.color(QPalette::Normal, QPalette::Window) );
		    painter->fillRect( left, pr.y() + pr.height() / 2, width, pr.height() / 2, mi->palette.brush(QPalette::Normal, QPalette::Window) ); 
		}
		
		if ( isEnabled && (QStyle::State_Selected & mi->state) ) {
//		    if ( QStyle::State_Sunken & mi->state ) {
			if ( isRnd(width, height) ) {
			    painter->save();
			    QPainterPath rc = roundRectPath( mi->rect, 5 );
			    painter->setRenderHint( QPainter::Antialiasing, true );
			    painter->setPen( mi->palette.mid().color() );
			    painter->setBrush( Qt::NoBrush );
			    painter->setClipPath( rc );
			    drawSurface( painter, mi->rect, 
					 mi->palette.color(QPalette::Normal, QPalette::Midlight).darker(105), 
					 mi->palette.color(QPalette::Normal, QPalette::Midlight).lighter(105) );
			    painter->drawPath( rc );
			    painter->restore();
			}
	
//			painter->setPen( mi->palette.mid().color() );
//			drawContour( painter, left, top, right, bottom );
//			painter->setPen( mi->palette.color(QPalette::Midlight) );
//			painter->drawLine( left + 1, bottom, right - 1, bottom );
//		    }
//		    else
//			drawButton( painter, mi->rect, mi->palette, (mi->state | State_Horizontal | State_MouseOver) );
		}
		
		if ( !mi->text.isEmpty() )
		    drawItemText ( painter, mi->rect, Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine,
				   mi->palette, isEnabled, mi->text, QPalette::WindowText );
		else if ( !mi->icon.isNull() )
		    drawItemPixmap( painter, mi->rect, Qt::AlignCenter, mi->icon.pixmap(pixelMetric(PM_SmallIconSize, option, widget)) );
		
		painter->setPen( oldPen );
	    }
	    break;
	}
    case CE_MenuBarEmptyArea:
	{
	    QPen oldPen = painter->pen();
	    drawSurface( painter, QRect(left, top, width, height / 2), 
			 option->palette.color(QPalette::Normal, QPalette::Base), 
			 option->palette.color(QPalette::Normal, QPalette::Window) );
	    //	    painter->fillRect( QRect( left, top + height / 2, width, height / 2 +  (height % 2) ), 
	    //			       option->palette.brush(QPalette::Normal, QPalette::Window) );
	    
	    painter->setPen( oldPen );
	    break;
	}
    case CE_SizeGrip:
	{
	    Qt::Corner corner;
	    QPen oldPen = painter->pen();
	    
	    if ( const QStyleOptionSizeGrip *sizeGrip = qstyleoption_cast<const QStyleOptionSizeGrip *>(option) ) 	
		corner = sizeGrip->corner;
	    else
		corner = Qt::BottomRightCorner;
	    
	    int k = 0;
	    
	    switch ( corner ) {
	    case Qt::BottomRightCorner:
		{
		    for ( int i = bottom + 1 ; i >= top - 1; i -= 4 ) {
			int j = left + k * 4;
			
			for ( ; j <= right;  j += 4 )
			    drawBoldPoint( painter, option->palette.color(QPalette::Text), 
					   option->palette.color(QPalette::Base), j, i, TRUE );
			
			++k;
		    }	
		    break;
		}
	    case Qt::TopRightCorner:
		{
		    for ( int i = top + 1; i <= bottom - 1; i += 4 ) {
			int j = left + k * 4;
			
			for( ; j <= right; j += 4 )
			    drawBoldPoint( painter, option->palette.color(QPalette::Text), 
					   option->palette.color(QPalette::Base), j, i, TRUE );
			
			++k;
		    }	
		    break;
		}
	    case Qt::TopLeftCorner:
		{
		    for ( int i = top + 1; i <= bottom - 1; i += 4 ) {
			int j = right - k * 4;
			
			for( ; j >= left; j -= 4 )
			    drawBoldPoint( painter, option->palette.color(QPalette::Text), 
					   option->palette.color(QPalette::Base), j, i, TRUE );
			
			++k;
		    }	
		    break;
		}
	    case Qt::BottomLeftCorner:
		{
		    for ( int i = bottom + 1 ; i >= top - 1; i -= 4 ) {
			int j = right - k * 4;
			
			for ( ; j >= left;  j -= 4 )
			    drawBoldPoint( painter, option->palette.color(QPalette::Text), 
					   option->palette.color(QPalette::Base), j, i, TRUE );
			
			++k;
		    }	
		    break;
		}
	    default: break;  
	    }
	    painter->setPen( oldPen );
	    break;
	}
    case CE_HeaderSection:
	{
	    QPen oldPen = painter->pen();
	    
	    if ( const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option) ) {
		QColor c1 = option->palette.color( QPalette::Base );
		QColor c2 = option->palette.color( QPalette::Window ).lighter( 104 );
		QColor mid = option->palette.color( QPalette::Mid );
		QRect r = ( qobject_cast<const QHeaderView *>(widget) ? widget->rect() : QRect(0, 0, -1, -1) );
		
		mid.setAlphaF( 0.33 );
		
		if ( State_MouseOver & header->state ) {
		    c1 = alphaBlendColors( c1,  header->palette.color(QPalette::Highlight).lighter(110), 240 );
		    c2 = alphaBlendColors( c2,  header->palette.color(QPalette::Highlight).lighter(110), 240 );
		}
		
		
		drawSurface( painter, option->rect, c1, c2 );
		painter->setPen( mid );
		
		if ( Qt::Horizontal == header->orientation ) {
		    painter->drawLine( left, bottom, right, bottom );
		    
		    if ( r.isValid() && (left > r.left() + 2) )
			painter->drawLine( left, top, left, bottom );
		    
		    if ( ((QStyleOptionHeader::End != header->position) && (QStyleOptionHeader::OnlyOneSection != header->position)) || 
			 (r.isValid() && (right < r.right() - 2)) )
			painter->drawLine( right, top, right, bottom);
		}
		else {
		    painter->drawLine( right, top, right, bottom );
		    
		    if ( r.isValid() && (top > r.top() + 2) )
			painter->drawLine( left, top, right, top );
		    
		    if ( ((QStyleOptionHeader::End != header->position) && (QStyleOptionHeader::OnlyOneSection != header->position)) ||
			 (r.isValid() && (bottom < r.bottom() - 2)) )
			painter->drawLine( left, bottom, right, bottom );
		}
	    }
	    
	    painter->setPen( oldPen );
	    break;
	}
    case CE_HeaderEmptyArea:
	{
	    drawSurface( painter, option->rect, option->palette.color(QPalette::Base), option->palette.color(QPalette::Window).lighter(120) );
	    break;
	}
    case CE_MenuHMargin:
    case CE_MenuVMargin:
	{
	    return;
	}
    case CE_ScrollBarAddLine:
	{  
	    if ( const QStyleOptionSlider *sb = qstyleoption_cast<const QStyleOptionSlider *>( option ) ) {
		State state = sb->state;
		bool horz = (sb->orientation == Qt::Horizontal);
		
		if ( isEnabled ) {
		    QColor c1 = option->palette.color( QPalette::Window );
		    QColor c2 = option->palette.color( QPalette::Base );
		    
		    if ( !c1.isValid() || (c1 == QColor(Qt::transparent)) ) {
			c1 = QColor( Qt::lightGray );
			c1 = c1.lighter( 120 );
		    }

		    if ( !c2.isValid() || (c2 == QColor(Qt::transparent)) ) {
			c2 = c1.lighter( 120 );
		    }
			
		    c1.setAlpha( 255 ); 
		    c2.setAlpha( 255 );
		    drawSurface( painter, sb->rect,  c1, c2, horz );
		}
		else
		    painter->fillRect( sb->rect, sb->palette.color(QPalette::Midlight) );
		
		PrimitiveElement a;
		QRect aRect;
		
		if ( horz ) {
		    a = ( !reverse ? PE_IndicatorArrowRight : PE_IndicatorArrowLeft );
		    aRect.setRect( left + (width - 7) / 2, top + (height - 9) / 2, 7, 9 ); 
		    
		} 
		else {
		    a = PE_IndicatorArrowDown;
		    aRect.setRect( left + (width - 9) / 2, top + (height - 7) / 2, 9, 7 ); 
		}
		
		if ( isEnabled && (State_MouseOver & state) )
		    drawSlider( painter, sb->rect, option->palette, (horz ? state | State_Horizontal : state));
		
		aRect.adjust( 2, 2, 0, 0 );
		drawAnyArrow( a, painter, aRect, sb->palette.buttonText() );
	    }
	    break;
	}
    case CE_ScrollBarSubLine:
	{ 
	    if ( const QStyleOptionSlider *sb = qstyleoption_cast<const QStyleOptionSlider *>(option) ) {
		bool horz = (sb->orientation == Qt::Horizontal);
		State state = sb->state;
		
		if ( isEnabled ) {
		    QColor c1 = option->palette.color( QPalette::Window );
		    QColor c2 = option->palette.color( QPalette::Base );
		    
		    if ( !c1.isValid() || (c1 == QColor(Qt::transparent)) ) {
			c1 = QColor( Qt::lightGray );
			c1 = c1.lighter( 120 );
		    }

		    if ( !c2.isValid() || (c2 == QColor(Qt::transparent)) ) {
			c2 = c1.lighter( 120 );
		    }
		    
		    c1.setAlpha( 255 );
		    c2.setAlpha( 255 );
		    drawSurface( painter, sb->rect,  c1, c2, horz );
		}
		else
		    painter->fillRect( sb->rect, sb->palette.color(QPalette::Midlight) );
		
		
		int scrollBarExtent = pixelMetric( PM_ScrollBarExtent, option, widget );
		PrimitiveElement a;
		QRect aRect1;
		QRect aRect2;
		
		if ( horz ) {
		    a = ( !reverse ? PE_IndicatorArrowLeft : PE_IndicatorArrowRight );
		    
		    if ( State_MouseOver & state ) {
			drawSlider( painter, QRect(left, top, scrollBarExtent, height), option->palette, option->state | State_Horizontal );
			drawSlider( painter, QRect(right - (scrollBarExtent - 1), top, scrollBarExtent, height),
				    option->palette, option->state | State_Horizontal );
		    }
		    
		    aRect1.setRect( left + (scrollBarExtent - 7) / 2, top + (height - 9) / 2, 7, 9 ); 
		    aRect2.setRect( right - (scrollBarExtent - 1) + (scrollBarExtent - 7) / 2, top + (height - 9) / 2, 7, 9 );
		}
		else {
		    a = PE_IndicatorArrowUp;
		    
		    if ( isEnabled && (State_MouseOver & state) ) {
			drawSlider( painter, QRect(left, top, width, scrollBarExtent), option->palette, option->state );
			drawSlider( painter, QRect(left, bottom - (scrollBarExtent - 1), width, scrollBarExtent), 
				    option->palette, option->state );
		    }
		    
		    aRect1.setRect( left + (width - 9) / 2, top + (scrollBarExtent - 7) / 2, 9, 7 ); 
		    aRect2.setRect( left + (width - 9) / 2, bottom - (scrollBarExtent - 1) + (scrollBarExtent - 7) / 2, 9, 7 );
		}
		aRect1.adjust( 2, 2, 0, 0 );
		aRect2.adjust( 2, 2, 0, 0 );
		drawAnyArrow( a, painter, aRect1, sb->palette.buttonText() );
		drawAnyArrow( a, painter, aRect2, sb->palette.buttonText() );
	    }
	    break;
	}
    case CE_ScrollBarAddPage:
    case CE_ScrollBarSubPage:
    case CE_ScrollBarFirst:
    case CE_ScrollBarLast:
	{
	    State state = option->state;
	    bool horz = false;
	    
	    if ( const QScrollBar *sbar = qobject_cast<const QScrollBar *>(widget) ) {
		if (  Qt::Horizontal == sbar->orientation() )
		    horz = true;
	    }
	    
	    if ( isEnabled ) {
		QColor c3 = option->palette.color( QPalette::Mid );
		QColor c1 = option->palette.color( QPalette::Window );
		QColor c2 = option->palette.color( QPalette::Base );
		
		if ( !c1.isValid() || (c1 == QColor(Qt::transparent)) ) {
		    c1 = QColor( Qt::lightGray );
		    c1 = c1.lighter( 120 );
		}
		
		if ( !c2.isValid() || (c2 == QColor(Qt::transparent)) ) {
		    c2 = c1.lighter( 120 );
		}
		
		c1.setAlpha( 255 );
		c2.setAlpha( 255 );
		c3.setAlpha( 255 );
		drawSurface( painter, option->rect, (state & State_Sunken ? c3 :  c1), (state & State_Sunken ? c3.light(120) :  c2), horz );
	    }
	    else
		painter->fillRect( option->rect, option->palette.color(QPalette::Midlight) );
	    
	    
	    break;
	}
    case CE_ScrollBarSlider:
	{
	    QPen oldPen = painter->pen();
	    drawSlider( painter, option->rect, option->palette, option->state );
	    int ext = pixelMetric( PM_ScrollBarExtent, option, widget ) - 4;
	    int h, s, v;
	    bool out = false;
	    QColor c1 = option->palette.color( QPalette::Mid );
	    QColor c2 = option->palette.color( QPalette::Base );
	    
	    c1.getHsv( &h, &s, &v );
	   if ( v < 128 )
	       out = true;
	   
	   if ( !out ) {
	       c2.getHsv( &h, &s, &v );
	   
	       if ( v < 128 )  out = true;
	   }
	   
	   if ( !out && ((width >= 3 * ext) || (height >= 3 * ext)) ) {
		QString pmkey;
		QPixmap pixmap( ext, ext );
		QRect rect = option->rect;
		
		if ( c1 == QColor(Qt::transparent) || c2 == QColor(Qt::transparent) ) {
		    c1 = QColor( Qt::darkGray );
		    c2 = c1.lighter( 140 );
		}
		
		c1.setAlpha( 255 );
		c2.setAlpha( 255 );
		c1 = c1.darker( 120 );
		pmkey.sprintf( "cc:%x-%x-%x-%x-%s", c1.rgb(),  c2.rgb(), ext, ext, "scrollbar" );
		
		if ( !UsePixmapCache || !QPixmapCache::find(pmkey, pixmap) ) {
		    pixmap.fill( Qt::transparent );
		    QRect pr = pixmap.rect();
		    QPainter pp( &pixmap );
		    pp.setRenderHint( QPainter::Antialiasing, true );	    
		    QRadialGradient radialGrad;
		    
		    if ( option->state & State_Horizontal ) {
			radialGrad.setCenter( pr.center().x(), pr.center().y() );
			radialGrad.setFocalPoint( pr.center().x(), pr.y() );
		    }
		    else {
			radialGrad.setCenter( pr.center().x(), pr.center().y() );
			radialGrad.setFocalPoint( pr.x(), pr.center().y() );
		    }
		    
		    radialGrad.setRadius( pr.width() / 2 );
		    radialGrad.setColorAt( 0.0, c1 );
		    radialGrad.setColorAt( 1.0, c2 ); 
		    pp.setPen( Qt::NoPen );
		    pp.setBrush( radialGrad );
		    pp.drawEllipse( pr );
		    pp.end();
		    
		    if ( UsePixmapCache )
			QPixmapCache::insert( pmkey, pixmap );
		}
		
		painter->drawPixmap( QRect( rect.center().x() - ext / 2,  rect.center().y() - ext / 2, ext, ext), pixmap );
	    }
	    painter->setPen( oldPen );
	    break;
	}
    case CE_ToolBoxTabShape:
	{
	    painter->setRenderHint( QPainter::Antialiasing, true );
	    painter->setPen( QPen(option->palette.brush(isEnabled ? QPalette::Midlight : QPalette::Mid), 2) );
	    QPainterPath arch;
	    
	    if ( !reverse ) {
		arch.moveTo( left, top + 1 );
		arch.lineTo( right - 60, top + 1 );
		arch.cubicTo( right - 50, top + 1, right - 45, bottom - 3, right - 20, bottom - 3 );
		arch.lineTo( right, bottom - 3);
	    }
	    else {
		arch.moveTo( left, bottom - 3 );
		arch.cubicTo( left + 20, bottom - 3, left + 45, bottom - 3, left + 50, top + 1 );
		arch.lineTo( left + 60, top + 1 );
		arch.lineTo( right, top + 1 );
	    }
	    
	    painter->drawPath( arch );
	    painter->setRenderHint( QPainter::Antialiasing, false );
	    break;
	}
    default:
	{
	    ParentStyle::drawControl( control, option, painter, widget );
	    break;
	}
    }
}

void L4uStyle2::drawComplexControl( ComplexControl control, 
				    const QStyleOptionComplex *option, QPainter *painter, const QWidget * widget ) const 
{
    if ( !option ) return;
    
    switch( control ) {
    case CC_SpinBox:
	{
	    if ( const QStyleOptionSpinBox *sbox = qstyleoption_cast<const QStyleOptionSpinBox *>(option) ) {
		State state = sbox->state;
//		painter->save();
		
		if ( (SC_SpinBoxFrame & sbox-> subControls) && sbox->frame ) {
		    QRect frame = subControlRect( CC_SpinBox, sbox, SC_SpinBoxFrame, widget );
		    
		    if ( frame.isValid() ) {   
			bool rnd = isRnd( frame.width(), frame.height() );
			
			if ( rnd )
			    painter->setClipPath( roundRectPath(sbox->rect, 4) );
		
			if ( State_Enabled & state ) { 	
			    QColor border; 
			    
			    if ( State_HasFocus & state )
				border = sbox->palette.highlight().color();
			    else {
				border = sbox->palette.dark().color();
				border.setAlphaF( 0.7 );
			    }
			    
			    painter->setPen( border );
			}
			else 
			    painter->setPen(  sbox->palette.color(QPalette::Mid) );
			
			if ( rnd ) {    
			    painter->setClipping( false );
			    painter->setRenderHint( QPainter::Antialiasing, true );
			    painter->setBrush( Qt::NoBrush );
			    painter->drawPath( roundRectPathF((QRectF(sbox->rect)).adjusted(0.5, 0.5, -0.5, -0.5), 4) );
			    painter->setRenderHint( QPainter::Antialiasing, false );
			}
			else 	
			    drawContour( painter, frame.left(), frame.top(), frame.right(), frame.bottom() );
		    }
		}
		
		if ( SC_SpinBoxUp & sbox-> subControls ) {
		    QRect up = subControlRect( CC_SpinBox, sbox, SC_SpinBoxUp, widget );
		    
		    if ( up.isValid() && (QAbstractSpinBox::NoButtons != sbox->buttonSymbols) ) { 
			State tmp = state;
			
			if ( !(QAbstractSpinBox::StepUpEnabled & sbox->stepEnabled) )
			    state &= ~State_Enabled;
			
			if ( State_Enabled & state ) { 
			    if ( SC_SpinBoxUp & sbox->activeSubControls ) {
				if ( State_Sunken & state )
				    state &= ~State_MouseOver;
			    }
			    else
				state &= ~(State_MouseOver | State_Sunken );
			}
			
			QStyleOption opt =  *option;
			QRegion clip( up );
			opt.rect = up.adjusted( 1, 1, 0, 0 );
			opt.state = state;
			painter->setClipRegion( clip );
			drawPrimitive( (QAbstractSpinBox::UpDownArrows == sbox->buttonSymbols ? PE_IndicatorSpinUp : 
					PE_IndicatorSpinPlus), &opt, painter, widget );
			state = tmp;
		    }
		}
		
		if ( SC_SpinBoxDown & sbox-> subControls ) {
		    QRect down = subControlRect( CC_SpinBox, sbox, SC_SpinBoxDown, widget );

		    
		    if ( down.isValid() && (QAbstractSpinBox::NoButtons != sbox->buttonSymbols) ) { 
			if ( !(QAbstractSpinBox::StepDownEnabled & sbox->stepEnabled) )
			    state &= ~State_Enabled;
			
			if ( State_Enabled & state ) { 
			    if ( SC_SpinBoxDown & sbox->activeSubControls ) {
				if ( State_Sunken & state )
				    state &= ~State_MouseOver;
			    }
			    else
				state &= ~(State_MouseOver | State_Sunken );
			}
			    
			QStyleOption opt = *option;
			QRegion clip( down );
            opt.rect = down.adjusted( 1, 2, 0, 0 );
			opt.state = state;
			painter->setClipRegion( clip );
			drawPrimitive( (QAbstractSpinBox::UpDownArrows == sbox->buttonSymbols ? PE_IndicatorSpinDown :
					PE_IndicatorSpinMinus), &opt, painter, widget );
		    }
		}
//		painter->restore();
	    }
	    break;
	}
    case CC_ComboBox:
	{
	    if ( const QStyleOptionComboBox *cmb = qstyleoption_cast<const QStyleOptionComboBox *>(option) ) {
//		painter->save();
		
		if ( (SC_ComboBoxFrame & cmb-> subControls) && cmb->frame ) { 
		    QRect frame = subControlRect( CC_ComboBox, cmb, SC_ComboBoxFrame, widget );
		    
		    if ( frame.isValid() ) {
			if ( !cmb->editable ) {
			    drawButton( painter, frame, cmb->palette, State_Horizontal | cmb->state );
			}
			else {
			    QRect ed = subControlRect( CC_ComboBox, cmb, SC_ComboBoxEditField, widget );
			    bool rnd = isRnd( frame.width(), frame.height() );
			    int fw =  pixelMetric( PM_ComboBoxFrameWidth );
			    QLinearGradient linearGrad;
			    linearGrad.setStart( cmb->rect.x() + fw, cmb->rect.y() + fw );
			    linearGrad.setFinalStop( cmb->rect.x() + fw, cmb->rect.bottom() - fw );
			    linearGrad.setColorAt( 0.0, cmb->palette.color(QPalette::Midlight) );
			    linearGrad.setColorAt( 0.5, cmb->palette.color(QPalette::Midlight).lighter(105) );
			    linearGrad.setColorAt( 1.0, cmb->palette.color(QPalette::Base) );
			    
			    if ( rnd )
				painter->setClipPath( roundRectPath(cmb->rect, 4) );
			    
			    painter->fillRect( cmb->rect.x() + fw, cmb->rect.y() + fw, ed.height(), ed.height(), linearGrad );
			    painter->setBrush( Qt::NoBrush );
			    
			    if ( State_Enabled & cmb->state ) { 	
				QColor border;
				
				if ( State_HasFocus & cmb->state )
				    border = cmb->palette.highlight().color();
				else {
				    border = cmb->palette.dark().color();
				    border.setAlphaF( 0.7 );
				}
				painter->setPen( border );
			    }
			    else 
				painter->setPen(  cmb->palette.color(QPalette::Mid) );
			    
			    if ( rnd ) {    
				painter->setClipping( false );
				painter->setRenderHint( QPainter::Antialiasing, true );
				painter->drawPath( roundRectPathF((QRectF(cmb->rect)).adjusted(0.5, 0.5, -0.5, -0.5), 4) );
				painter->setRenderHint( QPainter::Antialiasing, false );
			    }
			    else
				drawContour( painter, frame.left(), frame.top(), frame.right(), frame.bottom() );	
			}
		    }
		}
		
		if ( SC_ComboBoxArrow & cmb-> subControls ) {
		    QRect arrow = subControlRect( CC_ComboBox, cmb,  SC_ComboBoxArrow, widget );
		 
		    if ( arrow.isValid() ) {
			if (cmb->editable && (State_MouseOver & cmb->state) )
			    drawButton( painter, arrow, cmb->palette, State_Horizontal | cmb->state );
			
			QStyleOption opt = *option;
			arrow.adjust( 1, 1, 0, 0 );
			opt.rect = arrow;
			drawPrimitive( PE_IndicatorArrowDown, &opt, painter, widget );
		    }
		}
		
//		if ( !cmb->editable && ((State_HasFocus & cmb->state) && (State_KeyboardFocusChange & cmb->state)) ) {
//		    QStyleOptionFocusRect opt;
//		    opt.rect = subControlRect( CC_ComboBox, cmb, SC_ComboBoxEditField, widget );
//		    drawPrimitive( PE_FrameFocusRect, &opt, painter, widget );
//		}
		
//		painter->restore();
	    }
	    break;
	}
    case CC_Slider:
	{
	    if ( const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(option) ) {
		QPen oldPen = painter->pen();
		bool isEnabled = State_Enabled & slider->state;
		
		if ( (option->subControls & SC_SliderGroove) ) {
		   
		    QRect groove = subControlRect(CC_Slider, option, SC_SliderGroove, widget);
		    
		    if ( groove.isValid() ) {
			int left = groove.left(), right = groove.right(), top = groove.top(), bottom = groove.bottom();
			int width = groove.width(), height = groove.height();
			
			if ( Qt::Horizontal == slider->orientation ) {		
			    if ( isEnabled )
				drawSurface( painter, QRect(left, top + (height - 6) / 2 + 1, width, 4),
					     option->palette.color(QPalette::Mid), option->palette.color(QPalette::Midlight) ); 
	
			    else
				painter->fillRect( left + 1, top + (height - 6 ) / 2 + 1, width - 2, 4, 
						   option->palette.color(QPalette::Midlight) );
		
			    painter->setPen( option->palette.color(isEnabled ? QPalette::Dark : QPalette::Mid) );
			    drawContour( painter, left, top + (height - 6 ) / 2, right, bottom - (height - 6) / 2, 
					 DrawLeft | DrawRight | DrawTop | DrawBottom);
			    painter->setPen( option->palette.color(QPalette::Mid) );
			    drawContour( painter, left, top + (height - 6 ) / 2, right, bottom - ( height - 6) / 2, DrawRight | DrawBottom );
			}
			else {
			    if ( isEnabled )
				drawSurface( painter, QRect(left + (width - 6) / 2 + 1, top, 4, height), 
					     option->palette.color(QPalette::Mid), option->palette.color(QPalette::Midlight), false );
			    else
				painter->fillRect( left + (width - 6) / 2 + 1, top + 1, 4, height - 2, 
						   option->palette.color(QPalette::Midlight) );
		
			    painter->setPen( option->palette.color(isEnabled ? QPalette::Dark : QPalette::Mid) );
			    drawContour( painter, left + (width - 6) / 2, top, right - (width - 6) / 2, bottom,
					 DrawLeft | DrawRight | DrawTop | DrawBottom );
			    painter->setPen( option->palette.color(QPalette::Mid) );
			    drawContour( painter, left + (width - 6) / 2, top, right - (width - 6) / 2, bottom, DrawRight | DrawBottom );
			}
		    }
		}
		
		if ( (option->subControls & SC_SliderHandle) ) {
		    QRect handle = subControlRect(CC_Slider, option, SC_SliderHandle, widget);
		
		    if ( handle.isValid() ) {
			int left = handle.left(), top = handle.top();
			int width = handle.width(), height = handle.height();
						
			if ( Qt::Horizontal == slider->orientation ) {	
			    QRect sl = QRect( left + (width - 8) / 2, top + (height - 21) / 2, 8, 21 );
			    
			    if ( isEnabled )
				drawSlider( painter, sl, slider->palette, slider->state | 
					    (slider->activeSubControls & SC_SliderHandle ? State_Sunken : State_None) );
			    else 
				painter->fillRect( sl.adjusted( 1, 1, -1, -1), slider->palette.base() );
			    
			    painter->fillRect( sl.left() + 1, sl.top() + 1, 6, 3, slider->palette.mid() );
			    painter->fillRect( sl.left() + 1, sl.bottom() - 3, 6, 3, slider->palette.mid() );
			}
			else {
			    QRect sl = QRect( left + (width - 21) / 2, top + (height - 8) / 2, 21, 8 );
			    
			    if ( isEnabled )
				drawSlider( painter, sl, slider->palette, slider->state  | 
					    (slider->activeSubControls & SC_SliderHandle ? State_Sunken : State_None) );
			    else 
				painter->fillRect( sl.adjusted( 1, 1, -1, -1), slider->palette.base() );	
			   
			    painter->fillRect( sl.left() + 1, sl.top() + 1, 3, 6, slider->palette.mid() );
			    painter->fillRect( sl.right() - 4, sl.top() + 1, 4, 6, slider->palette.mid() );
			}
			
			if ( slider->state & State_HasFocus ) {
			    QStyleOptionFocusRect focusOpt;
			    focusOpt.QStyleOption::operator=(*slider);
			    focusOpt.rect = subElementRect( SE_SliderFocusRect, slider, widget );
			    drawPrimitive( PE_FrameFocusRect, &focusOpt, painter, widget );
			}
		    }
		}
		
		if ( option->subControls & SC_SliderTickmarks ) {
		    QStyleOptionSlider sliderOpt( *slider );
		    sliderOpt.subControls &= ~(SC_SliderGroove | SC_SliderHandle);
		    sliderOpt.activeSubControls &= ~(SC_SliderGroove | SC_SliderHandle);
		    ParentStyle::drawComplexControl( control, &sliderOpt, painter, widget );
		}
		painter->setPen( oldPen );
	    }
	    break;
	}
    case CC_GroupBox:
	{
	    if (const QStyleOptionGroupBox *gbox = qstyleoption_cast<const QStyleOptionGroupBox *>(option) ) {
		QRect textRect = subControlRect( CC_GroupBox, gbox, SC_GroupBoxLabel, widget );
		QRect boxRect = subControlRect( CC_GroupBox, gbox, SC_GroupBoxCheckBox, widget );
		
		if ( QStyle::SC_GroupBoxFrame & gbox->subControls ) {
		    QStyleOptionFrameV2 frOpt;
		    frOpt.QStyleOption::operator= ( *gbox );
		    frOpt.rect = subControlRect( CC_GroupBox, option, SC_GroupBoxFrame, widget );
		    frOpt.lineWidth = gbox->lineWidth;
		    frOpt.midLineWidth = gbox->midLineWidth;
		    frOpt.features = gbox->features;
		    
		    painter->save();
		    QRegion clip( gbox->rect );
		    
		    if ( SC_GroupBoxLabel & gbox->subControls )
			clip -= textRect;
		    
		    if ( SC_GroupBoxCheckBox & gbox->subControls )
			clip -= boxRect;
		    
		    painter->setClipRegion( clip );
		    drawPrimitive( PE_FrameGroupBox, &frOpt, painter, widget );
		    painter->restore();
		}
		
		if ( (SC_GroupBoxLabel & gbox->subControls) && !gbox->text.isEmpty() ) {
		    painter->save();
		    int textFlags = int( Qt::TextShowMnemonic  | Qt::AlignCenter | gbox->textAlignment );
		    painter->fillRect( textRect.x(), textRect.y() + textRect.height() / 2, textRect.width(), textRect.height() / 2 + 1, 
				       painter->background() );
		    painter->setPen( gbox->textColor.isValid() ? gbox->textColor : gbox->palette.windowText().color() );
		    
		    if ( !styleHint(QStyle::SH_UnderlineShortcut, option, widget) )
			textFlags |= Qt::TextHideMnemonic;
		    
		    QFont font =painter->font();
		    font.setBold( true );
		    painter->setFont( font );
		    painter->drawText( textRect, textFlags, gbox->text ); 
		    
		    if ( State_HasFocus & gbox->state ) {
			QStyleOptionFocusRect focus;
			focus.QStyleOption::operator=( *gbox );
			focus.rect = textRect;
			drawPrimitive( PE_FrameFocusRect, &focus, painter, widget );
		    }
		    
		    painter->restore();
		}
		
		if ( SC_GroupBoxCheckBox & gbox->subControls ) {
		    QStyleOptionButton cbox;
		    cbox.QStyleOption::operator = (*gbox);
		    cbox.rect = boxRect;
		    drawPrimitive( PE_IndicatorCheckBox, &cbox, painter, widget );
		}
	    }
	    break;
	}
    case CC_Q3ListView:
	{
	    if ( const QStyleOptionQ3ListView *lv3 = qstyleoption_cast<const QStyleOptionQ3ListView *>(option) ) {
		if ( SC_Q3ListView & lv3->subControls )
		    ParentStyle::drawComplexControl( control, lv3, painter, widget );
		
		if ( (SC_Q3ListViewBranch | SC_Q3ListViewExpand) & lv3->subControls ) {
		    if ( lv3->items.isEmpty() )
			break;
		    
		    QStyleOptionQ3ListViewItem item = lv3->items.at( 0 );
		    QPolygon lines;
		    int cnt;
		    int tp = lv3->rect.top();
		  
		    if ( (SC_All & lv3->activeSubControls) && (SC_Q3ListViewExpand & lv3->subControls) ) {
			cnt = 2;
			lines.resize( 2 );
			lines[0] = QPoint( lv3->rect.right(), lv3->rect.top() );
			lines[1] = QPoint( lv3->rect.right(), lv3->rect.bottom() );
		    } else {
			int i;
			int linetop = 0, linebot = 0;
			lines.resize( item.childCount * 4 );
			cnt = 0;
			
			for ( i = 1; i < lv3->items.size(); ++i ) {
			    QStyleOptionQ3ListViewItem child = lv3->items.at( i );
		
			    if (child.height + tp > 0)
				break;
			    
			    tp += child.totalHeight;
			}
			
			int cx = lv3->rect.width() / 2;
			
			while ( i < lv3->items.size() && tp < lv3->rect.height() ) {
			    QStyleOptionQ3ListViewItem child = lv3->items.at( i );
			    
			    if ( QStyleOptionQ3ListViewItem::Visible & child.features ) {
				int lh = qMax( (!(QStyleOptionQ3ListViewItem::MultiLine & item.features) ? child.height 
				       : painter->fontMetrics().height() + 2 * lv3->itemMargin), QApplication::globalStrut().height() );
				
				lh += (lh % 2);
				linebot = tp + lh / 2;
				
				if ( (QStyleOptionQ3ListViewItem::Expandable & child.features) || (child.childCount > 0 && child.height > 0) ) {
				    painter->setRenderHint( QPainter::Antialiasing, true );
				    painter->setPen( lv3->palette.dark().color() );
				    painter->setBrush( Qt::NoBrush );
				    painter->drawEllipse( cx - 5, linebot - 5, 11, 11 );
				    painter->setRenderHint( QPainter::Antialiasing, false );
				    painter->setPen( lv3->palette.foreground().color() );
				    painter->drawLine( cx - 2, linebot, cx + 2, linebot );
				    
				    if ( !(State_Open & child.state) ) {
					painter->drawLine( cx, linebot - 2, cx, linebot + 2 );
				    }
				    
				    lines[cnt++] = QPoint( cx, linetop );
				    lines[cnt++] = QPoint( cx, linebot - 5 );
				    lines[cnt++] = QPoint( cx + 6, linebot );
				    lines[cnt++] = QPoint( lv3->rect.width(), linebot );
				    linetop = linebot + 6;
				} else {
				    lines[cnt++] = QPoint( cx+1, linebot -1 );
				    lines[cnt++] = QPoint( lv3->rect.width(), linebot -1 );
				}
	
				tp += child.totalHeight;
			    }
			    
			    ++i;
			}
			
			while ( i < lv3->items.size() && lv3->items.at(i).height <= 0 )
			    ++i;
			
			if ( i < lv3->items.size() )
			    linebot = lv3->rect.height();
			
			if ( linetop < linebot ) {
			    lines[cnt++] = QPoint( cx, linetop );
			    lines[cnt++] = QPoint( cx, linebot );
			}
		    }
		    
		    painter->setPen( lv3->palette.mid().color() );
		    
		    if ( SC_Q3ListViewBranch & lv3->subControls  ) {
			for( int l = 0; l < cnt; l += 2 )	
			    painter->drawLine( lines[l], lines[l + 1] );
		    }
		}
	    }
	    break;
	}
    case CC_ToolButton:
	{
	    if ( const QStyleOptionToolButton *tb = qstyleoption_cast<const QStyleOptionToolButton *>(option) ) {
		QRect buttonRect = subControlRect( control, tb, SC_ToolButton, widget );
		QRect menuRect =  subControlRect( control, tb, SC_ToolButtonMenu, widget );
		State bflags = tb->state;
		bool b = SC_ToolButton & tb->subControls;
		bool m = SC_ToolButtonMenu & tb->subControls;
		int fw = pixelMetric( PM_DefaultFrameWidth, option, widget );
		
		if ( (State_AutoRaise & bflags) && !(State_MouseOver & bflags) )
		    bflags &= ~State_Raised;
		
		State mflags = bflags;
		bflags |= ( SC_ToolButton & tb->activeSubControls ? State_Sunken : State_None );
		mflags |= ( SC_ToolButtonMenu & tb->activeSubControls ? State_Sunken : State_None );
		QStyleOption opt = *option;
		
		if ( b ) {
		    if ( (State_Sunken | State_On | State_Raised | State_HasFocus) & bflags ) {
			opt.state = bflags;
			opt.rect = ( m ? buttonRect.united(menuRect) : buttonRect );
			drawPrimitive( PE_PanelButtonTool, &opt, painter, widget );
		    }
		
		    if ( m ) {
			if ( (State_Enabled & bflags) && (State_Raised & bflags) && (bflags == mflags) ) {
			    QColor dark = opt.palette.dark().color();
			    dark.setAlphaF( 0.6 );
			    painter->setPen( dark );
			    painter->drawLine( buttonRect.right(), buttonRect.top() + 3, buttonRect.right(), buttonRect.bottom() - 3 );
			    painter->setPen( opt.palette.midlight().color() );
			    painter->drawLine( menuRect.left(), menuRect.top() + 3, menuRect.left(), menuRect.bottom() - 3 );
			}
			
			opt.state = mflags;
			opt.rect = menuRect;
			
			if ( bflags != mflags ) {
			    drawButtonDropDown( painter, menuRect, opt.palette, mflags | State_Horizontal );  
			}
			
			drawAnyArrow( PE_IndicatorArrowDown, painter, QRect( menuRect.x() + (menuRect.width() - 9) / 2, 
							       menuRect.y() + (menuRect.height() - 7) / 2, 9, 7 ),  tb->palette.buttonText(), true );
		    }
		}
		else if ( m ) {
		    opt.state = mflags;
		    opt.rect = menuRect;
		    
		    if ( (State_Sunken | State_On | State_Raised | State_HasFocus) & mflags )
			drawPrimitive( PE_IndicatorButtonDropDown, &opt, painter, widget );
		    
		    drawPrimitive( PE_IndicatorArrowDown, &opt, painter, widget );
		}
		
		if ( !(State_HasFocus & (bflags | mflags)) && (State_HasFocus & bflags) ) {
		    QStyleOptionFocusRect frOpt;
		    int mi = (QStyleOptionToolButton::MenuButtonPopup & tb->features ? pixelMetric(PM_MenuButtonIndicator,  tb, widget) : 0);
		    frOpt.QStyleOption::operator=(*tb);
		    frOpt.rect.adjust( 3, 3, -(3 + mi), -3 );
		    drawPrimitive( PE_FrameFocusRect, &frOpt, painter, widget );
		}
		
		QStyleOptionToolButton label = *tb;
		label.rect = buttonRect.adjusted( fw, fw, -fw, -fw );	
		drawControl( CE_ToolButtonLabel, &label, painter, widget );
		
		if ( !m && (QStyleOptionToolButton::HasMenu & tb->features) ) {
		    int mi = pixelMetric( PM_MenuButtonIndicator, tb, widget );
		    QRect ir = tb->rect;
		    ir.setRect( ir.right() + 5 - mi, ir.height() + 4 - mi, mi - 6, mi - 6 );
		    drawAnyArrow( PE_IndicatorArrowDown, painter, ir, tb->palette.buttonText(), true );
		}
	    }
	    break;
	}
    default:
	{
	    ParentStyle::drawComplexControl( control, option, painter, widget );
	    break;
	}
    }
}

QRect L4uStyle2::subControlRect( ComplexControl control, const QStyleOptionComplex * option, 
			  SubControl subControl, const QWidget * widget ) const
{
    QRect subRect;
    
    switch( control ) {
    case CC_SpinBox:
	{
	    if ( const QStyleOptionSpinBox *sbox = qstyleoption_cast<const QStyleOptionSpinBox *>(option) ) {
		int fw = sbox->frame ? pixelMetric( PM_SpinBoxFrameWidth, sbox, widget ) : 0;
		int ext = pixelMetric( PM_ScrollBarExtent, sbox, widget );
		int left, top, right, width, height;
		
		left = sbox->rect.left(), top = sbox->rect.top(), right = sbox->rect.right();
		width = sbox->rect.width(); 
		height = sbox->rect.height();
		
		switch ( subControl ) {
		case SC_SpinBoxUp:
		    {
			if ( QAbstractSpinBox::NoButtons == sbox->buttonSymbols )
			    break;
			
			subRect = QRect( right -  fw - ext, top + fw, ext, (height - 2 * fw) / 2 );
			break;
		    }
		case SC_SpinBoxDown:
		    {
			if ( QAbstractSpinBox::NoButtons == sbox->buttonSymbols )
			    break;

			subRect = QRect( right -  fw - ext, top + height / 2, ext, (height - 2 * fw ) / 2 );
			break;
		    }
		case SC_SpinBoxEditField:
		    {
			if ( QAbstractSpinBox::NoButtons == sbox->buttonSymbols )	
			    subRect = QRect( left + fw, top + fw, width - 3 * fw, height - 2 * fw );
			else
			    subRect = QRect( left + fw, top + fw, width - 4 * fw - ext, height - 2 * fw );
			
			break;
		    }
		case SC_SpinBoxFrame:
		    {
			subRect = sbox->rect;
			break;
		    }
		default:
		    break;
		}
		
		 return visualRect( sbox->direction, sbox->rect, subRect ); 
	    }
	    return subRect;
	}
    case CC_ComboBox:
	{
	    if ( const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option) ) {
		int ext = pixelMetric( PM_ScrollBarExtent, cb, widget );
		int margin = cb->frame ? pixelMetric( PM_ComboBoxFrameWidth, cb, widget ) : 0;
		int left, top, right, width, height;
		
		left = cb->rect.left(), top = cb->rect.top(), right = cb->rect.right();
		width = cb->rect.width(); 
		height = cb->rect.height();
		
		switch ( subControl ) {
		case SC_ComboBoxFrame:
		    {
			subRect = cb->rect;
			break;
		    }
		case SC_ComboBoxArrow:
		    {
			subRect = QRect( right - margin - ext, top + margin, ext, height - 2 * margin );
			break;
		    }
		case SC_ComboBoxEditField:
		    {
			subRect = QRect( left + margin, top + margin, width - 4 * margin - ext, height - 2 * margin );
			break;
		    }
		case SC_ComboBoxListBoxPopup:
		    {
			subRect = cb->rect.adjusted( 0, 1, 0, 1 );
			break;
		    }
		default:
		    break;
		}
		
		return visualRect( cb->direction, cb->rect, subRect ); 
	    }
	    return subRect;
	}
    case CC_GroupBox:
	{
	    if ( const QStyleOptionGroupBox *gbox = qstyleoption_cast<const QStyleOptionGroupBox *>(option) ) {
		if ( widget ) {
		    QFont font = widget->font();
		    font.setBold( true );
		    QFontMetrics fm( font );
		    QStyleOptionGroupBox gbOpt = *gbox;
		    gbOpt.fontMetrics = fm;
		    return ParentStyle::subControlRect( control, &gbOpt, subControl, widget );
		}
	    }
	}
    default:
	return  ParentStyle::subControlRect( control, option, subControl, widget );
    }
}

void L4uStyle2::drawAnyArrow( PrimitiveElement pe, QPainter *painter, const QRect &rect, const QBrush &brush, bool real ) const
{
    if ( !rect.isValid() )
	return;
    
    painter->save();
    painter->translate( rect.center() );
    
    QMatrix matrix( rect.width() / 2, 0, 0, rect.height() / 2, 0, 0 );
    QPainterPath arrow;
    qreal x1, x2, x3, x4, y1, y2, y3, y4;
    
    if ( (PE_IndicatorArrowUp == pe) || (PE_IndicatorSpinUp == pe) ) {
	x1 = -1.0; y1 = 0.8;
	x2 = 0; y2 = -0.8;
	x3 = 1.0; y3 = 0.8;
	
	if ( !real )
	    x4 = 0; y4 = -0.4;
    }
    else if ( (PE_IndicatorArrowDown == pe)  || (PE_IndicatorSpinDown == pe) ) {
	x1 = -1.0; y1 = -0.8;
	x2 = 0; y2 = 0.8;
	x3 = 1.0; y3 = -0.8;
	
	if ( !real )
	    x4 = 0; y4 = 0.4;
    }
    else if ( (PE_IndicatorArrowLeft == pe) ) {
	x1 = 0.8; y1 = -1.0;
	x2 = -0.8; y2 = 0;
	x3 = 0.8; y3 = 1.0;
	
	if ( !real )
	    x4 = -0.4; y4 = 0;
    }
    else if ( PE_IndicatorArrowRight == pe ) {
	x1 = -0.8; y1 = -1.0;
	x2 = 0.8; y2 = 0;
	x3 = -0.8; y3 = 1.0;
	
	if ( !real )
	    x4 = 0.4; y4 = 0;
    }
    else
	return;
    
    arrow.moveTo( x1, y1 );
    arrow.lineTo( x2, y2 );
    arrow.lineTo( x3, y3 );
    
    if ( !real )
	arrow.lineTo( x4, y4 );
    
    arrow.closeSubpath();
    painter->setRenderHint( QPainter::Antialiasing, true );
   
    if ( !real ) {
	painter->setPen( QPen(brush, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
	painter->setBrush( Qt::NoBrush );
    }
    else {
	painter->setPen( Qt::NoPen );
	painter->setBrush( brush );
    }
    
    painter->drawPath( matrix.map( arrow) );
    painter->restore();
}

void L4uStyle2::drawCheckMark(  QPainter *painter, const QRect &rect, const QBrush &brush ) const
{
    if ( !rect.isValid() )
	return;
    painter->save();
    painter->translate( rect.center() );
    
    QPainterPath checkMark;
    checkMark.moveTo( -1.0, 0);
    checkMark.lineTo( 0, 1.0 );
    checkMark.lineTo( 1.0, -1.0 );
    checkMark.lineTo(  0, 0.4 );
    checkMark.lineTo( -1.0, 0 );
    checkMark.closeSubpath();
    QMatrix matrix( rect.width() / 2, 0, 0, rect.height() / 2, 0, 0 );
    
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->setPen( QPen(brush, 2) );
    painter->drawPath( matrix.map( checkMark) );
    painter->restore();
}

QSize L4uStyle2::sizeFromContents( ContentsType type, const QStyleOption *option, 
				   const QSize &contentsSize, const QWidget * widget ) const
{
    switch( type ) {
    case CT_PushButton:
	{
	    QSize csz( contentsSize );
	    
	    if ( const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option) ) {
		csz = QCommonStyle::sizeFromContents( type, option, contentsSize, widget );
		
		if ( !button->text.isEmpty() && csz.width()  < 74 )
		    csz.setWidth( 74 );
	    }
	    
	    return csz;
	}
    case CT_MenuItem:
	{	
	    int w = contentsSize.width(), h = contentsSize.height();
	    
	    if ( const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>( option ) ) {
		int iw =  mi->maxIconWidth;
		
		if ( mi->menuItemType == QStyleOptionMenuItem::Separator ) {
		    w = 10;
		    h = 6;
		}
		else {
		    if ( h < 16 ) h = 16;
		    
		    if ( !mi->text.isEmpty() )
			h = qMax( h, mi->fontMetrics.height() + 2 );
		    
		    if ( !mi->icon.isNull() )
			h = qMax( h, mi->icon.pixmap(pixelMetric(PM_SmallIconSize, option, widget)).height() );
		    
		    h += 4;
		}	
		
		iw = qMax( iw, 16 );
		w += iw + 68;
		
		if ( !mi->text.isEmpty() ) {
		    if ( mi->text.contains('\t') >= 0 )
			w += 12;
		
		    if ( mi->menuItemType == QStyleOptionMenuItem::DefaultItem ) {
			QFontMetrics fm( mi->font );
			QFont f( mi->font );
			f.setBold( true );
			QFontMetrics fmBold( f );
			w += fmBold.width( mi->text ) - fm.width( mi->text );
		    }
		}
	    }
	    
	    return QSize( w, h );
	}
    default:
	return  ParentStyle::sizeFromContents( type, option, contentsSize, widget );
    }
}

int L4uStyle2::styleHint( StyleHint hint, const QStyleOption * option, const QWidget * widget, QStyleHintReturn * returnData ) const
{
    switch( hint ) {
    case SH_EtchDisabledText:
	return false;
    case SH_ScrollView_FrameOnlyAroundContents:
	return true;
    case SH_Menu_Mask:
	{
	    if ( widget || option ) {
		if ( QStyleHintReturnMask *mask = qstyleoption_cast<QStyleHintReturnMask *>(returnData) ) {
		    int l, t, w, h;
		    
		    if ( widget )
			widget->rect().getRect( &l, &t, &w, &h );
		    else
			option->rect.getRect( &l, &t, &w, &h );
		    
		    QRegion clip( l + 4, t, w - 8, h );
		    clip += QRegion( l, t + 4, w, h - 8 );
		    clip += QRegion( l + 2, t + 1, w - 4, h - 2 );
		    clip += QRegion( l + 1, t + 2, w - 2, h - 4 );
		    mask->region = clip;
		}
	    }
	    return true;
	}
    default:
	return  ParentStyle::styleHint( hint, option, widget, returnData );
    }
}

bool L4uStyle2::eventFilter( QObject * watched, QEvent * event )
{
    if ( QFrame *frame = qobject_cast<QFrame*>(watched) ) {
	if ( (QEvent::Paint == event->type()) && (QFrame::HLine == frame->frameShape() || QFrame::VLine == frame->frameShape()) ) {
	    QRect rect = frame->rect();
	    QPainter painter( frame );
	    painter.setClipRect(((QPaintEvent*)event)->rect());
	    QColor mid =  frame->palette().color( QPalette::Mid );
	    mid.setAlphaF( 0.6 );
	    painter.setPen( mid );
	    
	    if ( rect.width() > rect.height() ) {
		painter.drawLine( rect.left(), rect.top(), rect.right(), rect.top() );
		painter.setPen( frame->palette().color(QPalette::Midlight) );
		painter.drawLine( rect.left(), rect.top() + 1, rect.right(), rect.top() + 1);
	    }
	    else {
		painter.drawLine( rect.left(), rect.top(), rect.left(), rect.bottom() );
		painter.setPen( frame->palette().color(QPalette::Midlight) );
		painter.drawLine( rect.left() + 1, rect.top(), rect.left() + 1, rect.bottom() );
	    }	
	    
	    painter.end();
	    return true;
	}
    }
    else if ( QProgressBar *pb = qobject_cast<QProgressBar *>(watched) ) {
	    switch ( event->type() ) {
	    case QEvent::Show:
		{
		    bars.insert( pb );
		
		    if ( bars.size() == 1 ) {
			clock.start();
			
			if ( !timer ) timer = startTimer( updateTime );
		    }
		
		    break;
		}
	    case QEvent::Hide:
	    case QEvent::Destroy:
		{
		    bars.remove( pb );
		
		    if ( bars.isEmpty() && timer ) {
			killTimer( timer );
			timer = 0;
		    }
		
		    break;
		}
	    default:
		break;
	}
    }
    else if ( QDockWidget*dw = qobject_cast<QDockWidget*>(watched) ) { 	 
	if ( (QEvent::Paint == event->type()) && !dw->isFloating() ) {
	    bool horz = true;
		
	    if ( QDockWidget::DockWidgetVerticalTitleBar & dw->features() )  horz = false;
	    
	    QPainter p(dw);
	    QColor mi = dw->palette().mid().color();
	    mi.setAlphaF(0.6);
	    p.setPen( mi );
	  
	    if ( horz ) {
		p.drawLine( dw->rect().left(), dw->rect().top() + 6, dw->rect().left(), dw->rect().bottom() );
		p.drawLine( dw->rect().right(), dw->rect().top() + 6, dw->rect().right(), dw->rect().bottom() );
	    }
	    else {
		p.drawLine( dw->rect().left() + 6, dw->rect().top(), dw->rect().right(), dw->rect().top() );
		p.drawLine( dw->rect().left() + 6, dw->rect().bottom(), dw->rect().right(), dw->rect().bottom() );
	    }
	    
	    return false;
	}
    }
    
    return QObject::eventFilter( watched, event );
}

void L4uStyle2::timerEvent( QTimerEvent *event )
{
    if ( timer == event->timerId() ) {
	QSet<QWidget *>::iterator iter = bars.begin();
	
	while ( iter != bars.end() ) {
	   if ( QProgressBar *bar = qobject_cast<QProgressBar *>(*iter) )
	       bar->update();
	    
	   ++iter;
       }
    }
 
    event->ignore();
}

int L4uStyle2::pixelMetric( PixelMetric metric, const QStyleOption *option, 
			    const QWidget *widget ) const
{
    switch( metric ) {
    case PM_ButtonMargin:
	return 3;
    case PM_ButtonDefaultIndicator:
	return 0;
    case PM_MenuButtonIndicator:
	return 13;
    case PM_SpinBoxFrameWidth:
	return 2;
    case PM_ComboBoxFrameWidth:
	return 2;
    case PM_MenuPanelWidth:
	return 2;
    case PM_MenuHMargin:
    case PM_MenuVMargin:
	return 0;
    case PM_ScrollBarSliderMin:
	return 10;
    case PM_ExclusiveIndicatorWidth:
    case PM_ExclusiveIndicatorHeight:
	return 17;
    case PM_IndicatorWidth:
    case PM_IndicatorHeight:
	return 17;
    case PM_TabBarTabHSpace:
	return  24;
    case PM_TabBarTabVSpace:
	{
	    int ret = 8;
	    if ( const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option) ) {
		if ( QTabBar::TriangularNorth == tab->shape || QTabBar::TriangularSouth == tab->shape ||  QTabBar::TriangularWest == tab->shape || QTabBar::TriangularEast == tab->shape )
		    ret = 4;
	    }
	    
	    return  ret;
	}
    case PM_TabBarTabOverlap:
    case PM_TabBarTabShiftHorizontal:
    case PM_TabBarTabShiftVertical:
	return 0;
    case PM_TabBarBaseHeight:
	return 2;
    case PM_SliderThickness:
	return 25;
    case PM_SliderControlThickness:
	return 21;
    case PM_SliderLength:
	return 8;
    case PM_SplitterWidth:
	return 6;
    case PM_ButtonShiftHorizontal:
    case PM_ButtonShiftVertical:
	{
	    if ( option && (State_On & option->state ) )
		return 0;
	}
    default:
	return ParentStyle::pixelMetric( metric, option, widget );
    }
}

void L4uStyle2::fillButton( QPainter *p, const QRect &fill, const QPalette &pal, State flags ) const
{
    int w2 = fill.width()  / 2, h2 = fill.height() / 2;
    QColor c1 = pal.color( QPalette::Active, (State_Enabled & flags ? QPalette::Button : QPalette::Midlight) ).lighter( 105 );
    QColor c2 = pal.color( QPalette::Active, (State_Enabled & flags ? QPalette::Button : QPalette::Midlight) ).darker( 111 );
    QColor c3 = pal.color( QPalette::Active, (State_Enabled & flags ? QPalette::Button : QPalette::Midlight) ).darker( 107 );
    c1.setAlphaF( 1.0 );
    c2.setAlphaF( 1.0 );
    c3.setAlphaF( 1.0 );
    
      if ( State_Enabled & flags ) {
	if ( State_On & flags ) {
	    QColor mi = pal.color( QPalette::Mid );
	    mi.setAlphaF( 1.0 );
	    c1 = alphaBlendColors( c1, mi.lighter(105), 180 );
	    c2 = alphaBlendColors( c2, mi.darker(111), 180 );
	    c3 = alphaBlendColors( c3, mi.darker(107), 180 );
	}
	else if ( (State_MouseOver & flags) && !(State_Sunken & flags) ) {
	    QColor hl = pal.color( QPalette::Highlight );
	    hl.setAlphaF( 1.0 );
	    c1 = alphaBlendColors( c1,  hl.lighter(150), 200 );
	    c2 = alphaBlendColors( c2,  hl.lighter(150), 200 );
	    c3 = alphaBlendColors( c3,  hl.lighter(150), 200 );
	}
	
	if ( State_Horizontal & flags ) {
	    if ( State_Sunken & flags ) {
		drawSurface( p, QRect(fill.left(), fill.top(), fill.width(), h2), c2, c1 );
		p->fillRect( fill.left(), fill.top() + h2, fill.width(), fill.height() - h2, c1 );
	    }
	    else {
		drawSurface( p, QRect( fill.left(), fill.top(), fill.width(), h2), c1, c3 );
		p->fillRect( fill.left(), fill.top() + h2, fill.width(), fill.height() - h2, c2 );
	    }
	}
	else {
	    if ( State_Sunken & flags ) {
		p->fillRect( fill.left(), fill.top(), w2, fill.height(), c2 );
		drawSurface( p, QRect(fill.left() + w2, fill.top(), fill.width() - w2, fill.height()), c1, c3, false );
	     }
	     else {
		 drawSurface( p, QRect(fill.left(), fill.top(), w2, fill.height()), c1, c3, false );
		 p->fillRect( fill.left() + w2, fill.top(), fill.width() - w2, fill.height(), c2 );
	     }
	}
    }
    else {
	if ( State_Horizontal & flags ) {
	    drawSurface( p, QRect( fill.left(), fill.top(), fill.width(), h2), c1, c3 );
	    p->fillRect( fill.left(), fill.top() + h2, fill.width(), fill.height() - h2, c2 );
	}
	else {
	    drawSurface( p, QRect( fill.left(), fill.top(), w2, fill.height()), c1, c3, FALSE );
	    p->fillRect( fill.left() + w2, fill.top(), fill.width() - w2, fill.height(), c2 );
	}
    }
}

void L4uStyle2::drawButton( QPainter *p, const QRect &rect, const QPalette &pal, State state ) const
{ 
    p->save();
    QPainterPath rc = roundRectPath( rect, 5 );
    bool rnd =  isRnd(rect.width(), rect.height() );
    bool sunken = ((State_On | State_Sunken) & state);
    QColor dark = pal.color( QPalette::Active, QPalette::Dark );
 
    if ( rnd ) {
	p->setRenderHint( QPainter::Antialiasing, true );
	p->setClipPath( rc );
    }
    
    fillButton( p, rect.adjusted(2, 2, -2, -2), pal, state );
    p->setBrush( Qt::NoBrush );
    
     if ( rnd ) {
	 p->setClipping( false );
	 dark.setAlphaF( 0.7 );
	 
	 if ( sunken ) {  
	     p->setPen( dark );
	     p->drawPath( roundRectPathF((QRectF(rect)).adjusted(1.0, 1.0, -1.0, -1.0), 5) );
	     dark.setAlphaF( 1.0 );
	 }
	 else {
	     QColor mi = pal.midlight().color().darker(105);
	     
	     if ( state & State_MouseOver )
		 mi = alphaBlendColors( mi,  pal.color(QPalette::Highlight).lighter(150), 200 );
	     
	     p->setPen( mi );
	     p->drawPath( roundRectPathF((QRectF(rect)).adjusted(1.0, 1.0, -1.0, -1.0), 5) );
	 }
	 
	 if ( (State_HasFocus & state) && !sunken ) {
	     p->setPen( Qt::red );
	     p->drawPath( roundRectPath(rect.adjusted(1, 1, 0, 0), 4) );
	 }
	 else {
	     p->setPen( dark );
	     p->drawPath(  roundRectPathF((QRectF(rect)).adjusted(0.5, 0.5, -0.5, -0.5), 5) );
	 }
     } 
     else {
	 dark.setAlphaF( 0.7 );	
	 
	 if ( sunken ) {
	     p->setPen( dark );
	     p->drawRect( rect.adjusted(1, 1, -1, -1) );
	     dark.setAlphaF( 1.0 );
	 } 
	 else {
	     QColor mi = pal.midlight().color().darker(105);
	     
	     if ( state & State_MouseOver )
		 mi = alphaBlendColors( mi,  pal.color(QPalette::Highlight).lighter(150), 200 );
	     
	     p->setPen( mi );
	     drawContour( p, rect.left() + 1, rect.top() + 1, rect.right() - 1, rect.bottom() - 1 );
	 }
   
	 if ( (State_HasFocus & state) && !sunken )
	     p->setPen( Qt::red );
	 else
	     p->setPen( dark );
	 
	 drawContour( p, rect.left(), rect.top(), rect.right(), rect.bottom() );  
     }
     
     p->restore();
}

void L4uStyle2::drawButtonDropDown( QPainter *p, const QRect &rect, const QPalette &pal, State state ) const
{
    p->save();
    QColor dark = pal.color( QPalette::Dark );
    p->setClipPath( roundTabPath( rect, QTabBar::RoundedEast ) );
    p->setBrush( Qt::NoBrush );
    p->setRenderHint( QPainter::Antialiasing, true );
    fillButton( p, rect.adjusted(2, 2, -2, -2), pal, state );
    p->setClipping( false );
    dark.setAlphaF( 0.7 );
	 
    if ( state & (State_On | State_Sunken) ) {  
	p->setPen( dark );
	p->drawPath( roundTabPathF((QRectF(rect)).adjusted(1.0, 1.0, -1.0, -1.0), QTabBar::RoundedEast) );
	dark.setAlphaF( 1.0 );
    }
    else {
	QColor mi = pal.midlight().color().darker(105);
	
	if ( state & State_MouseOver )
	    mi = alphaBlendColors( mi,  pal.color(QPalette::Highlight).lighter(150), 200 );
	
	p->setPen( mi );
	p->drawPath( roundTabPathF((QRectF(rect)).adjusted(1.0, 1.0, -1.0, -1.0), QTabBar::RoundedEast) );
    }
    
    p->setPen( dark );
    p->drawPath(  roundTabPathF((QRectF(rect)).adjusted(0.5, 0.5, -0.5, -0.5), QTabBar::RoundedEast) );
    
    p->restore();
}

void L4uStyle2::drawSlider( QPainter *p, const QRect &rect, const QPalette &pal, State state ) const
{ 
    int w2 = (rect.width() - 2) / 3, h2 = (rect.height() - 2)  / 3;
    QColor c1 = pal.color( QPalette::Active, (State_Enabled & state ? QPalette::Button : QPalette::Midlight) ).lighter( 102 );
    QColor c2 = pal.color( QPalette::Active, (State_Enabled & state ? QPalette::Button : QPalette::Midlight) ).darker( 108 );
    QColor c3 = pal.color( QPalette::Active, (State_Enabled & state ? QPalette::Button : QPalette::Midlight) ).darker( 104 );
    
    if ( State_Enabled & state ) {
	if ( (State_On | State_Sunken)  & state ) {
	    c1 = alphaBlendColors( c1, pal.color(QPalette::Mid).lighter(102), 180 );
	    c2 = alphaBlendColors( c2, pal.color(QPalette::Mid).darker(108), 180 );
	    c3 = alphaBlendColors( c3, pal.color(QPalette::Mid).darker(104), 180 );
	}
	else if ( State_MouseOver & state ) {
	    QColor highlight = pal.color( QPalette::Highlight );
	    c1 = alphaBlendColors( c1,  pal.color(QPalette::Highlight).lighter(130), 200 );
	    c2 = alphaBlendColors( c2,  pal.color(QPalette::Highlight).lighter(130), 200 );
	    c3 = alphaBlendColors( c3,  pal.color(QPalette::Highlight).lighter(130), 200 );
	}
    }
	
    if ( State_Horizontal & state ) {
	drawSurface( p, QRect( rect.left() + 1, rect.top() + 1, rect.width()  - 2, h2),  c1, c3 );
	drawSurface( p, QRect( rect.left() + 1, rect.top() + h2 + 1, rect.width() - 2, rect.height() - h2 - 2 ), c2,  c3 );
    }
    else {
	drawSurface( p, QRect( rect.left() + 1, rect.top() + 1, w2, rect.height() - 2 ), c1, c3, FALSE );
	drawSurface( p, QRect( rect.left() + w2 + 1, rect.top() + 1, rect.width() - w2 - 2, rect.height() - 2 ), c2,  c3, FALSE );
    }
	
    if ( State_Enabled & state ) {
	p->setPen( State_Sunken & state ? c3 : c1 );
	drawContour( p, rect.left() + 1, rect.top() + 1, rect.right() - 1, rect.bottom() - 1, DrawLeft | DrawTop | DrawRight | DrawBottom );
    } 
    
    QColor dark = pal.color( QPalette::Active, QPalette::Dark );
    
    if ( (State_On | State_Sunken) & state )
	dark.setAlphaF( 0.6 );
    else
	dark.setAlphaF( 0.3 );
    
    p->setPen( dark );
    drawContour( p, rect.left(), rect.top(), rect.right(), rect.bottom() );    
}

void L4uStyle2::drawContour( QPainter *p,  int l, int t, int r, int b, int how ) const
{
    if ( (r - l > 4) && ( b - t > 4) ) {
	if ( how & RoundUpperLeft ) {
	    p->drawPoint( l + 1, t );
	    p->drawPoint( l, t + 1 );
	}
	
	if ( how & RoundUpperRight ) {
	    p->drawPoint( r - 1, t );
	    p->drawPoint( r, t + 1 );
	}
	
	if ( how & RoundLowerLeft ) {
	    p->drawPoint( l + 1, b );
	    p->drawPoint( l, b - 1 );
	}
	
	if ( how & RoundLowerRight ) {
	    p->drawPoint( r - 1, b );
	    p->drawPoint( r, b - 1 );
	}
	
	
	if ( how & DrawTop ) {
	    int l2 = l, r2 = r;
	    
	    if ( how & RoundUpperLeft )
		l2 += 2;
	    if ( how & RoundUpperRight )
		r2 -= 2;
	    
	    p->drawLine( l2, t, r2, t );
	}
	
	if ( how & DrawLeft ) {
	    int t2 = t, b2 = b;
	    
	    if ( how & RoundUpperLeft )
		t2 += 2;
	    if ( how & RoundLowerLeft )
		b2 -= 2;
	    
	    p->drawLine( l, t2, l, b2 );
	}
	
	if ( how & DrawBottom ) {
	    int l2 = l, r2 = r;
	    
	    if ( how & RoundLowerLeft )
		l2 += 2;
	    if ( how & RoundLowerRight )
		r2 -= 2;
	    
	    p->drawLine( l2, b, r2, b );
	}
	
	if ( how & DrawRight ) {
	    int t2 = t, b2 = b;
	    
	    if ( how & RoundUpperRight )
		t2 +=2;
	    if ( how & RoundLowerRight )
		b2 -= 2;
	    
	    p->drawLine( r, t2, r,  b2 );
	}
    }
    else
	p->drawRect( l, t, r - l + 1, b - t + 1 );
}

void L4uStyle2::drawLinearGrad( QPainter *p, const QRect &fill, 
			     const QColor &first, const QColor &last, bool horz ) const
{	
    QLinearGradient lg;
    lg.setStart( fill.topLeft() );
    lg.setFinalStop( horz ?  fill.topRight() : fill.bottomLeft() );
    lg.setColorAt( 0.1, first );
    lg.setColorAt( 1.0, last );
    p->fillRect( fill, lg );
}

QPainterPath L4uStyle2::roundRectPath( const QRect &fill, int radius ) const
{
    int diam = 2 * radius;
    int left, top, right, bottom;
    fill.getCoords( &left, &top, &right, &bottom );
    
    QPainterPath path;
    path.moveTo( left + radius, top );
    path.arcTo( QRect(left, top, diam, diam), 90.0, +90.0 );
    path.lineTo( left, bottom - radius );
    path.arcTo( QRect(left, bottom - diam, diam, diam), 180.0, +90.0 );
    path.lineTo( right - radius, bottom );
    path.arcTo( QRect(right - diam, bottom - diam, diam, diam), 270.0, +90.0 );
    path.lineTo( right, top + radius );
    path.arcTo( QRect( right - diam, top, diam, diam), 0.0, +90.0 );
   
    path.closeSubpath();
    return path;
 }

QPainterPath L4uStyle2::roundRectPathF( const QRectF &fill, qreal radius ) const
{
    qreal diam = 2 * radius;
    qreal left, top, right, bottom;
    fill.getCoords( &left, &top, &right, &bottom );
    
    QPainterPath path;
    path.moveTo( left + radius, top );
    path.arcTo( QRectF(left, top, diam, diam), 90.0, +90.0 );
    path.lineTo( left, bottom - radius );
    path.arcTo( QRectF(left, bottom - diam, diam, diam), 180.0, +90.0 );
    path.lineTo( right - radius, bottom );
    path.arcTo( QRectF(right - diam, bottom - diam, diam, diam), 270.0, +90.0 );
    path.lineTo( right, top + radius );
    path.arcTo( QRectF(right - diam, top, diam, diam), 0.0, +90.0 );
   
    path.closeSubpath();
    return path;
 }

QPainterPath L4uStyle2::roundTabPath( const QRect &fill, QTabBar::Shape shape ) const
{
    static const int radius = 5;
    static const int diam = 2 * radius;
    int left, top, right, bottom;
    fill.getCoords( &left, &top, &right, &bottom );
    QPainterPath path;
    
    switch ( shape ) {
    case QTabBar::RoundedNorth:
	  path.moveTo( right, top + radius );
	  path.arcTo( QRect( right - diam, top, diam, diam), 0.0, +90.0 );
	  path.lineTo( left + radius, top );
	  path.arcTo( QRect(left, top, diam, diam), 90.0, +90.0 );
	  path.lineTo( left, bottom );
	  path.lineTo( right, bottom );
	  break;
      case QTabBar::RoundedSouth:
	  path.moveTo( left, top );
	  path.lineTo( left, bottom - radius );
	  path.arcTo( QRect(left, bottom - diam, diam, diam), 180.0, +90.0 );
	  path.lineTo( right - radius, bottom );
	  path.arcTo( QRect(right - diam, bottom - diam, diam, diam), 270.0, +90.0 );
	  path.lineTo( right, top );
	  break;
      case QTabBar::RoundedWest:
	  path.moveTo( right, top );
	  path.lineTo( left + radius, top );
	  path.arcTo( QRect(left, top, diam, diam), 90.0, +90.0 );
	  path.lineTo( left, bottom - radius );
	  path.arcTo( QRect(left, bottom - diam, diam, diam), 180.0, +90.0 );
	  path.lineTo( right, bottom );
	  break;
      case QTabBar::RoundedEast:
	  path.moveTo( left, top );
	  path.lineTo( left, bottom );
	  path.lineTo( right - radius, bottom );
	  path.arcTo( QRect(right - diam, bottom - diam, diam, diam), 270.0, +90.0 );
	  path.lineTo( right, top + radius );
	  path.arcTo( QRect( right - diam, top, diam, diam), 0.0, +90.0 );
      default:
	break;
    }
    
    path.closeSubpath();
    return path;
}

QPainterPath L4uStyle2::roundTabPathF( const QRectF &fill, QTabBar::Shape shape, bool close ) const
{
    static const qreal radius = 5;
    static const qreal diam = 2 * radius;
    qreal left, top, right, bottom;
    fill.getCoords( &left, &top, &right, &bottom );
    QPainterPath path;
    
    switch ( shape ) {
    case QTabBar::RoundedNorth:
	path.moveTo( right, bottom );
	path.lineTo( right, top + radius );
	path.arcTo( QRectF( right - diam, top, diam, diam), 0.0, +90.0 );
	path.lineTo( left + radius, top );
	path.arcTo( QRectF(left, top, diam, diam), 90.0, +90.0 );
	path.lineTo( left, bottom );
	break;
    case QTabBar::RoundedSouth:
	path.moveTo( left, top );
	path.lineTo( left, bottom - radius );
	path.arcTo( QRectF(left, bottom - diam, diam, diam), 180.0, +90.0 );
	path.lineTo( right - radius, bottom );
	path.arcTo( QRectF(right - diam, bottom - diam, diam, diam), 270.0, +90.0 );
	path.lineTo( right, top );
	break;
    case QTabBar::RoundedWest:
	path.moveTo( right, top );
	path.lineTo( left + radius, top );
	path.arcTo( QRectF(left, top, diam, diam), 90.0, +90.0 );
	path.lineTo( left, bottom - radius );
	path.arcTo( QRectF(left, bottom - diam, diam, diam), 180.0, +90.0 );
	path.lineTo( right, bottom );
	break;
    case QTabBar::RoundedEast:
	path.moveTo( left, bottom );
	path.lineTo( right - radius, bottom );
	path.arcTo( QRectF(right - diam, bottom - diam, diam, diam), 270.0, +90.0 );
	path.lineTo( right, top + radius );
	path.arcTo( QRectF( right - diam, top, diam, diam), 0.0, +90.0 );	
	path.lineTo( left, top );	
	  
      default:
	break;
    }
    
    if ( close )
	path.closeSubpath();
    
    return path;
}

void L4uStyle2::drawSurface( QPainter *p, const QRect &fill, 
			     const QColor &top, const QColor &bot, bool horz, bool bar ) const
{
    if ( fill.width() > 0 && fill.height() > 0 ) {
	if ( top == bot )
	    p->fillRect( fill, top );
	else {
	    int pw = horz ? 10 : fill.width();
	    int ph = horz ? fill.height() : 10;
	    QString pmkey;
	    QPixmap pixmap( pw, ph );
	    pmkey.sprintf( "cc:%x-%x-%x-%x-%s",  top.rgb(), bot.rgb(), pw, ph, (horz ? "horz" : "vert") );
	    
	    if ( !UsePixmapCache || !QPixmapCache::find(pmkey, pixmap) ) {
		QPainter pp( &pixmap );	
		QLinearGradient lg;
		QRect pr = pixmap.rect();
		lg.setStart( pr.topLeft() );
		
		if ( !bar )
		    lg.setFinalStop( horz ? pr.bottomLeft() : pr.topRight() );
		else
		    lg.setFinalStop( pr.bottom(), pr.right() + pr.width() * 2 );
		
		lg.setColorAt( 0.0, top );
		lg.setColorAt( 1.0, bot );
		pp.fillRect( pr, lg );
		pp.end();
		
		if ( UsePixmapCache )
		    QPixmapCache::insert( pmkey, pixmap );
	    }
	    
	    p->drawTiledPixmap(  fill, pixmap );
	}
    }
}

void L4uStyle2::drawBoldPoint( QPainter  *p, const QColor &dark, const QColor &light, int x, int y, bool horz) const
{
    if ( !horz ) {
	p->setPen( dark);	
	p->drawPoint( x, y);
	p->drawPoint( x + 1, y );
	p->drawPoint( x, y + 1 );
	p->setPen( light );
	p->drawPoint( x + 1, y + 1 );
	p->drawPoint( x,  y + 2 );
	p->drawPoint( x + 1, y + 2 );
    }
    else {
	p->setPen( dark);	
	p->drawPoint( x, y);
	p->drawPoint( x + 1, y );
	p->drawPoint( x, y + 1 );
	p->setPen( light );
	p->drawPoint( x + 1, y + 1 );
	p->drawPoint( x + 2,  y );
	p->drawPoint( x + 2, y + 1 );
    }
}
