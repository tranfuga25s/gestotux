//
// C++ Implementation: sizes
//
// Description: 
//
//
// Author: Matías Costa <mcc3@alu.um.es>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtGui>
#include "floatstyle.h"
#include "helpers.h"

QSize FloatStyle::sizeFromContents( ContentsType type, const QStyleOption *o, const QSize &contentsSize, const QWidget *widget ) const 
{
	QSize size(contentsSize);
	switch( type ) {
	case CT_PushButton:
		size.rwidth() += 36;
		size.rheight() += 12;
		break;
// 	case CT_ComboBox:
	case CT_LineEdit: {
		int lineWidth = 4;
		if (const QStyleOptionFrame *f = qstyleoption_cast<const QStyleOptionFrame *>(o)) lineWidth = f->lineWidth;
		size += QSize( 2*lineWidth, 2*lineWidth);
		break;
	}
	case CT_MenuItem:
		if (const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>(o)) {
			int w = size.width();
			size = QWindowsStyle::sizeFromContents(type, o, contentsSize, widget);

			if (mi->menuItemType == QStyleOptionMenuItem::Separator) {
				size = QSize(10, 10);
			}
			else if (mi->icon.isNull()) {
				size.setHeight(size.height() - 2);
				w -= 6;
			}

			if (mi->menuItemType != QStyleOptionMenuItem::Separator && !mi->icon.isNull())
				size.setHeight(qMax(size.height(),
					     mi->icon.pixmap(pixelMetric(PM_SmallIconSize), QIcon::Normal).height()
							     + 2 * menuItemFrame));
			int maxpmw = mi->maxIconWidth;
			int tabSpacing = 20;
			if (mi->text.contains(QLatin1Char('\t')))
				w += tabSpacing;
			else if (mi->menuItemType == QStyleOptionMenuItem::SubMenu)
				w += 2;
			else if (mi->menuItemType == QStyleOptionMenuItem::DefaultItem) {
                // adjust the font and add the difference in size.
                // it would be better if the font could be adjusted in the getStyleOptions qmenu func!!
				QFontMetrics fm(mi->font);
				QFont fontBold = mi->font;
				fontBold.setBold(true);
				QFontMetrics fmBold(fontBold);
				w += fmBold.width(mi->text) - fm.width(mi->text);
			}

			int checkcol = qMax(maxpmw, 20 ); // Windows always shows a check column
			w += checkcol;
			w += menuRightBorder + 10;
			w = int( w * 1.2 );
			size.setWidth(w);
		}
		break;
	case CT_MenuBarItem:
		size += QSize( menuItemHMargin * 4, menuItemHMargin * 2);
		break;
	case CT_ToolButton:
		size += QSize( 6, 6 );
		break;
	case CT_Slider:
		if ( const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(o) ) {
			const int thickness = pixelMetric(PM_SliderThickness, slider, widget);
			const int tickSize = pixelMetric(PM_SliderTickmarkOffset, slider, widget);

			if (slider->orientation == Qt::Horizontal) size.setHeight( thickness );
			else size.setWidth( thickness );
			
			if (slider->tickPosition & QSlider::TicksBelow) {
				if (slider->orientation == Qt::Horizontal) size.rheight() += tickSize;
				else size.rwidth() += tickSize;
			}
			if (slider->tickPosition & QSlider::TicksAbove) {
				if (slider->orientation == Qt::Horizontal) size.rheight() += tickSize;
				else size.rwidth() += tickSize;
			}
		}
		break;
	case CT_TabBarTab:
		size = QWindowsStyle::sizeFromContents( type, o, contentsSize, widget);
		// This modifies CE_TabBarTabLabel paint rect (tr)
		if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(o)) {
			bool verticalTabs = tab->shape == QTabBar::RoundedEast
					|| tab->shape == QTabBar::RoundedWest
					|| tab->shape == QTabBar::TriangularEast
					|| tab->shape == QTabBar::TriangularWest;
			if (verticalTabs) {
				size.rwidth() += 3;
				size.rheight() += 10;
			} else {
				size.rwidth() += 10;
				size.rheight() += 3;
			}
		}
		break;
	default:
		size = QWindowsStyle::sizeFromContents( type, o, contentsSize, widget);
	}
	return size;
}

