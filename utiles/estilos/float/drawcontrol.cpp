//
// C++ Implementation: drawcontrol
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

void FloatStyle::drawControl( ControlElement element, const QStyleOption *o, QPainter *p, const QWidget *widget) const{
        p->save();
        switch( element ) {
        case CE_CheckBox:
                if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton*>(o)) {
                        bool isRadio = (element == CE_RadioButton);
                        QStyleOptionButton subopt = *btn;
                        subopt.rect = subElementRect( SE_CheckBoxIndicator, btn, widget );
                        drawPrimitive( PE_IndicatorCheckBox, &subopt, p, widget );
                        subopt.rect = subElementRect(isRadio ? SE_RadioButtonContents
                                : SE_CheckBoxContents, btn, widget);
                        drawControl(isRadio ? CE_RadioButtonLabel : CE_CheckBoxLabel, &subopt, p, widget);

                        if (btn->state & State_HasFocus) {
                                QStyleOptionFocusRect fropt;
                                fropt.QStyleOption::operator=(*btn);
                                fropt.rect = subElementRect(isRadio ? SE_RadioButtonFocusRect
                                        : SE_CheckBoxFocusRect, btn, widget);
                                drawPrimitive(PE_FrameFocusRect, &fropt, p, widget);
                        }
                }
                break;
        case CE_ScrollBarSubPage:
        case CE_ScrollBarAddPage:
                if ( o->state & State_Sunken )
                        p->fillRect( o->rect,  o->palette.window().color().dark() );
// 		p->fillRect( o->rect, Qt::green );
                break;
        case CE_ScrollBarSlider:
                if (const QStyleOptionSlider *scrollbar = qstyleoption_cast<const QStyleOptionSlider *>(o)) {
                        // Background and border
                        QRect realSlider = o->rect.adjusted( 2, 2, -3, -3 );
                        p->setBrush( o->palette.button() );
                        if (o->state & State_Sunken ) p->setPen( o->palette.highlight().color() );
                        else p->setPen( outlineColor( o->palette.button().color() ) );
                        p->drawRect( realSlider );
                        // Bevel
                        renderTiled( o->rect, p, m_slider );
                        // Gripper
                        p->setPen( outlineColor( o->palette.button().color() ) );
                        bool horizontal = scrollbar->orientation & Qt::Horizontal;
                        if ( ( horizontal && realSlider.width() > 15 ) || ( !horizontal && realSlider.height() > 15 ) )
                                renderGripper( p, realSlider, horizontal );
                        break;
                }
        case CE_ScrollBarSubLine:
        case CE_ScrollBarAddLine: {
                PrimitiveElement arrow;
                if (o->state & State_Horizontal) {
                        if (element == CE_ScrollBarAddLine)
                                arrow = o->direction == Qt::LeftToRight ? PE_IndicatorArrowRight : PE_IndicatorArrowLeft;
                        else
                                arrow = o->direction == Qt::LeftToRight ? PE_IndicatorArrowLeft : PE_IndicatorArrowRight;
                } else {
                        if (element == CE_ScrollBarAddLine)
                                arrow = PE_IndicatorArrowDown;
                        else
                                arrow = PE_IndicatorArrowUp;
                }
                drawPrimitive(arrow, o, p, widget);
                break; }
        case CE_ProgressBarGroove:
                p->setBrush( o->palette.base() );
                renderBox( o, p );
                break;
        case CE_ProgressBarLabel:
                if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(o)) {
                        QMatrix oldMatrix = p->matrix();
                        QRect rect = pb->rect;
                        bool vertical = false;
                        bool invert = false;
                        bool bottomToTop = false;
                        if (const QStyleOptionProgressBarV2 *pb2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(o)) {
                                vertical = (pb2->orientation == Qt::Vertical);
                                invert = pb2->invertedAppearance;
                                bottomToTop = pb2->bottomToTop;
                        }
                        if (vertical) {
                                QMatrix m;
                                rect = QRect(rect.left(), rect.top(), rect.height(), rect.width()); // flip width and height
                                if (bottomToTop) {
                                        m.translate(rect.height(), 0.0);
                                        m.rotate(90);
                                } else {
                                        m.translate(0.0, rect.width());
                                        m.rotate(-90);
                                }
                                p->setMatrix(m);
                        }
                        int u = rect.width() ;
                        int p_v = pb->progress - pb->minimum;
                        int t_s = qMax(0, pb->maximum - pb->minimum);
                        if (u > 0 && pb->progress >= INT_MAX / u && t_s >= u) {
                        // scale down to something usable.
                                p_v /= u;
                                t_s /= u;
                        }
                        if (pb->textVisible && t_s) {
                                int nu = (u * p_v + t_s/2) / t_s;
                                int x = nu;
                                QRect left(rect.x(), rect.y(), x, rect.height());
                                QRect right(rect.x() + x, rect.y(), rect.width() - x, rect.height());
                                Qt::LayoutDirection dir;
                                dir = vertical ? (bottomToTop ? Qt::LeftToRight : Qt::RightToLeft) : pb->direction;
                                if (invert)
                                        dir = (dir == Qt::LeftToRight) ? Qt::RightToLeft : Qt::LeftToRight;
                                const QRect highlighted = visualRect(dir, rect, left);
                                const QRect background = visualRect(dir, rect, right);

                                // Clear the background
                                QRect text = p->boundingRect( rect, Qt::AlignCenter | Qt::TextSingleLine, pb->text );
                                p->setClipRect( text );
                                p->fillRect( highlighted, o->palette.highlight() );
                                p->fillRect( background, o->palette.base() );

                                // Set bold font
                                QFont font;
                                font.setBold(true);
                                p->setFont(font);

                                p->setPen(o->palette.highlightedText().color());
                                p->setClipRect(highlighted);
                                p->drawText(rect, Qt::AlignCenter | Qt::TextSingleLine, pb->text);

                                if (pb->progress != pb->maximum) {
                                        p->setClipRect(background);
                                        p->setPen(o->palette.text().color());
                                        p->drawText(rect, Qt::AlignCenter | Qt::TextSingleLine, pb->text);
                                }
                        }
                        p->setMatrix(oldMatrix);
                        break;
                }

        case CE_ProgressBarContents:
                if (const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(o)) {

                        QRect rect = pb->rect;
                        bool vertical = false;
                        bool inverted = false;
                        // Get extra style options if version 2
                        const QStyleOptionProgressBarV2 *pb2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(o);
                        if (pb2) {
                                vertical = (pb2->orientation == Qt::Vertical);
                                inverted = pb2->invertedAppearance;
                        }

                        p->setPen( o->palette.mid().color() );
                        renderProgressLines( o->rect, p, vertical );

                        bool reverse = ((!vertical && (pb->direction == Qt::RightToLeft)) || vertical);
                        if (inverted) reverse = !reverse;

                        if (pb->minimum == 0 && pb->maximum == 0) {
                        // TODO draw busy indicator
                        } else if( ( pb->progress <= pb->minimum ) || ( pb->maximum <= pb->minimum ) ) {
                        // draw nothing if no progress and insane values
                                break;
                        } else {
                                QRect progress( pb->rect );
                                if(  pb->progress < pb->maximum ) {
                                // if vertical we modify height and move to the bottom
                                // else just set the width
                                        if ( vertical ) {
                                                progress.setHeight( ( (pb->progress - pb->minimum) * pb->rect.height() )  / ( pb->maximum -pb->minimum +1 ) );
                                                progress.translate( 0, progress.top() + ( o->rect.height() - progress.height() ) -2 );
                                        } else {
                                                progress.setWidth( ( (pb->progress - pb->minimum) * pb->rect.width() )  / ( pb->maximum -pb->minimum +1 ) );
                                        }
                                }
                                p->fillRect( progress, pb->palette.highlight() );
                                p->setPen( o->palette.highlight().color().light() );
                                renderProgressLines( progress, p, vertical );
                        }
                }
                break;
        /*case CE_ToolBoxTabShape: {
                if( const QStyleOptionToolBox *tb = qstyleoption_cast<const QStyleOptionToolBox *>(o) ) {
                   p->save();
                   if( tb->state == QStyle::State_Sunken ) {
                       // Lo dibujo con la barra azul

                   } else {
                       // Sin barra azul
                   }
                   // puntas
                   int x1,x2,y1,y2;

                   QRect r2( o->rect() );
                }
                QWindowsStyle::drawControl( element, o, p, widget );
                break;
        }*/
        case CE_TabBarTabShape:
                if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(o)) {
                        p->save();
                        bool rtlHorTabs = (tab->direction == Qt::RightToLeft
                                        && (tab->shape == QTabBar::RoundedNorth
                                        || tab->shape == QTabBar::RoundedSouth));
                        bool selected = tab->state & State_Selected;
                        bool lastTab = ((!rtlHorTabs && tab->position == QStyleOptionTab::End)
                                        || (rtlHorTabs
                                        && tab->position == QStyleOptionTab::Beginning));
                        bool firstTab = ( (!rtlHorTabs && tab->position == QStyleOptionTab::Beginning) || ( rtlHorTabs && tab->position == QStyleOptionTab::End) || tab->position == QStyleOptionTab::OnlyOneTab );
/*			bool previousSelected =
                                ((!rtlHorTabs
                                && tab->selectedPosition == QStyleOptionTab::PreviousIsSelected)
                                || (rtlHorTabs
                                && tab->selectedPosition == QStyleOptionTab::NextIsSelected));*/
                        bool nextSelected =
                                        ((!rtlHorTabs
                                        && tab->selectedPosition == QStyleOptionTab::NextIsSelected)
                                        || (rtlHorTabs
                                        && tab->selectedPosition
                                                == QStyleOptionTab::PreviousIsSelected));
// 			int tabBarAlignment = styleHint(SH_TabBar_Alignment, tab, widget);
// 			bool leftAligned = (!rtlHorTabs && tabBarAlignment == Qt::AlignLeft)
// 					|| (rtlHorTabs
// 					&& tabBarAlignment == Qt::AlignRight);
                                //
// 			bool rightAligned = (!rtlHorTabs && tabBarAlignment == Qt::AlignRight)
// 					|| (rtlHorTabs
// 					&& tabBarAlignment == Qt::AlignLeft);

/*			QColor light = tab->palette.light().color();
                        QColor midlight = tab->palette.midlight().color();
                        QColor dark = tab->palette.dark().color();
                        QColor shadow = tab->palette.shadow().color();
                        QColor background = tab->palette.background().color();*/
// 			int borderThinkness = pixelMetric(PM_TabBarBaseOverlap, tab, widget);
                        int borderThinkness = 10;
                        if (selected)
                                borderThinkness /= 2;
                        QRect r2(o->rect);
                        int x1, y1, x2, y2;
                        p->setPen( outlineColor( o ) );
                        p->setBrush( o->palette.background() );
// 			p->fillRect( o->rect, Qt::green );
                        bool paint = true;
                        QRect rect( o->rect.adjusted( 0,3, 0,0 ) );
                        bool south = false;
                        switch (tab->shape) {
                                case QTabBar::RoundedNorth:
                                        break;
                                case QTabBar::RoundedSouth: {
                                        // vertical mirror transformation
                                        QMatrix m( 1, 0, 0, -1, 0, rect.height()-1 );
                                        p->setMatrix( m, true );
                                        south = true;
                                        break;
                                }
                                default:
                                        paint = false;
                                        QWindowsStyle::drawControl(element, tab, p, widget);
                                        break;
                        }
                        rect.getCoords( &x1, &y1, &x2, &y2 );

                        if( paint ){
                                QRect grect( rect.adjusted( firstTab? 4:0,3, (lastTab&&selected)?-3:0,-1 ) );
                                QColor light = o->palette.background().color();
                                light.setAlpha( 0 );
                                QColor shadow = alternateColor( light );
                                QLinearGradient linearGrad( grect.left(), grect.top(), grect.left(), grect.bottom() );
                                if ( tab->shape == QTabBar::RoundedNorth ) {
                                        linearGrad.setColorAt(0, light);
                                        linearGrad.setColorAt(1, shadow);
                                } else {
                                        if ( firstTab ) grect.adjust( 1,1, 0,0 );
                                        linearGrad.setColorAt(1, light);
                                        linearGrad.setColorAt(0, shadow);
                                }
                                p->setPen( Qt::NoPen );
                                p->setBrush( QBrush( linearGrad ) );
                                p->drawRect( grect );

                                p->setPen( outlineColor( o ) );
                                p->setBrush( Qt::NoBrush );

                                if (selected) {
                                        if ( !firstTab ) x1 -= 4;

                                        if ( tab->position == QStyleOptionTab::Beginning || tab->position == QStyleOptionTab::Middle ) {
                                                p->drawLine(x2-4,y1+4,  x2,y1+4);
                                        }
                                        if ( tab->position == QStyleOptionTab::End || tab->position == QStyleOptionTab::Middle ) {
                                                p->drawLine(x1,y1+4,  x1+4,y1+4);
                                        }
                                        QRect rrect;
                                        rrect.setCoords( x1,y1, x2,y2-1 );
                                        const QPixmap *pm = &m_tabSelected;
                                        if ( south ) {
                                                rrect.adjust( 2,-3, 0,-4 );
                                                pm = &m_tabSelectedS;
                                        }
                                        renderTiled( rrect, p, *pm );
                                        p->setViewTransformEnabled( true );

                                        QPolygon polygon;
                                        polygon.putPoints(0, 6,
                                                        x1+5,y1+1,  x2-4,y1+1,
                                                        x2-3,y1+2,  x2-3,y2  ,
                                                        x1+3,y2  ,  x1+3,y1+3 );
// 					p->setPen( Qt::red );
                                        p->setBrush( o->palette.background() );
                                        p->drawPolygon( polygon );
                                        p->setPen( o->palette.background().color() );
                                        p-> drawLine( x1+4, y2, x2-4,y2 );
                                        // Selected indicator
                                        QRect ribbon = rect.adjusted( rect.width() -10, 2, -4, 0);
                                        // HACK Don't know why do I have to adjust, the rotation avobe shoul make everything
                                        if ( tab->shape == QTabBar::RoundedSouth ) ribbon.translate( 0, -1 );
                                        p->fillRect( ribbon, o->palette.highlight() );
                                } else {
                                        if ( o->state & State_MouseOver ) {
                                                QRect ribbon = rect.adjusted( rect.width() -7, 4, -1, south?-5:-1 );
                                                QColor fill( o->palette.highlight().color() );
                                                fill.setAlphaF( 0.5 );
                                                p->fillRect( ribbon, fill );

                                        }
                                        if( firstTab ) {
                                                // vertical top
                                                p->drawLine( x1+6,y1+4,  x2,y1+4 );
                                                // horizontal left
                                                p->drawLine( x1+4,y1+6,  x1+4,y2   );
                                                // corner point
                                                p->drawPoint( x1+5, y1+5 );
                                        } else if( lastTab ){
                                                p->drawLine( x1,y1+4,  x2-1,y1+4 );
                                        } else {
                                                p->drawLine( x1,y1+4,  x2,y1+4 );
                                        }
                                        if( !nextSelected ) {
                                                if( lastTab ) p->drawLine( x2,y1+5,  x2,y2 );
                                                else p->drawLine( x2,y1+4,  x2,y2 );
                                        }
                                }
                        }
                        p->restore();
                }
                break;
        case CE_TabBarTabLabel:
                if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(o)) {
                        QStyleOptionTabV2 tabV2(*tab);
                        QRect tr = tabV2.rect;
                        bool verticalTabs = tabV2.shape == QTabBar::RoundedEast
                                        || tabV2.shape == QTabBar::RoundedWest
                                        || tabV2.shape == QTabBar::TriangularEast
                                        || tabV2.shape == QTabBar::TriangularWest;
                        bool selected = tabV2.state & State_Selected;
                        // We add 10px in CT_TabBarTab, so, we remove it here
                        if (verticalTabs) {
                                tr.adjust( 0,5, 0,-5 );
                                p->save();
                                int newX, newY, newRot;
                                if (tabV2.shape == QTabBar::RoundedEast || tabV2.shape == QTabBar::TriangularEast) {
                                        newX = tr.width();
                                        newY = tr.y();
                                        newRot = 90;
                                } else {
                                        newX = 0;
                                        newY = tr.y() + tr.height();
                                        newRot = -90;
                                }
                                tr.setRect(0, 0, tr.height(), tr.width());
                                QMatrix m;
                                m.translate(newX, newY);
                                m.rotate(newRot);
                                p->setMatrix(m, true);
                        } else tr.adjust( 5, 0, -5, 0 );

                        tr.adjust( 0, 2, 0, 2 );
                        int horizontalShift = -pixelMetric( QStyle::PM_TabBarTabShiftHorizontal );
                        int verticalShift = -pixelMetric( QStyle::PM_TabBarTabShiftVertical, tab, widget );
                        if ( tab->shape == QTabBar::RoundedSouth ) {
// 				horizontalShift = -horizontalShift;
                                verticalShift = -verticalShift;
                                tr.translate( 0,-verticalShift -3 );
                        }
                        if (selected) tr.translate( horizontalShift, verticalShift );

                        int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
                        if (!styleHint(SH_UnderlineShortcut, o, widget))
                                alignment |= Qt::TextHideMnemonic;
                        if (!tabV2.icon.isNull()) {
                                QSize iconSize = tabV2.iconSize;
                                if (!iconSize.isValid()) {
                                        int iconExtent = pixelMetric(PM_SmallIconSize);
                                        iconSize = QSize(iconExtent, iconExtent);
                                }
                                QPixmap tabIcon = tabV2.icon.pixmap(iconSize,
                                                (tabV2.state & State_Enabled) ? QIcon::Normal
                                        : QIcon::Disabled);
                                p->drawPixmap(tr.left() + 6, tr.center().y() - tabIcon.height() / 2 +2, tabIcon);
                                tr.setLeft(tr.left() + iconSize.width() + 4);
                        }

                        drawItemText(p, tr, alignment, tab->palette, tab->state & State_Enabled, tab->text, QPalette::WindowText);
                        if (verticalTabs)
                                p->restore();

                        if (tabV2.state & State_HasFocus) {
                                const int OFFSET = 1 + pixelMetric(PM_DefaultFrameWidth);

                                int x1, x2;
                                x1 = tabV2.rect.left();
                                x2 = tabV2.rect.right() - 1;

                                QStyleOptionFocusRect fropt;
                                fropt.QStyleOption::operator=(*tab);
                                fropt.rect.setRect(x1 + 1 + OFFSET, tabV2.rect.y() + OFFSET,
                                                x2 - x1 - 2*OFFSET, tabV2.rect.height() - 2*OFFSET);
                                drawPrimitive(PE_FrameFocusRect, &fropt, p, widget);
                        }
                }
                break;
        case CE_MenuBarItem:
                if (const QStyleOptionMenuItem *mbi = qstyleoption_cast<const QStyleOptionMenuItem *>(o)) {
                        bool active = mbi->state & State_Selected;
                        bool hasFocus = mbi->state & State_HasFocus;
                        bool down = mbi->state & State_Sunken;
                        QStyleOptionMenuItem newMbi = *mbi;
                        if (active || hasFocus) {
                                QBrush b = mbi->palette.brush(QPalette::Button);

                                if (active && hasFocus) {
                                //### This can be done with setOpacity(), but this should be faster
                                        const QColor semiTransparent = alphaMix( o->palette.background().color(), o->palette.highlight().color(), 0.7 );
                                        p->setPen( o->palette.highlight().color() );
                                        p->setBrush( semiTransparent );
                                        p->drawRect( o->rect.adjusted( 0,0, -1,-1 ) );
                                }
                                if (active && down) {
                                        p->fillRect( o->rect, o->palette.highlight() );
                                }
                        }
                        uint alignment = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip
                                        | Qt::TextSingleLine;
                        if (!styleHint(SH_UnderlineShortcut, mbi, widget))
                                alignment |= Qt::TextHideMnemonic;
                        QPixmap pix = mbi->icon.pixmap(pixelMetric(PM_SmallIconSize), (mbi->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled);
                        if (!pix.isNull())
                                drawItemPixmap(p,mbi->rect, alignment, pix);
                        else
                                drawItemText(p, mbi->rect, alignment, mbi->palette, mbi->state & State_Enabled,
                                        mbi->text, active && down? QPalette::HighlightedText:QPalette::ButtonText );
                }
                break;

        case CE_MenuItem:
                if (const QStyleOptionMenuItem *menuitem = qstyleoption_cast<const QStyleOptionMenuItem *>(o)) {
                        int x, y, w, h;
                        menuitem->rect.getRect(&x, &y, &w, &h);
                        int tab = menuitem->tabWidth;
                        bool disabled = !(menuitem->state & State_Enabled);
                        const bool checkable = menuitem->checkType != QStyleOptionMenuItem::NotCheckable;
                        const bool checked = checkable ? menuitem->checked : false;
                        bool selected = menuitem->state & State_Selected;


                        const int checkcol = qMax( menuitem->maxIconWidth, 10 );
                        const QRect vCheckRect = visualRect( o->direction, menuitem->rect,
                                        QRect( x+1, y +(h -checkcol)/2,
                                                        checkcol, checkcol ) );
                        const QColor alternateButtonColor = alternateColor( o->palette.button().color() );

                        if( selected ) {
                                //### This can be done with setOpacity(), but this should be faster
                                const QColor itemBg = alphaMix( o->palette.background().color(), o->palette.highlight().color(), 0.7 );
                                p->setPen( o->palette.highlight().color() );
                                p->setBrush( itemBg );
                                p->drawRect( o->rect.adjusted( 0,0, -1,-1 ) );
                                QRect ribbon = o->rect;
                                ribbon.setLeft( ribbon.right() - 8 );
                                p->fillRect( ribbon, o->palette.highlight() );
                        } else {
                                QRect ribbon = o->rect;
                                ribbon.setLeft( ribbon.right() - 8 );
                                p->fillRect( ribbon, alternateButtonColor );
                        }


                        if (menuitem->menuItemType == QStyleOptionMenuItem::Separator){
                                int yoff = y-1 + h / 2;
                                p->setPen( alternateButtonColor );
                                p->drawLine(x + w/4, yoff, x + w - 4, yoff);
                                break;
                        }

                        // On Windows Style, if we have a checkable item and an icon we
                        // draw the icon recessed to indicate an item is checked. If we
                        // have no icon, we draw a checkmark instead.
                        if (!menuitem->icon.isNull()) {
                                QIcon::Mode mode = disabled ? QIcon::Disabled : QIcon::Normal;
                                if (selected && !disabled)
                                        mode = QIcon::Active;
                                QPixmap pixmap;
                                if (checked) pixmap = menuitem->icon.pixmap(pixelMetric(PM_SmallIconSize), mode, QIcon::On);
                                else pixmap = menuitem->icon.pixmap(pixelMetric(PM_SmallIconSize), mode);
                                int pixw = pixmap.width();
                                int pixh = pixmap.height();
                                if ( checkable )
                                        qDrawShadePanel(p, vCheckRect,  menuitem->palette, checked, 1,
                                                &menuitem->palette.brush(QPalette::Background) );
                                QRect pmr(0, 0, pixw, pixh);
                                pmr.moveCenter(vCheckRect.center());
                                p->setPen(menuitem->palette.text().color());
                                p->drawPixmap(pmr.topLeft(), pixmap);
                        } else if( checkable ) {
                                QRect checkMark( 0, 0, 10, 10 );
                                checkMark.moveCenter( vCheckRect.center() );
                                p->setPen( outlineColor( o ) );
                                p->setBrush( o->palette.base() );
// 				qDebug() << vCheckRect;
                                p->drawRect ( checkMark );
                                if( checked ) {
                                        QStyleOptionMenuItem newMi = *menuitem;
                                        newMi.state = State_None;
                                        if (!disabled) newMi.state |= State_Enabled;
                                        if (selected) newMi.state |= State_On;
                                        newMi.rect = checkMark;
                                        drawPrimitive(PE_IndicatorMenuCheckMark, &newMi, p, widget);
                                }
                        }
                        p->setPen( menuitem->palette.buttonText().color() );

                        QColor discol;
                        if( disabled ) {
                                discol = menuitem->palette.text().color();
                                p->setPen( discol );
                        }

                        int xm = menuItemFrame + checkcol + menuItemHMargin;
                        int xpos = menuitem->rect.x() + xm;
                        QRect textRect(xpos, y + menuItemHMargin, w - xm - menuRightBorder - tab + 1, h - 2 * menuItemVMargin);
                        QRect vTextRect = visualRect(o->direction, menuitem->rect, textRect);
                        QString s = menuitem->text;
                        if (!s.isEmpty()) {                     // draw text
// 				p->save();
                                int t = s.indexOf(QLatin1Char('\t'));
                                int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
                                if (!styleHint(SH_UnderlineShortcut, menuitem, widget))
                                        text_flags |= Qt::TextHideMnemonic;
                                text_flags |= Qt::AlignLeft;
                                if (t >= 0) {
                                        QRect vShortcutRect = visualRect(o->direction, menuitem->rect,
                                                        QRect(textRect.topRight(), QPoint(menuitem->rect.right(), textRect.bottom())));
                                        if (disabled && !selected) {
                                                p->setPen(menuitem->palette.light().color());
                                                p->drawText(vShortcutRect.adjusted(1,1,1,1), text_flags, s.mid(t + 1));
                                                p->setPen(discol);
                                        }
                                        p->drawText(vShortcutRect, text_flags, s.mid(t + 1));
                                        s = s.left(t);
                                }
                                QFont font = menuitem->font;
                                if (menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem)
                                        font.setBold(true);
                                p->setFont(font);
                                if (disabled && !selected) {
                                        p->setPen(menuitem->palette.light().color());
                                        p->drawText(vTextRect.adjusted(1,1,1,1), text_flags, s.left(t));
                                        p->setPen(discol);
                                }
                                p->drawText(vTextRect, text_flags, s.left(t));
// 				p->restore();
                        }
                        if (menuitem->menuItemType == QStyleOptionMenuItem::SubMenu) {// draw sub menu arrow
                                int dim = (h - 2 * menuItemFrame) / 2;
                                PrimitiveElement arrow;
                                arrow = (o->direction == Qt::RightToLeft) ? PE_IndicatorArrowLeft : PE_IndicatorArrowRight;
                                xpos = x + w - dim;
                                QRect  vSubMenuRect = visualRect(o->direction, menuitem->rect, QRect(xpos, y + h / 2 - dim / 2, dim, dim));
                                QStyleOptionMenuItem newMI = *menuitem;
                                newMI.rect = vSubMenuRect;
                                newMI.state = disabled ? State_None : State_Enabled;
                                if (selected)
                                        newMI.palette.setColor(QPalette::ButtonText,
                                                newMI.palette.highlightedText().color());
                                drawPrimitive(arrow, &newMI, p, widget);
                        }

                }
                break;

        case CE_HeaderSection:
                // Differentiate between normal and selected
                p->save();
                if ( (o->state & State_Enabled) && (o->state & State_Sunken) ) {
                        QColor light = o->palette.button().color();
                        QColor shadow = alternateColor( light );
                        QLinearGradient linearGrad( o->rect.left(), o->rect.top(), o->rect.left(), o->rect.bottom() );
                        linearGrad.setColorAt(0, shadow);
                        linearGrad.setColorAt(1, light);
                        p->fillRect( o->rect, QBrush( linearGrad ) );
                }
                if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(o)) {
                        //TODO take care for the cases
                        const bool isOne = header->position == QStyleOptionHeader::OnlyOneSection;
                        const bool horizontal = header->orientation == Qt::Horizontal;
                        bool rightLine = !horizontal || !isOne;

                        // now we guess if the header's is of a table
                        // If it is, paint
                        if ( widget && !rightLine ) {
                                if ( widget->parentWidget() )
                                        rightLine = qobject_cast<const QTableView*>( widget->parentWidget() ) != 0;
                        }

                        p->setPen( outlineColor( o ) );
                        p->drawLine( o->rect.bottomLeft(), o->rect.bottomRight() );
                        if ( rightLine ) p->drawLine( o->rect.topRight(), o->rect.bottomRight() );
                }
                p->restore();
                break;
        case CE_ToolBar:
                // nothing to paint in the toolbar
