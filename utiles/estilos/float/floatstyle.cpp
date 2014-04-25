/****************************************************************************
**
** Copyright ( C ) 2006 Matias Costa
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file COPYING included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "floatstyle.h"
#include "helpers.h"
//enum FloatStyle::Position {Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left, TopLeft};


// -- Style Plugin Interface -------------------------
/*!
 * \brief Clase contenedora del plugin de estilo personalizado
 */
class FloatStylePlugin : public QStylePlugin
{
public:
	FloatStylePlugin( QObject *parent=0 ):QStylePlugin( parent ) {}
	~FloatStylePlugin() {}

	QStringList keys() const
	{
		#ifndef DEVEL
		return QStringList() << "Float";
		#else
		return QStringList() << "test";
		#endif
	}

	QStyle* create( const QString& key )
	{

		#ifndef DEVEL
		if ( key == "float" )
		#else
		if ( key == "test" )
		#endif
			return new FloatStyle;
		return 0;
	}
};

Q_EXPORT_PLUGIN2(FloatStyle, FloatStylePlugin )



FloatStyle::FloatStyle() : QWindowsStyle(),
m_button( ":/images/button.png", "png" ),
m_buttonNS( ":/images/button_ns.png", "png" ),
m_toolButton( ":/images/toolbutton.png", "png" ),
m_check( ":/images/check.png", "png" ),
m_radio( ":/images/radio.png", "png" ),
m_box( ":/images/box.png", "png" ),
m_groupbox( ":/images/groupbox.png", "png" ),
m_slider( ":/images/slider.png", "png" ),
m_dockBase( ":/images/dock_base.png", "png" ),
m_dockTitle( ":/images/dock_title.png", "png" ),
m_tabSelected( ":/images/tab_selected.png", "png" ),
m_tabSelectedS( ":/images/tab_selectedS.png", "png" ),
m_tabFrame( ":/images/tab_frame.png", "png" ),
m_arrowU( ":/images/arrow_u.png", "png" ),
m_arrowD( ":/images/arrow_d.png", "png" ),
m_arrowL( ":/images/arrow_l.png", "png" ),
m_arrowR( ":/images/arrow_r.png", "png" ),
m_arrowUNS( ":/images/arrow_uns.png", "png" ),
m_arrowDNS( ":/images/arrow_dns.png", "png" ),
m_arrowLNS( ":/images/arrow_lns.png", "png" ),
m_arrowRNS( ":/images/arrow_rns.png", "png" ),
m_close( ":/standard/DockWidgetCloseButton.png", "png" ),
m_dockNormalButton( ":/standard/DockNormalButton.png", "png" ),
m_titleBarCloseButton( ":/standard/TitleBarCloseButton.png", "png" ),
m_titleBarMaxButton( ":/standard/TitleBarMaxButton.png", "png" ),
m_titleBarNormalButton( ":/standard/TitleBarNormalButton.png", "png" ),
m_titleBarMinButton( ":/standard/TitleBarMinButton.png", "png" ),
m_titleBarShadeButton( ":/standard/TitleBarShadeButton.png", "png" ),
m_titleBarUnshadeButton( ":/standard/TitleBarUnshadeButton.png", "png" )
{
	m_titleBarColor1 = QColor( 157, 179, 204 );
	m_titleBarColor2 = QColor( 255, 206, 12 );
}

FloatStyle::~FloatStyle(){
}

// QIcon FloatStyle::standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,const QWidget *widget) const
// {
// 	QIcon ret = QWindowsStyle::standardIconImplementation( standardIcon, option, widget );
// // 	if ( standardIcon == SP_TitleBarCloseButton ) {
// // 		qDebug() << "SP_TitleBarCloseButton" << ret.isNull();
// // 	} else
// 		qDebug() << "stdIcon" << standardIcon << ret.isNull();
// 	return ret;
// }