QRect FloatStyle::subElementRect ( SubElement element, const QStyleOption *o, const QWidget *widget ) const
{
	QRect r( o->rect );
	switch( element ) {
	case SE_PushButtonContents:
		if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(o)) {
			int dx1, dx2;
			dx1 = pixelMetric(PM_DefaultFrameWidth, btn, widget);
// 			if (btn->features & QStyleOptionButton::AutoDefaultButton)
// 				dx1 += pixelMetric(PM_ButtonDefaultIndicator, btn, widget);
			dx2 = dx1 * 2;
			r.setRect(o->rect.x() + dx1, o->rect.y() + dx1, o->rect.width() - dx2 +1,
				  o->rect.height() - dx2);
			// Shift if disabled
			if( !(o->state & State_Enabled) ) r.translate( 3, 3 );
			r = visualRect(o->direction, o->rect, r);
		}
		break;

	case SE_PushButtonFocusRect:
		r.adjust( 6, 6, -8, -8 );
		if( o->state & State_Sunken || o->state & State_On ) r.translate( 3,3 );
		break;	
	case SE_LineEditContents: {
		int lineWidth = 4;
		if( const QStyleOptionFrame *f = qstyleoption_cast<const QStyleOptionFrame *>(o)) lineWidth = f->lineWidth;
		r.adjust( lineWidth, lineWidth, -lineWidth, -lineWidth);
		r = visualRect( o->direction, o->rect, r );
		break;
	}
	case SE_CheckBoxIndicator: {
		int h = pixelMetric(PM_IndicatorHeight, o, widget);
		r.setRect(o->rect.x(), o->rect.y() + ((o->rect.height() - h) / 2) -1,
			  pixelMetric(PM_IndicatorWidth, o, widget), h);
		r = visualRect(o->direction, o->rect, r);
	}
	break;

	case SE_ProgressBarGroove: 
		r = visualRect( o->direction, o->rect, r );
		break;
 	case SE_ProgressBarLabel:
		if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(o)) {
			int textw = 0;
			if (pb->textVisible)
				textw = pb->fontMetrics.width(QLatin1String("100%")) + 6;

			if (pb->textAlignment == Qt::AlignLeft || pb->textAlignment == Qt::AlignCenter) {
				r = o->rect;
			} else {
				if(element == SE_ProgressBarLabel)
					r.setCoords(o->rect.right() - textw, o->rect.top(),
						o->rect.right(), o->rect.bottom());
				else
					r.setCoords(o->rect.left(), o->rect.top(),
						o->rect.right() - textw, o->rect.bottom());
			}
			r = visualRect(pb->direction, pb->rect, r);
		}
		break;

	case SE_ProgressBarContents:	
		r = box( r );
		r = visualRect(o->direction, o->rect, r);
		r.adjust( 1,1, 0,0 );
// 		const QStyleOptionProgressBarV2 *pb2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(o);
// 		if (pb2) {
// 			if (pb2->orientation == Qt::Vertical) r.adjust( -2,-2, -3,-3 );
// 		} else {
// 			
// 		}
		break;
// 	}
	default:
		r = QWindowsStyle::subElementRect( element, o, widget );
	}
	return r;
}

