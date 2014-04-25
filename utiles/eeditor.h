/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   *
 *   juiraze@yahoo.com.ar   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef EEDITOR_H
#define EEDITOR_H

#include <QTextEdit>
#include <QList>
#include <QMap>
#include <QPointer>

#include <QAction>
#include <QColorDialog>
#include <QComboBox>
#include <QFontDatabase>
#include <QTextCursor>
#include <QPainter>
#include <QIcon>
#include <QMenu>
#include <QMoveEvent>
#include <QTabWidget>
#include <QTextDocument>
#include <QTextBlock>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

class RichTextEditor : public QTextEdit
{
Q_OBJECT
public:
    RichTextEditor(QWidget *parent = 0);
    ~RichTextEditor();
    void setDefaultFont(const QFont &font);
    QToolBar *createToolBar( QWidget *parent = 0);
    void focusInEvent( QFocusEvent *event );
    void focusOutEvent( QFocusEvent *event );

public slots:
    void setFontBold(bool b);
    void setFontPointSize(double);
    void setText(const QString &text);
    QString text(Qt::TextFormat format) const;

signals:
    void stateChanged();
    void mostrarToolBar( bool estado );

};

////////////////////////////////////////////////////////////////////////////////////////////
// Clase complementaria
class HtmlTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    HtmlTextEdit(QWidget *parent = 0) : QTextEdit(parent) {}
    void contextMenuEvent(QContextMenuEvent *event);
private slots:
    void actionTriggered(QAction *action);
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Clase Complementaria
/*!
 * \brief Clase de selector de color del editor html
 *
 * Clase que maneja el selector de color del editor html embebido
 */
class ColorAction : public QAction
{
    Q_OBJECT
public:
    ColorAction(QObject *parent);
    const QColor& color() const { return m_color; }
    void setColor(const QColor &color);
signals:
    void colorChanged(const QColor &color);
private slots:
    void chooseColor();
private:
    QColor m_color;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Clase de barra
class RichTextEditorToolBar : public QToolBar
{
    Q_OBJECT
public:
    RichTextEditorToolBar( RichTextEditor *editor,
                           QWidget *parent = 0 );

public slots:
    void updateActions();

private slots:
    void alignmentActionTriggered(QAction *action);
    void sizeInputActivated(const QString &size);
    void colorChanged(const QColor &color);
    void setVAlignSuper(bool super);
    void setVAlignSub(bool sub);
    void insertImage();

private:
    QAction *m_bold_action;
    QAction *m_italic_action;
    QAction *m_underline_action;
    QAction *m_valign_sup_action;
    QAction *m_valign_sub_action;
    QAction *m_align_left_action;
    QAction *m_align_center_action;
    QAction *m_align_right_action;
    QAction *m_align_justify_action;
    QAction *m_link_action;
    QAction *m_image_action;
    ColorAction *m_color_action;
    QComboBox *m_font_size_input;

    QPointer<RichTextEditor> m_editor;
};

/**
 * \brief Editor tipo html
 *
 * Editor con capacidades html, del mismo codigo del designer
 *      @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EEditor : public QWidget
{
    Q_OBJECT
public:
    EEditor( QWidget *parent = 0);
    void setDefaultFont(const QFont &font);
    void setText(const QString &text);
    QString contenido(Qt::TextFormat format = Qt::AutoText) const;
    void setHtml( QString contenido );

private:
    enum State { Clean, RichTextChanged };
   /*!
    * Editor interno
    */
    RichTextEditor *m_editor;
   /*!
    * Estado del texto
    */
    State m_state;
    QToolBar *tool_bar;

private slots:
    void ocultarBarra( bool estado );
};

#endif