// void FloatStyle::drawItemPixmap ( QPainter * painter, const QRect & rect, int alignment, const QPixmap & pixmap ) const
// {
// 	qDebug() << pixmap.height() << pixmap.width();
// 	painter->drawPixmap(rect.x(), rect.y(), pixmap );
//
// }

QPixmap FloatStyle::standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt, const QWidget *widget) const
{

	switch ( standardPixmap ) {
		case SP_DockWidgetCloseButton: return m_close;
		case SP_TitleBarMinButton: return m_titleBarMinButton;
		case SP_TitleBarShadeButton: return m_titleBarShadeButton;
		case SP_TitleBarUnshadeButton: return m_titleBarUnshadeButton;
		case SP_TitleBarMaxButton: return m_titleBarMaxButton;
		case SP_TitleBarNormalButton:
			if ( widget ) if ( qobject_cast<const QDockWidget*>(widget) ) return m_dockNormalButton;
			return m_titleBarNormalButton;
		case SP_TitleBarCloseButton:
			if ( widget ) if( qobject_cast<const QDockWidget*>(widget) ) return m_close;
			return m_titleBarCloseButton;
		default:
			return QWindowsStyle::standardPixmap( standardPixmap, opt, widget );
	}
}


void FloatStyle::renderButton( const QStyleOption *o, QPainter *p ) const
{
	const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(o);
	if ( !button ) return;
	const bool sunken = (button->state & State_Sunken) || (button->state & State_On);
	const bool flat = button->features & QStyleOptionButton::Flat;
 	if ( flat && !sunken ) return;
	p->save();
	if( !(o->state & State_Enabled) ) p->setBrush( o->palette.background() );
	else p->setBrush( o->palette.button() );
	// rect is the full button area (with shadow), reduced if sunken.
	// adjusted is the real button area
	QRect rect = o->rect, adjusted = o->rect.adjusted( 3,3, -3,-3 );
	const QPixmap*  buttonPm;//  = buttonNS;
	QPixmap tmp;
	QMatrix rotation = QMatrix().rotate( 180 );
	if ( flat ) {
 		adjusted.translate( 3,3 );
		// adjust to prevent antialias getting cliped out
		adjusted.adjust( -1,-1, -1,-1 );
		rect = adjusted;
		tmp = m_buttonNS.transformed( rotation );
		buttonPm = &tmp;
	} else if( sunken || !(o->state & State_Enabled) ) {
		adjusted.translate( 3,3 );
		// adjust to prevent antialias getting cliped out
		adjusted.adjust( 0,0, -1,-1 );
		rect = adjusted;
		buttonPm = &m_buttonNS;
	} else {
		buttonPm = &m_button;
	}
// 	p->setPen( Qt::green );
	if ( !flat ) {
		// Choose color
		const QPolygon base = buttonBorder( adjusted );
		QColor penColor;
		if ( o->state & State_MouseOver ) penColor = o->palette.highlight().color();
		else penColor = outlineColor( o->palette.button().color() );

		QPen pen( penColor );
		pen.setWidth( 0 );
		p->setPen( pen );
		p->setRenderHint( QPainter::Antialiasing, true );
 		p->drawPolygon( base );
		p->setRenderHint( QPainter::Antialiasing, false );
		p->setBrush( Qt::NoBrush );
		p->drawPolygon( base );
	}
  	renderTiled( rect, p, *buttonPm );
	p->restore();
}

void FloatStyle::renderBox( const QStyleOption *o, QPainter *p, bool frame ) const
{
 	QRect rect2 = frame? o->rect.adjusted( 0, 2, 0, 0 ) : o->rect ;
	renderTiled( rect2, p, m_box );
	p->save();

	QPen pen;
	// frames never are highlighted
	if( o->state & State_HasFocus && !frame ) pen.setColor( o->palette.highlight().color() );
	else pen.setColor( outlineColor( o->palette.button().color() ) );

	pen.setWidth( 0 );
	pen.setJoinStyle( Qt::MiterJoin );
	p->setPen( pen );
	p->drawRect( box( o->rect, frame ) );
	p->restore();
}