QRect FloatStyle::subControlRect ( ComplexControl control, const QStyleOptionComplex * o, SubControl subControl, const QWidget * widget ) const
{
	QRect r( o->rect );
	switch( control ) {
	case CC_ComboBox: 
		if (const QStyleOptionComboBox *cbO = qstyleoption_cast<const QStyleOptionComboBox *>(o)) {
			switch( subControl ) {
			// These values are fine tuned by eye-O-meter(tm)
			// First we adjust r if has frame. So embebed ComboBexes work (as the qt designer ones)
			case SC_ComboBoxArrow:
				if( cbO->frame ) {
					r = box( o->rect );
                    r.adjust( 1,1, 0,0 );
				}
				r.setLeft( r.right() -r.height()+4 );
				break;
			case SC_ComboBoxEditField:
				if( cbO->frame ) {
					r = subElementRect( SE_LineEditContents, o, widget  ) ;
					r.setLeft( r.left() +1 );
				}
				r.setWidth( r.width() -r.height()+2 );
				break;
			case SC_ComboBoxListBoxPopup:
// 				r.translate( 0, -2 );
				r.adjust( 3, -2, -3, -2 );
				break;
			default:
				break;
			}
			visualRect( o->direction, o->rect, r);
		}
		break;
	case CC_SpinBox:
		if (const QStyleOptionSpinBox *spinbox = qstyleoption_cast<const QStyleOptionSpinBox *>(o)) {
			QSize bs;
			int fw = spinbox->frame ? pixelMetric(PM_SpinBoxFrameWidth, spinbox, widget) : 0;
			bs.setHeight(qMax(8, spinbox->rect.height()/2 - fw));
			// 1.6 -approximate golden mean
			bs.setWidth(qMax(16, qMin(bs.height() * 8 / 5, spinbox->rect.width() / 4)));
			bs = bs.expandedTo(QApplication::globalStrut());
			int y = fw;
			int x, lx, rx;
			x = spinbox->rect.width() - y - bs.width();
			lx = fw;
			rx = x - fw;
			switch ( subControl ) {
				case SC_SpinBoxUp:
                    r = QRect(x, y-2, bs.width(), bs.height()+1 );
					break;
				case SC_SpinBoxDown:
                    r = QRect(x, y + bs.height()-2, bs.width(), bs.height()+2 );
					break;
				case SC_SpinBoxEditField:
					r = QRect(lx, fw, rx, spinbox->rect.height() - 2*fw);
					break;
				case SC_SpinBoxFrame:
					r = spinbox->rect;
				default:
					break;
			}
			r = visualRect(spinbox->direction, spinbox->rect, r);
		}
		break;
	case CC_GroupBox:
		if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(o)) {
			QFontMetrics fontMetrics = groupBox->fontMetrics;
			int h = fontMetrics.height()+4;

			switch (subControl) {
			case SC_GroupBoxFrame:
				// This sub control just tells where to paint the frame, it is nothing about
				// the widget size. PE_FrameGroupBox knows what this means if the GrupBox is flat.
				if( groupBox->features & QStyleOptionFrameV2::Flat ) 
					r.setHeight( h +4 );
                		break;
			case SC_GroupBoxContents: {
				int topMargin = 0;
				int topHeight = 0;
				int verticalAlignment = styleHint(SH_GroupBox_TextLabelVerticalAlignment, groupBox, widget);
				if (groupBox->text.size()) {
					topHeight = groupBox->fontMetrics.height();
					if (verticalAlignment & Qt::AlignVCenter)
						topMargin = topHeight / 2;
					else if (verticalAlignment & Qt::AlignTop)
						topMargin = topHeight;
				}

				QRect frameRect = groupBox->rect;
              
				if (subControl == SC_GroupBoxFrame) {
					frameRect.setTop(topMargin);
					r = frameRect;
					break;
				}

				int frameWidth = 0;
				if ((groupBox->features & QStyleOptionFrameV2::Flat) == 0)
					frameWidth = pixelMetric(PM_DefaultFrameWidth, groupBox, widget);
				r = frameRect.adjusted(frameWidth, frameWidth + topHeight, -frameWidth, -frameWidth);
				break;
			}
			case SC_GroupBoxCheckBox:
			case SC_GroupBoxLabel: {
				
				int tw = fontMetrics.size(Qt::TextShowMnemonic, groupBox->text + QLatin1Char(' ')).width();
				const int marg = 8;
				r = groupBox->rect.adjusted( marg, 0, -marg, 0);
				r.setHeight( h );
	
				int indicatorWidth = pixelMetric(PM_IndicatorWidth, o, widget);
				int indicatorSpace = pixelMetric(PM_CheckBoxLabelSpacing, o, widget) - 1;
				bool hasCheckBox = groupBox->subControls & QStyle::SC_GroupBoxCheckBox;
				int checkBoxSize = hasCheckBox ? (indicatorWidth + indicatorSpace) : 0;
	
				// Adjusted rect for label + indicatorWidth + indicatorSpace
				QRect totalRect = alignedRect(groupBox->direction, groupBox->textAlignment,
					QSize(tw + checkBoxSize, h), r);
	
				// Adjust totalRect if checkbox is set
				if( hasCheckBox ) {
					bool ltr = groupBox->direction == Qt::LeftToRight;
					int left = 0;
					// Adjust for check box
					if (subControl == SC_GroupBoxCheckBox) {
						int indicatorHeight = pixelMetric(PM_IndicatorHeight, o, widget);
						left = ltr ? totalRect.left() : (totalRect.right() - indicatorWidth);
						int top = totalRect.top() + (fontMetrics.height() - indicatorHeight) / 2;
						totalRect.setRect(left, top-1, indicatorWidth, indicatorHeight);
					// Adjust for label
					} else {
						left = ltr ? (totalRect.left() + checkBoxSize - 2) : totalRect.left();
						totalRect.setRect(left, totalRect.top(),
								totalRect.width() - checkBoxSize, totalRect.height());
					}
				}
				r = totalRect;
				r.translate( 0, 5 );
				break;
			}
			default: break;
		} // if cast
	break;
	}
	case CC_Slider:
		if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(o)) {
			const int thickness = pixelMetric( PM_SliderThickness, slider, widget );
			// a resized shape moves the center moves the half
			const int tickShift = pixelMetric( PM_SliderTickmarkOffset, slider, widget )/2;

			// Calculate center shift
			int centerX = r.center().x();
			int centerY = r.center().y();
			if (slider->tickPosition & QSlider::TicksBelow) {
				if (slider->orientation == Qt::Horizontal) centerY -= tickShift;
				else centerX -= tickShift;
			}
			if (slider->tickPosition & QSlider::TicksAbove) {
				if (slider->orientation == Qt::Horizontal) centerY += tickShift;
				else centerX += tickShift;
			}
			
			switch (subControl) {
			case SC_SliderHandle: {
				int sliderPos = 0;
				int handleThickness = pixelMetric( PM_SliderControlThickness, slider, widget );
				int len = pixelMetric(PM_SliderLength, slider, widget);
				const int frame = pixelMetric( PM_DefaultFrameWidth, slider, widget )-1;
				
				bool horizontal = slider->orientation == Qt::Horizontal;
				// To respect frame: think the groove is smaller and shift the slider
				sliderPos = sliderPositionFromValue(slider->minimum, slider->maximum,
						slider->sliderPosition,
         					(horizontal ? slider->rect.width() : slider->rect.height()) -len -2*frame,
						slider->upsideDown) + frame;
				// HACK rounding errors? shift +1
				if ( horizontal ) r.setRect( slider->rect.x() + sliderPos, centerY -handleThickness/2 +1 , len, handleThickness);
				else r.setRect( centerX -handleThickness/2 +1, slider->rect.y() + sliderPos, handleThickness, len);
				break; 
			}
			case SC_SliderGroove: {
				// Adjust size
				if ( slider->orientation == Qt::Horizontal ) r.setHeight( thickness );
				else r.setWidth( thickness );
				// Adjust place
				r.moveCenter( QPoint( centerX, centerY ) );
				break;
			}
			default:
				break;
			}
			r = visualRect(slider->direction, slider->rect, r);
		}
		break;

	default:
	r = QWindowsStyle::subControlRect( control, o, subControl, widget );
	}

	return r;
}