// 		p->fillRect( o->rect, Qt::white );
                break;

        case CE_ToolButtonLabel:
                if (const QStyleOptionToolButton *toolbutton = qstyleoption_cast<const QStyleOptionToolButton *>(o)) {
                        QRect rect = toolbutton->rect;
                        // Arrow type always overrules and is always shown
                        bool hasArrow = toolbutton->features & QStyleOptionToolButton::Arrow;
                        if ( ( (!hasArrow && toolbutton->icon.isNull() ) && !toolbutton->text.isEmpty() )
                                                   || toolbutton->toolButtonStyle == Qt::ToolButtonTextOnly ) {
                                int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
                                if (!styleHint(SH_UnderlineShortcut, o, widget))
                                        alignment |= Qt::TextHideMnemonic;
                                drawItemText(p, rect, alignment, toolbutton->palette,
                                             o->state & State_Enabled, toolbutton->text,
                                             QPalette::ButtonText);
                        } else {
                                QPixmap pm;
                                QSize pmSize = toolbutton->iconSize;
                                if (!toolbutton->icon.isNull()) {
                                        QIcon::State state = toolbutton->state & State_On ? QIcon::On : QIcon::Off;
                                        QIcon::Mode mode;
                                        if (!(toolbutton->state & State_Enabled))
                                                mode = QIcon::Disabled;
                                        else if ((o->state & State_MouseOver) && (o->state & State_AutoRaise))
                                                mode = QIcon::Active;
                                        else
                                                mode = QIcon::Normal;
                                        pm = toolbutton->icon.pixmap(toolbutton->rect.size().boundedTo(toolbutton->iconSize),
                                                        mode, state);
                                        pmSize = pm.size();
                                }

                                if (toolbutton->toolButtonStyle != Qt::ToolButtonIconOnly) {
                                        p->setFont(toolbutton->font);
                                        QRect pr = rect,
                                        tr = rect;
                                        int alignment = Qt::TextShowMnemonic;
                                        if (!styleHint(SH_UnderlineShortcut, o, widget))
                                                alignment |= Qt::TextHideMnemonic;

                                        if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon) {
                                                pr.setHeight(pmSize.height() + 6);

                                                tr.adjust( 0, pr.bottom(), 0, 0 );
                                                if ( !hasArrow ) {
                                                        drawItemPixmap( p, pr, Qt::AlignCenter, pm );
                                                } else {
// 							drawArrow( this, toolbutton, pr, p, widget );
                                                }
                                                alignment |= Qt::AlignCenter;
                                        } else {
                                                pr.setWidth(pmSize.width() + 8);
                                                tr.adjust(pr.right(), 0, 0, 0);
                                                if (!hasArrow) {
                                                        drawItemPixmap(p, pr, Qt::AlignCenter, pm);
                                                } else {
// 							drawArrow(this, toolbutton, pr, p, widget);
                                                }
                                                alignment |= Qt::AlignLeft | Qt::AlignVCenter;
                                        }
// 					p->fillRect( tr, Qt::green );
                                        drawItemText(p, tr, alignment, toolbutton->palette,
                                                        toolbutton->state & State_Enabled, toolbutton->text,
                                                        QPalette::ButtonText);
                                } else {
                                        if (hasArrow) {
// 						qDebug() << "hasArrow";
                                                drawArrow(this, toolbutton, rect, p, widget);
                                        } else {
                                                drawItemPixmap(p, rect, Qt::AlignCenter, pm);
                                        }
                                }
                        }
                }
                break;
        case CE_DockWidgetTitle: {
// 		p->fillRect( o->rect, Qt::blue );
                // FIXME look why Antialias must be disabled for renderTiled
                QRect dock = o->rect;
                dock.setHeight( 14 );
                dock.moveCenter( o->rect.center() );
// 		p->setRenderHint( QPainter::Antialiasing, true );
                p->setPen( outlineColor( m_titleBarColor1 ) );
                p->setBrush( m_titleBarColor1 );
                p->drawPolygon( dockBaseBorder( dock ) );
// 		p->setRenderHint( QPainter::Antialiasing, false );
                renderTiled( dock, p, m_dockBase );

// 		p->setRenderHint( QPainter::Antialiasing, false );
// 		p->setBrush( Qt::NoBrush );
// 		p->setPen( Qt::red );
// 		p->drawPolygon( dockBasePolygon( dock ) );

                if (const QStyleOptionDockWidget *dockWidget = qstyleoption_cast<const QStyleOptionDockWidget *>(o)) {
                        if ( !dockWidget->title.isNull() ) {
                                // Find text width and title rect
                                QString title = dockWidget->title;
                                int textWidth = o->fontMetrics.width( title );
                                const int margin = 4;
                                QRect titleRect = visualRect(dockWidget->direction, dockWidget->rect, dockWidget->rect.adjusted(margin, 0, -margin * 2 - 30, 0));
                                // Chop and insert ellide into title if text is too wide
                                if ( titleRect.width() < textWidth ) title = contractText( title, dockWidget->fontMetrics, titleRect, &textWidth);

                                // Draw the text
                                QFont font = p->font();
                                font.setPointSize( font.pointSize() - 1 );
                                p->setFont(font);
                                p->setPen(dockWidget->palette.text().color());
                                QRect textBound( p->boundingRect( titleRect, Qt::AlignLeft | Qt::AlignTop,  title ) );
                                textBound.setLeft( dock.left() );
                                textBound.setWidth( textBound.width() +6 );
                                p->setPen( outlineColor( m_titleBarColor2 ) );
// 				p->setPen( Qt::red );
                                p->setBrush( m_titleBarColor2 );
// 				p->setRenderHint( QPainter::Antialiasing, true );
                                p->drawPolygon( dockTitleBorder( textBound ) );
// 				p->setRenderHint( QPainter::Antialiasing, false );
                                renderTiled( textBound, p, m_dockTitle );
// 				p->setBrush( Qt::NoBrush );
// 				p->drawPolygon( dockTitlePolygon( textBound ) );
                                p->setPen( o->palette.foreground().color() );
                                p->drawText(titleRect, title, QTextOption(Qt::AlignLeft | Qt::AlignTop));
                        }
                }
                break;
        }
        case CE_Splitter: {
                renderSplitter( o, p, !(o->state & State_Horizontal)  );
                break;
        }
        default:
                QWindowsStyle::drawControl( element, o, p, widget );
        }
        p->restore();
}