void FloatStyle::drawPrimitive( PrimitiveElement elem, const QStyleOption *o, QPainter *p, const QWidget *widget ) const
{


	switch( elem ){
	case PE_PanelButtonCommand:
		renderButton( o, p );
		break;
	case PE_IndicatorArrowLeft:
		if( o->state & (State_On | State_Sunken) ) drawPixmapCentered( o->rect.adjusted( 1,2,0,0 ), p, m_arrowLNS );
		else drawPixmapCentered( o->rect, p, m_arrowL );
		break;
	case PE_IndicatorArrowRight:
		if( o->state & (State_On | State_Sunken) ) drawPixmapCentered( o->rect.adjusted( 1,2,0,0 ), p, m_arrowRNS );
		else drawPixmapCentered( o->rect, p, m_arrowR );
		break;
	case PE_IndicatorArrowDown:
	case PE_IndicatorSpinDown:
        if( o->state & (State_On | State_Sunken) ) drawPixmapCentered( o->rect.adjusted( 1,-2,0,0 ), p, m_arrowDNS );
		else drawPixmapCentered( o->rect, p, m_arrowD );
		break;
	case PE_IndicatorArrowUp:
	case PE_IndicatorSpinUp:
		if( o->state & (State_On | State_Sunken) ) drawPixmapCentered( o->rect.adjusted( 1,2,0,0 ), p, m_arrowUNS );
		else drawPixmapCentered( o->rect, p, m_arrowU );
		break;
	case PE_IndicatorMenuCheckMark: {
		p->save();
		int x1, y1, x2, y2;
		o->rect.getCoords( &x1, &y1, &x2, &y2 );
		QPen pen;
		pen.setColor( o->palette.color(QPalette::Highlight) );
		pen.setWidth( 2 );
		pen.setCapStyle( Qt::FlatCap );
		p->setPen( pen );
		p->setRenderHint( QPainter::Antialiasing, true );
		p->drawLine ( x1+2,y1+2,  x2,y2 );
		p->drawLine ( x1+2,y2,  x2,y1+2 );
		pen.setColor( Qt::white );
		pen.setWidth( 0 );
		p->setRenderHint( QPainter::Antialiasing, false );
		p->setPen( pen );
		p->setBrush( Qt::NoBrush );
		p->drawRect( x1+1,y1+1, x2-x1-1,y2-y1-1 );
		p->restore();
		break;
	}
	case PE_IndicatorRadioButton: {

		p->drawPixmap( o->rect.topLeft(), m_radio );

		p->save();
		QRect ir = o->rect;
		p->setRenderHint( QPainter::Antialiasing, true );
		ir.adjust( 2, 2, -4, -4 );
		p->setPen( outlineColor( o ) );
		p->setBrush( o->palette.base() );
		p->drawEllipse( ir );

		if (o->state & (State_Sunken | State_On)) {
			ir.adjust(2, 2, -2, -2);
			p->setPen( Qt::NoPen );
			p->setBrush( o->palette.highlight() );
			p->drawEllipse(ir);
		}
		p->restore();
		break;
	}
	case PE_IndicatorViewItemCheck:
	case PE_IndicatorCheckBox: {
 		p->drawPixmap( o->rect.topLeft(), m_check );

		p->save();
		int x = o->rect.left(), y = o->rect.top();
		if ( o->state & State_On ) {
			QPen pen;
			pen.setColor( o->palette.color(QPalette::Highlight) );
			pen.setWidth( 2 );
			pen.setCapStyle( Qt::FlatCap );
			p->setPen( pen );
			p->setRenderHint( QPainter::Antialiasing, true );
			p->drawLine ( x+5,y +5, x+12,y+12 );
			p->drawLine ( x+5,y+12, x+12,y +5 );
 			pen.setColor( Qt::white );
			pen.setWidth( 0 );
			p->setRenderHint( QPainter::Antialiasing, false );
 			p->setPen( pen );
			p->drawRect( x+4,y+4, 8,8 );
		} else if ( o->state & State_NoChange ) {// tristate
			p->setBrush( o->palette.highlight() );
			p->setPen( Qt::NoPen );
			p->drawRect ( x+5,y+5, 7,7 );
		}
		p->restore();
		break;
	}
	case PE_IndicatorBranch: {
		if (o->state & State_Children) {
			p->save();
			// These height and width values refer to the arrow looking right
			// So, when loking down must be swapped
			const int w = 4, h = 8;
			p->setPen( Qt::NoPen );
			p->setBrush( o->palette.foreground() );
			QPolygon arrow( 3 );
			if (o->state & State_Open) {
				const int x = o->rect.x() +(o->rect.width()-h)/2, y = o->rect.y()+ (o->rect.height()-w)/2;
				arrow.putPoints( 0, 3,
						x, y,
						x+h, y,
						x+h/2, y+w );
			} else {
				const int x = o->rect.x() +(o->rect.width()-w)/2, y = o->rect.y()+ (o->rect.height()-h)/2 -1;
				arrow.putPoints( 0, 3,
						x, y,
						x+w, y +h/2,
						x, y+h );
			}
			p->drawPolygon( arrow );
			p->restore();
		}
		break;
	}
	case PE_IndicatorButtonDropDown:
	case PE_PanelButtonTool: {
	// HACK We do not paint QDockWidgetTittle buttons
		if( widget ) {
 			if ( widget->parentWidget() ) {
				if ( widget->parentWidget()->parentWidget() )
					if ( strcmp( widget->parentWidget()->parentWidget()->metaObject()->className(), "QMenuBar" ) == 0 ) return;
			}
			if( strcmp( widget->metaObject()->className(), "QDockWidgetTitleButton" ) == 0 ) {
				return;
			}
		}
		p->save();

		const QRect rect = (elem == PE_IndicatorButtonDropDown) ?
				o->rect.adjusted( -1,1, -1,-1 ) : o->rect.adjusted( 1,1, -1,-1 );
		p->setPen( outlineColor( o ) );
		// Differentiate between normal and pressed
		QColor light = o->palette.background().color();
		QColor shadow = alternateColor( light );
		QLinearGradient linearGrad( o->rect.left(), o->rect.top(), o->rect.left(), o->rect.bottom() );
		if ( o->state & (State_Sunken|State_On) ) {
			linearGrad.setColorAt(0, shadow);
			linearGrad.setColorAt(1, light);
		} else {
			linearGrad.setColorAt(0, light);
			linearGrad.setColorAt(1, shadow);
		}
		p->setBrush( QBrush( linearGrad ) );
		p->setRenderHint( QPainter::Antialiasing, true );
		p->drawRect( rect );

		p->restore();
		break;
	}
	case PE_FrameDefaultButton:
		if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(o)) {
			const bool sunken = (button->state & State_Sunken) || (button->state & State_On);
			const bool flat = button->features & QStyleOptionButton::Flat;
			p->save();
			p->setBrush( Qt::NoBrush );
			QRect rect = o->rect.adjusted( 3,3, -3,-3 );
			if ( flat ) rect.adjust( 1,1, -2,-2 );
			if( ( sunken || !(o->state & State_Enabled) ) ) {
				rect.translate( 3,3 );
				// adjust to prevent antialias getting cliped out
 				if ( !flat ) rect.adjust( 0,0, -1,-1 );
			}
			const QPolygon base = buttonBorder( rect );
			QColor penColor( o->palette.highlight().color() );
			if ( flat ) {
				penColor.setAlphaF( 0.4 );
// 				rect.adjust( 0,0, 2,2 );
			}
			QPen pen( penColor );
			pen.setWidth( flat? 2 : 3 );
			p->setPen( pen );
			p->setRenderHint( QPainter::Antialiasing, true );
			p->drawPolygon( base );
			p->restore();
	}
	break;
	case PE_FrameFocusRect:
		if (const QStyleOptionFocusRect *focusFrame = qstyleoption_cast<const QStyleOptionFocusRect *>(o)) {
			if (!(focusFrame->state & State_KeyboardFocusChange))
				break;
			QRect rect = focusFrame->rect;
			p->save();
			p->setBrush( Qt::NoBrush );
			QPen pen( o->palette.highlight().color().dark(150) );
			pen.setStyle( Qt::DotLine );
			pen.setWidth( 0 );
			p->setPen( pen );
			p->drawRect( focusFrame->rect.adjusted( 0,0, -1,-1 ) );
			p->restore();
		}
		break;
	case PE_FrameDockWidget:
	case PE_FrameWindow:
		p->save();
		p->setBrush( Qt::NoBrush );
		p->setPen( (o->state & State_HasFocus ) ?
				o->palette.highlight().color() : outlineColor( o->palette.button().color() ) );
		p->drawRect( o->rect.adjusted( 0,0, -1,-1 ) );
		p->restore();
		break;
	case PE_FrameTabBarBase:
	case PE_FrameTabWidget:	{
// 		qDebug() << o->rect;
		// HACK detect height to fix strange behaviour in mainwindow qt demo
		p->save();
		if ( o->rect.height() < 5 ) {
// 			p->fillRect( o->rect, outlineColor( o ) );
			p->setPen( outlineColor( o ) );
			p->drawLine( o->rect.topRight(), o->rect.topLeft() );
		} else {
			QRect rect2 = o->rect.adjusted( 0, 2, 0, 0 );
			renderTiled( rect2, p, m_tabFrame );

			QPen pen( outlineColor( o ) );
			pen.setWidth( 0 );
			pen.setJoinStyle( Qt::MiterJoin );
			p->setPen( pen );
			p->drawRect( box( o->rect, true ) );
		}
		p->restore();
	}
	break;
	case PE_Frame:
		p->save();
		p->setBrush( Qt::NoBrush );
		renderBox( o, p, true );
		p->restore();
		break;
	case PE_FrameGroupBox: {
		bool plain = false;
		bool text = true;
		if ( const QGroupBox *gb = qobject_cast<const QGroupBox*>(widget) ) {
			text = !gb->title().isEmpty();
		}
		if (const QStyleOptionFrameV2 *groupBoxO = qstyleoption_cast<const QStyleOptionFrameV2*>(o)) {
			plain = groupBoxO->features & QStyleOptionFrameV2::Flat;
		}

		p->save();

		if ( plain ) {
			QRect headRect( o->rect );
			headRect.setHeight( o->fontMetrics.height() );
/*			QColor headColor( o->palette.highlight().color() );
			headColor.setAlphaF( 0.3 );*/
			QPen pen( outlineColor( o ) );
			pen.setWidth( 1 );
			p->setPen( pen );
// 			p->drawLine( headRect.topLeft(), headRect.bottomLeft() );
			p->drawLine( headRect.bottomLeft(), headRect.bottomRight() );
		} else {
			p->setBrush( Qt::NoBrush );
			renderTiled( o->rect, p, m_groupbox );
			QPolygon border = groupboxBorder( o->rect );
			p->setPen( alphaMix( outlineColor( o ), o->palette.background().color(), 0.9 ) );
// 			p->setRenderHint( QPainter::Antialiasing, true );
			p->drawPolygon( border );

			if ( text ) {
				QRect headRect( o->rect );
				headRect.setHeight( o->fontMetrics.height() +4 );
				// we want to paint inside the border
				QRegion head( headRect );
				head &= border;
				p->setClipRegion( head );
				QColor headColor( o->palette.highlight().color() );
				headColor.setAlphaF( 0.3 );
				p->fillRect( headRect, headColor );
			}
		}
		p->restore();

		break;
	}
	case PE_FrameLineEdit:
		p->save();
		p->setBrush( Qt::NoBrush );
		renderBox( o, p );
		p->restore();
		break;
	case PE_FrameMenu:
		p->save();
		p->setPen( outlineColor( o ) );
		p->drawRect( o->rect.adjusted( 0,0, -1,-1 ) );
		p->restore();
		break;
	case PE_PanelMenuBar:
		break;
	case PE_PanelLineEdit:
		if (const QStyleOptionFrame *frameOption = qstyleoption_cast<const QStyleOptionFrame *>(o)) {
			// Panel of a line edit inside combo box or spin box is drawn in CC_ComboBox and CC_SpinBox
			if (widget) {
				if (qobject_cast<const QComboBox *>(widget->parentWidget())) break;
				if (qobject_cast<const QAbstractSpinBox *>(widget->parentWidget())) break;
			}

			p->save();
			if( frameOption->lineWidth > 0 ) {
				p->setBrush( o->palette.base() );
				renderBox( o, p );
			} else {
				p->fillRect( o->rect, o->palette.base() );
			}
			p->restore();
		}
		break;
	case PE_IndicatorToolBarHandle:
		renderSplitter( o, p, !( o->state & State_Horizontal ) );
		break;
	case PE_IndicatorDockWidgetResizeHandle:{
		renderSplitter( o, p, o->state & State_Horizontal );
		break;
	}


	default:
		QWindowsStyle::drawPrimitive( elem, o, p, widget );
	}
}

