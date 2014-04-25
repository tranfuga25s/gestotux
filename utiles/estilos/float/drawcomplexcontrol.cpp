//
// C++ Implementation: drawcomplexcontrol
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

void FloatStyle::drawComplexControl( ComplexControl control, const QStyleOptionComplex * o, QPainter * p, const QWidget * widget ) const
{
	p->save();
	switch( control ) {
	case CC_ComboBox: { //TODO look what parts must be drawn
		if (const QStyleOptionComboBox *cbOption = qstyleoption_cast<const QStyleOptionComboBox *>(o)) {
			const QColor bgColor = alternateColor( o->palette.button().color() );
			// If is editable content field color is base, else is alternate button
 			const QBrush fieldColor = cbOption->editable? o->palette.base() : o->palette.button();
			if ( cbOption->frame ) {
				p->setBrush( fieldColor );
				renderBox( o, p );
			} else {
				p->fillRect( o->rect, fieldColor );
			}
			
			QRect arrowRect = subControlRect( CC_ComboBox, o, SC_ComboBoxArrow, widget );
			p->fillRect( arrowRect, bgColor );
			// If is On rotate the arrow
			if ( cbOption->state & (State_Sunken | State_On) ) drawPixmapCentered( arrowRect, p, m_arrowD );
			else drawPixmapCentered( arrowRect, p, m_arrowL );
			
		}
		break;
 	}
	case CC_SpinBox:
		if (const QStyleOptionSpinBox *sb = qstyleoption_cast<const QStyleOptionSpinBox *>(o)) {
			QStyleOptionSpinBox copy = *sb;
			PrimitiveElement pe;
			if (sb->frame && (sb->subControls & SC_SpinBoxFrame)) {
				p->setBrush( o->palette.base() );
				renderBox( o, p );
			}
			

			if (sb->subControls & SC_SpinBoxUp) {
				copy.rect = subControlRect(CC_SpinBox, sb, SC_SpinBoxUp, widget);
				p->fillRect( copy.rect, alternateColor( o->palette.button().color() ));
				
				copy.subControls = SC_SpinBoxUp;
				if ( sb->stepEnabled & QAbstractSpinBox::StepUpEnabled ) {

					copy.palette = sb->palette;

					if (sb->activeSubControls == SC_SpinBoxUp && (sb->state & State_Sunken)) {
						copy.state |= State_On;
						copy.state |= State_Sunken;
					} else {
						copy.state |= State_Raised;
						copy.state &= ~State_Sunken;
					}
					pe = (sb->buttonSymbols == QAbstractSpinBox::PlusMinus ? PE_IndicatorSpinPlus
						: PE_IndicatorSpinUp);

					drawPrimitive(pe, &copy, p, widget);
				}
			}
			if (sb->subControls & SC_SpinBoxDown) {
				copy.rect = subControlRect(CC_SpinBox, sb, SC_SpinBoxDown, widget);
				p->fillRect( copy.rect, alternateColor( o->palette.button().color() ));
				
				copy.subControls = SC_SpinBoxDown;
				copy.state = sb->state;
				QPalette pal2 = sb->palette;
				if (!(sb->stepEnabled & QAbstractSpinBox::StepDownEnabled)) break;
				copy.palette = pal2;

				if (sb->activeSubControls == SC_SpinBoxDown && (sb->state & State_Sunken)) {
					copy.state |= State_On;
					copy.state |= State_Sunken;
				} else {
					copy.state |= State_Raised;
					copy.state &= ~State_Sunken;
				}
				pe = (sb->buttonSymbols == QAbstractSpinBox::PlusMinus ? PE_IndicatorSpinMinus
					: PE_IndicatorSpinDown);

				drawPrimitive(pe, &copy, p, widget);
			}
		}
		break;
	case CC_GroupBox:
		if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox*>(o)) {
            // Draw frame
			QRect textRect = subControlRect(CC_GroupBox, o, SC_GroupBoxLabel, widget);
			QRect checkBoxRect = subControlRect(CC_GroupBox, o, SC_GroupBoxCheckBox, widget);
			if (groupBox->subControls & QStyle::SC_GroupBoxFrame) {
				QStyleOptionFrameV2 frame;
				frame.QStyleOption::operator=(*groupBox);
				frame.features = groupBox->features;
				frame.lineWidth = groupBox->lineWidth;
				frame.midLineWidth = groupBox->midLineWidth;
				frame.rect = subControlRect(CC_GroupBox, o, SC_GroupBoxFrame, widget);
				drawPrimitive(PE_FrameGroupBox, &frame, p, widget);
			}

            		// Draw title
			if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
				if (!groupBox->text.isEmpty()) {
					QColor textColor = groupBox->textColor;
					if (textColor.isValid())
						p->setPen(textColor);
					int alignment = int(groupBox->textAlignment);
					if (!styleHint(QStyle::SH_UnderlineShortcut, o, widget))
						alignment |= Qt::TextHideMnemonic;

					drawItemText(p, textRect,  Qt::TextShowMnemonic | Qt::AlignHCenter | alignment,
							groupBox->palette, groupBox->state & State_Enabled, groupBox->text,
							textColor.isValid() ? QPalette::NoRole : QPalette::WindowText);

					if (groupBox->state & State_HasFocus) {
						QStyleOptionFocusRect fropt;
						fropt.QStyleOption::operator=(*groupBox);
						fropt.rect = textRect;
						drawPrimitive(PE_FrameFocusRect, &fropt, p, widget);
					}
				}
			}

            		// Draw checkbox
			if (groupBox->subControls & SC_GroupBoxCheckBox) {
				QStyleOptionButton box;
				box.QStyleOption::operator=(*groupBox);
				box.rect = checkBoxRect;
				drawPrimitive(PE_IndicatorCheckBox, &box, p, widget);
			}
		}
		break;
	case CC_ToolButton:
// 		p->save();
// 		p->setPen( Qt::green );
// 		p->setBrush( Qt::yellow );
// 		p->drawRect( o->rect );
// 		p->restore();
		if (const QStyleOptionToolButton *toolbutton
				  = qstyleoption_cast<const QStyleOptionToolButton *>(o)) {
			QRect button = subControlRect(control, toolbutton, SC_ToolButton, widget);

			State bflags = toolbutton->state;

			if (bflags & State_AutoRaise) {
				if (!(bflags & State_MouseOver)) {
					bflags &= ~State_Raised;
				}
			}
			State mflags = bflags;

			if (toolbutton->activeSubControls & SC_ToolButton)
				bflags |= State_Sunken;
			if (toolbutton->activeSubControls & SC_ToolButtonMenu)
				mflags |= State_Sunken;

			QStyleOption tool(0);
			tool.palette = toolbutton->palette;
			if (toolbutton->subControls & SC_ToolButton) {
				if (bflags & (State_Sunken | State_On | State_Raised)) {
					tool.rect = button;
					tool.state = bflags;
					drawPrimitive(PE_PanelButtonTool, &tool, p, widget);
				}
			}

			if (toolbutton->subControls & SC_ToolButtonMenu) {
				tool.rect = subControlRect(control, toolbutton, SC_ToolButtonMenu, widget);
				tool.state = mflags;
				if (mflags & (State_Sunken | State_On | State_Raised))
					drawPrimitive(PE_IndicatorButtonDropDown, &tool, p, widget);
				tool.rect.translate( -2, 0 );
				drawPrimitive(PE_IndicatorArrowDown, &tool, p, widget);
			}

			if (toolbutton->state & State_HasFocus) {
				QStyleOptionFocusRect fr;
				fr.QStyleOption::operator=(*toolbutton);
				fr.rect.adjust(3, 3, -3, -3);
				if (toolbutton->features & QStyleOptionToolButton::Menu)
					fr.rect.adjust(0, 0, -pixelMetric(QStyle::PM_MenuButtonIndicator,
						toolbutton, widget), 0);
				drawPrimitive(PE_FrameFocusRect, &fr, p, widget);
			}
			QStyleOptionToolButton label = *toolbutton;
			int fw = 0; //pixelMetric(PM_DefaultFrameWidth, o, widget);
			label.rect = button.adjusted(fw, fw, -fw, -fw);
			drawControl(CE_ToolButtonLabel, &label, p, widget);
		}
		break;

 	case CC_TitleBar:
		if (const QStyleOptionTitleBar *tb = qstyleoption_cast<const QStyleOptionTitleBar *>(o)) {
			QRect ir;
			if (o->subControls & SC_TitleBarLabel) {
				p->setPen( outlineColor( m_titleBarColor1 ) );
				p->setBrush( m_titleBarColor1 );
				QRect base = o->rect;
				base.setTop( base.top() +2 );
				p->drawPolygon( dockBaseBorder( base ) );
				renderTiled( base, p, m_dockBase ); 

				ir = subControlRect(CC_TitleBar, tb, SC_TitleBarLabel, widget);
				QRect ir2 = p->boundingRect( ir, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, tb->text );
				ir2.setLeft( o->rect.left() );
				ir2.setRight( ir2.right()+8 );
				if ( ir2.bottom() > o->rect.bottom() ) ir2.setBottom( o->rect.bottom() );

				p->setPen( outlineColor( m_titleBarColor2 ) );
				p->setBrush( m_titleBarColor2 );
				p->drawPolygon( dockTitleBorder( ir2 ) );
				renderTiled( ir2, p, m_dockTitle ); 
				
				p->setPen(tb->palette.windowText().color());
				p->drawText(ir.x() + 2, ir.y(), ir.width() - 2, ir.height(),
					    Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, tb->text);
			}

			bool down = false;
			QPixmap pm;

			QStyleOption tool(0);
			tool.palette = tb->palette;
			if (tb->subControls & SC_TitleBarCloseButton) {
				ir = subControlRect(CC_TitleBar, tb, SC_TitleBarCloseButton, widget);
				down = tb->activeSubControls & SC_TitleBarCloseButton && (o->state & State_Sunken);
				if ((tb->titleBarFlags & Qt::WindowType_Mask) == Qt::Tool || qobject_cast<const QDockWidget *>(widget) )
					pm = standardPixmap(SP_DockWidgetCloseButton, &tool, widget);
				else
					pm = standardPixmap(SP_TitleBarCloseButton, &tool, widget);
				tool.rect = ir;
				tool.state = down ? State_Sunken : State_Raised;

				p->save();
				if (down) p->translate(pixelMetric(PM_ButtonShiftHorizontal, tb, widget),pixelMetric(PM_ButtonShiftVertical, tb, widget));
				drawItemPixmap(p, ir, Qt::AlignCenter, pm);
				p->restore();
			}

			if (tb->subControls & SC_TitleBarMaxButton
						 && tb->titleBarFlags & Qt::WindowMaximizeButtonHint) {
				ir = subControlRect(CC_TitleBar, tb, SC_TitleBarMaxButton, widget);

				down = tb->activeSubControls & SC_TitleBarMaxButton && (o->state & State_Sunken);
				pm = standardPixmap(SP_TitleBarMaxButton, &tool, widget);
				tool.rect = ir;
				tool.state = down ? State_Sunken : State_Raised;
// 				drawPrimitive(PE_PanelButtonTool, &tool, p, widget);

				p->save();
				if (down)
					p->translate(pixelMetric(PM_ButtonShiftHorizontal, tb, widget),
						pixelMetric(PM_ButtonShiftVertical, tb, widget));
				drawItemPixmap(p, ir, Qt::AlignCenter, pm);
				p->restore();
			}
		
			if ((tb->subControls & SC_TitleBarNormalButton
						  || tb->subControls & SC_TitleBarMinButton)
						  && tb->titleBarFlags & Qt::WindowMinimizeButtonHint) {
				if (tb->subControls & SC_TitleBarNormalButton)
					ir = subControlRect(CC_TitleBar, tb, SC_TitleBarNormalButton, widget);
				else {
					ir = subControlRect(CC_TitleBar, tb, SC_TitleBarMinButton, widget);
				}
				QStyle::SubControl ctrl = (tb->subControls & SC_TitleBarNormalButton ?
                                           SC_TitleBarNormalButton : SC_TitleBarMinButton);
				QStyle::StandardPixmap spixmap = (tb->subControls & SC_TitleBarNormalButton ?
                                               SP_TitleBarNormalButton : SP_TitleBarMinButton);
				down = tb->activeSubControls & ctrl && (o->state & State_Sunken);
				pm = standardPixmap(spixmap, &tool, widget);
				tool.rect = ir;
				tool.state = down ? State_Sunken : State_Raised;
// 				drawPrimitive(PE_PanelButtonTool, &tool, p, widget);

				p->save();
				if (down)
					p->translate(pixelMetric(PM_ButtonShiftHorizontal, tb, widget),
						pixelMetric(PM_ButtonShiftVertical, tb, widget));
				drawItemPixmap(p, ir, Qt::AlignCenter, pm);
				p->restore();
			}
			if (tb->subControls & SC_TitleBarShadeButton) {
				ir = subControlRect(CC_TitleBar, tb, SC_TitleBarShadeButton, widget);
				down = (tb->activeSubControls & SC_TitleBarShadeButton && (o->state & State_Sunken));
				pm = standardPixmap(SP_TitleBarShadeButton, &tool, widget);
				tool.rect = ir;
				tool.state = down ? State_Sunken : State_Raised;
				p->save();
				if (down) p->translate(pixelMetric(PM_ButtonShiftHorizontal, tb, widget),pixelMetric(PM_ButtonShiftVertical, tb, widget));
				drawItemPixmap(p, ir, Qt::AlignCenter, pm);
				p->restore();
			  }

			if (tb->subControls & SC_TitleBarUnshadeButton) {
				ir = subControlRect(CC_TitleBar, tb, SC_TitleBarUnshadeButton, widget);
				down = tb->activeSubControls & SC_TitleBarUnshadeButton  && (o->state & State_Sunken);
				pm = standardPixmap(SP_TitleBarUnshadeButton, &tool, widget);
				tool.rect = ir;
				tool.state = down ? State_Sunken : State_Raised;
				p->save();
				if (down) p->translate(pixelMetric(PM_ButtonShiftHorizontal, tb, widget), pixelMetric(PM_ButtonShiftVertical, tb, widget));
				drawItemPixmap(p, ir, Qt::AlignCenter, pm);
				p->restore();
			}
			if (tb->subControls & SC_TitleBarContextHelpButton && tb->titleBarFlags & Qt::WindowContextHelpButtonHint) {
				  ir = subControlRect(CC_TitleBar, tb, SC_TitleBarContextHelpButton, widget);

				  down = tb->activeSubControls & SC_TitleBarContextHelpButton  && (o->state & State_Sunken);
				  pm = standardPixmap(SP_TitleBarContextHelpButton, &tool, widget);
				  tool.rect = ir;
				  tool.state = down ? State_Sunken : State_Raised;
				  drawPrimitive(PE_PanelButtonTool, &tool, p, widget);
				  p->save();
				  if (down)
					  p->translate(pixelMetric(PM_ButtonShiftHorizontal, tb, widget),
						  pixelMetric(PM_ButtonShiftVertical, tb, widget));
				  drawItemPixmap(p, ir, Qt::AlignCenter, pm);
				  p->restore();
			}
			if (tb->subControls & SC_TitleBarSysMenu && tb->titleBarFlags & Qt::WindowSystemMenuHint) {
				ir = subControlRect(CC_TitleBar, tb, SC_TitleBarSysMenu, widget);
				if (!tb->icon.isNull()) {
					tb->icon.paint(p, ir);
				} else {
					pm = standardPixmap(SP_TitleBarMenuButton, &tool, widget);
					tool.rect = ir;
					p->save();
					drawItemPixmap(p, ir, Qt::AlignCenter, pm);
					p->restore();
				}
			}
		}
		break;
	case CC_Slider:	
		if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(o)) {
			QRect groove = subControlRect(CC_Slider, o, SC_SliderGroove, widget);
			QRect handle = subControlRect(CC_Slider, o, SC_SliderHandle, widget);
			QRect ticks = subControlRect(CC_Slider, o, SC_SliderTickmarks, widget);
			if ((o->subControls & SC_SliderGroove) && groove.isValid()) {
				renderTiled( groove.adjusted( 0, 2, 0, 0), p, m_box );
				QPen pen;
				if ( slider->state & State_HasFocus )  pen.setColor( o->palette.highlight().color() );
				else  pen.setColor( outlineColor( o->palette.button().color() ) );
				pen.setWidth( 0 );
				pen.setJoinStyle( Qt::MiterJoin );
				p->setPen( pen );
				p->setBrush( Qt::NoBrush );
				p->drawRect( box( groove, true ) );
				p->setBrush( o->palette.highlight() );
				QRect inside = box( groove, true );
				if ( slider->orientation == Qt::Horizontal ) p->drawRect(QRect(inside.left(), inside.top(), handle.left() , inside.height()));
				else p->drawRect(QRect(inside.left(), handle.bottom(), inside.width(), inside.bottom() - handle.bottom() +1));
			}
			
			if ((o->subControls & SC_SliderHandle) ) {
				QPen pen;
				pen.setColor( outlineColor( o->palette.button().color() ) );
				pen.setWidth( 0 );
				pen.setJoinStyle( Qt::MiterJoin );
				p->setPen( pen );
				p->setBrush( o->palette.button() );
				p->drawRect( handle.adjusted( 0, 0, -1, -1 ) );
				// Draw the 3 handle dots
				QStyleOption fake = *o;
				if (slider->orientation == Qt::Horizontal) fake.rect = handle.adjusted( 2, 2, 2, 0 );
				else fake.rect = handle.adjusted( 2, 2, 0, 2 );
				fake.state &= ~State_MouseOver;
				renderSplitter( &fake, p, slider->orientation == Qt::Horizontal );
				// Draw the arrow(s)
				if (slider->tickPosition & QSlider::TicksBelow) {
					p->setPen( pen );
					if (slider->orientation == Qt::Horizontal) {
						QPolygon pol( 3 );
						const int hcenter = handle.center().x();
						const int hbottom = handle.bottom();
						pol.putPoints( 0, 3,
								hcenter -3, hbottom,
								hcenter, groove.bottom(),
								hcenter +3, hbottom);
						p->drawPolygon( pol );
						p->setPen( o->palette.button().color() );
						p->drawLine( hcenter -2, hbottom, hcenter +2, hbottom );
					} else {
						QPolygon pol( 3 );
						const int hcenter = handle.center().y();
						const int hright = handle.right();
						pol.putPoints( 0, 3,
								hright, hcenter -3,
								groove.right(), hcenter,
								hright, hcenter +3 );
						p->drawPolygon( pol );
						p->setPen( o->palette.button().color() );
						p->drawLine( hright, hcenter -2, hright, hcenter +2 );
					}
				}
				if (slider->tickPosition & QSlider::TicksAbove) {
					p->setPen( pen );
					if (slider->orientation == Qt::Horizontal) {
						QPolygon pol( 3 );
						const int hcenter = handle.center().x();
						const int htop = handle.top();
						pol.putPoints( 0, 3,
								hcenter -3, htop,
								hcenter, groove.top(),
								hcenter+3, htop );
						p->drawPolygon( pol );
						p->setPen( o->palette.button().color() );
						p->drawLine( hcenter -2, htop, hcenter +2, htop );
					} else {
						QPolygon pol( 3 );
						const int hcenter = handle.center().y();
						const int hleft = handle.left();
						pol.putPoints( 0, 3,
								hleft, hcenter -3,
								groove.left(), hcenter,
		    						hleft, hcenter +3 );
						p->drawPolygon( pol );
						p->setPen( o->palette.button().color() );
						p->drawLine( hleft, hcenter -2, hleft, hcenter +2 );
					}
				}

			}

 			if (o->subControls & SC_SliderTickmarks) {
				const bool ticksAbove = slider->tickPosition & QSlider::TicksAbove;
				const bool ticksBelow = slider->tickPosition & QSlider::TicksBelow;
				QPen oldPen = p->pen();
				p->setPen(outlineColor( o->palette.button().color() ));
				int tickSize = pixelMetric(PM_SliderTickmarkOffset, o, widget);
				int available = pixelMetric(PM_SliderSpaceAvailable, slider, widget);
				int interval = slider->tickInterval;
				if (interval <= 0) {
					interval = slider->singleStep;
					const int pos1 = QStyle::sliderPositionFromValue(slider->minimum, slider->maximum, interval, available);
					const int pos2 = QStyle::sliderPositionFromValue(slider->minimum, slider->maximum, 0, available);
					if ( pos1 -pos2  < 3) interval = slider->pageStep;
				}
				if (interval <= 0)
					interval = 1;

				int sliderLength = slider->maximum - slider->minimum + 1;
				int nticks = sliderLength / interval; // add one to get the end tickmark
				if (sliderLength % interval > 0)
					nticks++; // round up the number of tick marks

				int v = slider->minimum;
				int len = pixelMetric(PM_SliderLength, slider, widget);
				while (v <= slider->maximum) {
					int pos = sliderPositionFromValue(slider->minimum, slider->maximum,
							v, (slider->orientation == Qt::Horizontal ? slider->rect.width() : slider->rect.height()) - len -6,
							slider->upsideDown) + len / 2 +3;

					int extra = 2 - ((v == slider->minimum || v == slider->maximum) ? 1 : 0);

					if (slider->orientation == Qt::Horizontal) {
						if (ticksAbove) {
							p->drawLine(pos, slider->rect.top() + extra,
									pos, slider->rect.top() + tickSize);
						}
						if (ticksBelow) {
							p->drawLine(pos, slider->rect.bottom() - extra,
									pos, slider->rect.bottom() - tickSize);
						}
					} else {
						if (ticksAbove) {
							p->drawLine(slider->rect.left() + extra, pos,
									slider->rect.left() + tickSize, pos);
						}
						if (ticksBelow) {
							p->drawLine(slider->rect.right() - extra, pos,
									slider->rect.right() - tickSize, pos);
						}
					}

					v += interval;
				}
				p->setPen(oldPen);
			}
		}
		break;
	case CC_ScrollBar: p->fillRect( o->rect, o->palette.background() );
	default: QWindowsStyle::drawComplexControl( control, o, p, widget );
		
	}
	p->restore();
}