int FloatStyle::pixelMetric ( PixelMetric metric, const QStyleOption * o, const QWidget * widget ) const
{
	switch( metric ) {
		case PM_ButtonShiftHorizontal:
		case PM_ButtonShiftVertical: return 3;
		case PM_ComboBoxFrameWidth:
		case PM_DefaultFrameWidth: return 4;
		case PM_MenuBarPanelWidth: return 2;
		case PM_MenuPanelWidth: return 1;
		case PM_ProgressBarChunkWidth: return 1;
		// checkboxes and radios
		case PM_ExclusiveIndicatorWidth:
		case PM_ExclusiveIndicatorHeight:
		case PM_IndicatorWidth:
		case PM_IndicatorHeight: return 18;
		case PM_TabBarTabShiftHorizontal:
		case PM_TabBarTabShiftVertical: return 3;
		case PM_ScrollBarSliderMin: return 15;
		case PM_ToolBarItemMargin: return 2;
		case PM_DefaultTopLevelMargin:
		case PM_DockWidgetTitleMargin: return 1;
		case PM_DockWidgetFrameWidth: return 2;
		case PM_MDIFrameWidth: return 2;
// 		case PM_SplitterWidth: return 10;
		// Slider
		case PM_SliderThickness: return 16; // 8 + 2*PM_DefaultFrameWidth
		case PM_SliderControlThickness: return 10;
		case PM_SliderLength: return 23;
		case PM_SliderTickmarkOffset: return 4;
		default: return QWindowsStyle::pixelMetric( metric, o, widget );
	}
		
}