int FloatStyle::styleHint ( StyleHint hint, const QStyleOption *o, const QWidget *widget, QStyleHintReturn *returnData ) const
{
	switch( hint ) {
		case SH_MenuBar_MouseTracking:
		case SH_Menu_MouseTracking:
		case SH_ComboBox_ListMouseTracking: return 1;
		case SH_ComboBox_Popup: return 0;
// 		case SH_ComboBox_PopupFrameStyle: return QFrame::NoFrame;
		case SH_ComboBox_PopupFrameStyle: return (QFrame::Box | QFrame::Plain);
// 		case SH_ScrollBar_BackgroundMode: return Qt::PaletteBackground;
		case SH_Menu_SubMenuPopupDelay: return 96;
		case SH_Table_GridLineColor: return outlineColor( o ).rgb();
		default: return QWindowsStyle::styleHint( hint, o, widget, returnData );
	}
}

void FloatStyle::polish(QWidget *widget)
{
	QWindowsStyle::polish( widget );
	if( qobject_cast<QAbstractButton*>(widget)
		   || qobject_cast<QSplitterHandle*>(widget)
		   || qobject_cast<QTabBar*>(widget)
	  ) {
		widget->setAttribute(Qt::WA_Hover);
		widget->setAutoFillBackground( false );
	  }
	if( qobject_cast<QMenuBar*>(widget)  || qobject_cast<QSlider*>(widget) ) {
		widget->setAutoFillBackground( false );
 		widget->setBackgroundRole( QPalette::Window );
	}
//  	if( qobject_cast<QMenu*>(widget) ) widget->setBackgroundRole( QPalette::Highlight );
}

QPalette FloatStyle::standardPalette() const
{
	QPalette palette;
	QColor hl( 80, 114, 154 );
	QColor bg( 239, 235, 231 );
	QColor button( 244, 244, 240 );

	palette.setBrush( QPalette::BrightText, Qt::white );
	palette.setBrush( QPalette::Base, Qt::white );
	palette.setBrush( QPalette::Highlight, hl );
	palette.setBrush( QPalette::Background, bg );
	palette.setBrush( QPalette::Button, button );

	return palette;

}
