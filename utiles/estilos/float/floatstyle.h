/****************************************************************************
**
** Copyright (C) 2005 Matias Costa
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef METASTYLE_H
#define METASTYLE_H

#include <QWindowsStyle>

/*!
 * \brief Clase de estilo de ventana personalizado
 */
class FloatStyle : public QWindowsStyle
{
	Q_OBJECT
public:
	FloatStyle();
	virtual ~FloatStyle();

	QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt, const QWidget *widget) const;
// 	void drawItemPixmap ( QPainter * painter, const QRect & rectangle, int alignment, const QPixmap & pixmap ) const;

	void drawPrimitive(PrimitiveElement elem, const QStyleOption *option, QPainter *painter, const QWidget *widget=0) const;
	void drawControl( ControlElement element, const QStyleOption * option, QPainter * painter, const QWidget *widget=0 ) const;
	void drawComplexControl ( ComplexControl control, const QStyleOptionComplex * option, QPainter * painter, const QWidget * widget = 0 ) const;
	QSize sizeFromContents( ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget=0 ) const;
	QRect subElementRect ( SubElement element, const QStyleOption * option, const QWidget * widget = 0 ) const ;
	QRect subControlRect ( ComplexControl control, const QStyleOptionComplex * option, SubControl subControl, const QWidget * widget = 0 ) const;
	int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const;
	int styleHint ( StyleHint hint, const QStyleOption * option = 0, const QWidget * widget = 0, QStyleHintReturn * returnData = 0 ) const;
	void polish( QWidget *widget );
// 	void polish( QPalette &palette );
	QPalette standardPalette() const;
// 	void drawItemPixmap ( QPainter * painter, const QRect & rectangle, int alignment, const QPixmap & pixmap ) const;

// protected slots:
// 	QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,const QWidget *widget = 0) const;

private:
	QColor m_titleBarColor1, m_titleBarColor2;
	void renderButton( const QStyleOption *option, QPainter *painter ) const;
	void renderBox( const QStyleOption *o, QPainter *p, bool frame = false ) const;
	const QPixmap m_button, m_buttonNS, m_toolButton;
	const QPixmap m_check, m_radio, m_box, m_groupbox, m_slider, m_dockBase, m_dockTitle;
	const QPixmap m_tabSelected, m_tabSelectedS, m_tabFrame;
	const QPixmap m_arrowU, m_arrowD, m_arrowL, m_arrowR, m_arrowUNS, m_arrowDNS, m_arrowLNS, m_arrowRNS;
	const QPixmap m_close, m_dockNormalButton, m_titleBarCloseButton, m_titleBarMaxButton, m_titleBarNormalButton, m_titleBarMinButton, m_titleBarShadeButton, m_titleBarUnshadeButton;
};

#endif
