/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar    						   *
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
#include "eeditor.h"

RichTextEditor::RichTextEditor(QWidget *parent)
 : QTextEdit(parent)
{
     connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SIGNAL(stateChanged()));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SIGNAL(stateChanged()));
}

QToolBar *RichTextEditor::createToolBar( QWidget *parent)
{ return new RichTextEditorToolBar( this, parent); }

void RichTextEditor::setFontBold(bool b)
{
    if (b)
        setFontWeight(QFont::Bold);
    else
        setFontWeight(QFont::Normal);
}

void RichTextEditor::setFontPointSize(double d)
{
    QTextEdit::setFontPointSize(qreal(d));
}

void RichTextEditor::setText(const QString &text)
{
    if (Qt::mightBeRichText(text))
        setHtml(text);
    else
        setPlainText(text);
}

void RichTextEditor::setDefaultFont(const QFont &font)
{
    document()->setDefaultFont(font);
    if (font.pointSize() > 0)
        setFontPointSize(font.pointSize());
    else
        setFontPointSize(QFontInfo(font).pointSize());
    emit textChanged();
}

QString RichTextEditor::text(Qt::TextFormat format) const
{
    switch (format) {
    case Qt::LogText:
    case Qt::PlainText:
        return toPlainText();
    case Qt::RichText:
        return toHtml();
    case Qt::AutoText:
        break;
    }
    const QString html = toHtml();
    const QString plain = toPlainText();
    QTextEdit tester;
    tester.setPlainText(plain);
    return tester.toHtml() == html ? plain : html;
}


RichTextEditor::~RichTextEditor()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Html Text Editor

void HtmlTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    QMenu *htmlMenu = new QMenu(tr("Insert HTML entity"), menu);

    typedef struct {
        const char *text;
        const char *entity;
    } Entry;

    const Entry entries[] = {
        { "&&amp; (&&)", "&amp;" },
        { "&&nbsp;", "&nbsp;" },
        { "&&lt; (<)", "&lt;" },
        { "&&gt; (>)", "&gt;" },
        { "&&copy; (Copyright)", "&copy;" },
        { "&&reg; (Trade Mark)", "&reg;" },
    };

    for (int i = 0; i < 6; ++i) {
        QAction *entityAction = new QAction(QLatin1String(entries[i].text),
                                            htmlMenu);
        entityAction->setData(QLatin1String(entries[i].entity));
        htmlMenu->addAction(entityAction);
    }

    menu->addMenu(htmlMenu);
    connect(htmlMenu, SIGNAL(triggered(QAction*)),
                      SLOT(actionTriggered(QAction*)));
    menu->exec(event->globalPos());
    delete menu;
}

void HtmlTextEdit::actionTriggered(QAction *action)
{
    insertPlainText(action->data().toString());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// QColor Actionaction
ColorAction::ColorAction(QObject *parent):
    QAction(parent)
{
    setText( tr("Color del texto") );
    setColor(Qt::black);
    connect(this, SIGNAL(triggered()), this, SLOT(chooseColor()));
}

void ColorAction::setColor(const QColor &color)
{
    if (color == m_color)
        return;
    m_color = color;
    QPixmap pix(24, 24);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.fillRect(pix.rect(), m_color);
    painter.setPen(m_color.darker());
    painter.drawRect(pix.rect().adjusted(0, 0, -1, -1));
    setIcon(pix);
}

void ColorAction::chooseColor()
{
    const QColor col = QColorDialog::getColor(m_color, 0);
    if (col.isValid() && col != m_color) {
        setColor(col);
        emit colorChanged(m_color);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Barra de acciones
static QAction *createCheckableAction(const QIcon &icon, const QString &text,
                                      QObject *receiver, const char *slot,
                                      QObject *parent = 0)
{
    QAction *result = new QAction(parent);
    result->setIcon(icon);
    result->setText(text);
    result->setCheckable(true);
    result->setChecked(false);
    if (slot)
        QObject::connect(result, SIGNAL(triggered(bool)), receiver, slot);
    return result;
}

RichTextEditorToolBar::RichTextEditorToolBar( RichTextEditor *editor,
                                              QWidget *parent ) :
    QToolBar(parent),
    m_link_action(new QAction(this)),
    m_image_action(new QAction(this)),
    m_color_action(new ColorAction(this)),
    m_font_size_input(new QComboBox),
    m_editor(editor)
{
    // Font size combo box
    m_font_size_input->setEditable(false);
    const QList<int> font_sizes = QFontDatabase::standardSizes();
    foreach (int font_size, font_sizes)
        m_font_size_input->addItem(QString::number(font_size));

    connect(m_font_size_input, SIGNAL(activated(QString)),
            this, SLOT(sizeInputActivated(QString)));
    addWidget(m_font_size_input);

    addSeparator();

    // Bold, italic and underline buttons

    m_bold_action = createCheckableAction(
            QIcon( ":/imagenes/textbold.png" ),
            tr( "Negrita" ), editor, SLOT( setFontBold( bool ) ), this);
    m_bold_action->setShortcut( tr( "CTRL+B" ) );
    addAction( m_bold_action );

    m_italic_action = createCheckableAction(
            QIcon( ":/imagenes/textitalic.png"),
            tr("Italica"), editor, SLOT(setFontItalic(bool)), this);
    m_italic_action->setShortcut(tr("CTRL+I"));
    addAction(m_italic_action);

    m_underline_action = createCheckableAction(
            QIcon( ":/imagenes/textunder.png"),
            tr("Subrayado"), editor, SLOT(setFontUnderline(bool)), this);
    m_underline_action->setShortcut(tr("CTRL+U"));
    addAction(m_underline_action);

    addSeparator();

    // Left, center, right and justified alignment buttons

    QActionGroup *alignment_group = new QActionGroup(this);
    connect(alignment_group, SIGNAL(triggered(QAction*)),
                             SLOT(alignmentActionTriggered(QAction*)));

    m_align_left_action = createCheckableAction(
            QIcon( ":/imagenes/textleft.png"),
            tr("Alineación izquierda"), editor, 0, alignment_group);
    addAction(m_align_left_action);

    m_align_center_action = createCheckableAction(
            QIcon( ":/imagenes/textcenter.png"),
            tr("Centrado"), editor, 0, alignment_group);
    addAction(m_align_center_action);

    m_align_right_action = createCheckableAction(
            QIcon( ":/imagenes/textright.png"),
            tr("Alineación derecha"), editor, 0, alignment_group);
    addAction(m_align_right_action);

    m_align_justify_action = createCheckableAction(
            QIcon( ":/imagenes/textjustify.png"),
            tr("Justificar"), editor, 0, alignment_group);
    addAction(m_align_justify_action);

    addSeparator();

    // Superscript and subscript buttons

    m_valign_sup_action = createCheckableAction(
            QIcon( ":/imagenes/textsuperscript.png"),
            tr("Superindice"),
            this, SLOT(setVAlignSuper(bool)), this);
    addAction(m_valign_sup_action);

    m_valign_sub_action = createCheckableAction(
            QIcon( ":/imagenes/textsubscript.png"),
            tr("Subindice"),
            this, SLOT(setVAlignSub(bool)), this);
    addAction(m_valign_sub_action);

    addSeparator();

    // Insert image button
    m_image_action->setIcon( QIcon( ":/imagenes/insertimage.png") );
    m_image_action->setText( tr("Insertar &Imagen") );
    connect(m_image_action, SIGNAL(triggered()), SLOT(insertImage()));
    addAction(m_image_action);

    addSeparator();

    // Text color button
    connect(m_color_action, SIGNAL(colorChanged(QColor)),
            this, SLOT(colorChanged(QColor)));
    addAction(m_color_action);

    connect(editor, SIGNAL(textChanged()), this, SLOT(updateActions()));
    connect(editor, SIGNAL(stateChanged()), this, SLOT(updateActions()));

    updateActions();
}

void RichTextEditorToolBar::alignmentActionTriggered(QAction *action)
{
    Qt::Alignment new_alignment;

    if (action == m_align_left_action) {
        new_alignment = Qt::AlignLeft;
    } else if (action == m_align_center_action) {
        new_alignment = Qt::AlignCenter;
    } else if (action == m_align_right_action) {
        new_alignment = Qt::AlignRight;
    } else {
        new_alignment = Qt::AlignJustify;
    }

    m_editor->setAlignment(new_alignment);
}

void RichTextEditorToolBar::colorChanged(const QColor &color)
{
    m_editor->setTextColor(color);
    m_editor->setFocus();
}

void RichTextEditorToolBar::sizeInputActivated(const QString &size)
{
    bool ok;
    int i = size.toInt(&ok);
    if (!ok)
        return;

    m_editor->setFontPointSize(i);
    m_editor->setFocus();
}

void RichTextEditorToolBar::setVAlignSuper(bool super)
{
    const QTextCharFormat::VerticalAlignment align = super ?
        QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal;

    QTextCharFormat charFormat = m_editor->currentCharFormat();
    charFormat.setVerticalAlignment(align);
    m_editor->setCurrentCharFormat(charFormat);

    m_valign_sub_action->setChecked(false);
}

void RichTextEditorToolBar::setVAlignSub(bool sub)
{
    const QTextCharFormat::VerticalAlignment align = sub ?
        QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal;

    QTextCharFormat charFormat = m_editor->currentCharFormat();
    charFormat.setVerticalAlignment(align);
    m_editor->setCurrentCharFormat(charFormat);

    m_valign_sup_action->setChecked(false);
}

#include <QFileDialog>
#include <QApplication>

void RichTextEditorToolBar::insertImage()
{
 QString fileName = QFileDialog::getOpenFileName( this,
                                                 tr("Insertar Imagen"),
                                                 QApplication::applicationDirPath(),
                                                 tr( "Archivos de imagen (*.png *.jpg *.bmp)" )
                                                 );
 if (!fileName.isEmpty())
        m_editor->insertHtml(QLatin1String("<img src=\"") + fileName + QLatin1String("\"/>"));
}

void RichTextEditorToolBar::updateActions()
{
    if (m_editor == 0) {
        setEnabled(false);
        return;
    }

    const Qt::Alignment alignment = m_editor->alignment();
    const QTextCursor cursor = m_editor->textCursor();
    const QTextCharFormat charFormat = cursor.charFormat();
    const QFont font = charFormat.font();
    const QTextCharFormat::VerticalAlignment valign =
        charFormat.verticalAlignment();
    const bool superScript = valign == QTextCharFormat::AlignSuperScript;
    const bool subScript = valign == QTextCharFormat::AlignSubScript;

    if (alignment & Qt::AlignLeft) {
        m_align_left_action->setChecked(true);
    } else if (alignment & Qt::AlignRight) {
        m_align_right_action->setChecked(true);
    } else if (alignment & Qt::AlignHCenter) {
        m_align_center_action->setChecked(true);
    } else {
        m_align_justify_action->setChecked(true);
    }

    m_bold_action->setChecked(font.bold());
    m_italic_action->setChecked(font.italic());
    m_underline_action->setChecked(font.underline());
    m_valign_sup_action->setChecked(superScript);
    m_valign_sub_action->setChecked(subScript);

    const int size = font.pointSize();
    const int idx = m_font_size_input->findText(QString::number(size));
    if (idx != -1)
        m_font_size_input->setCurrentIndex(idx);

    m_color_action->setColor(m_editor->textColor());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dialogo
EEditor::EEditor( QWidget *parent)  :
    QWidget(parent),
    m_editor(new RichTextEditor),
    m_state(Clean)
{
    Q_INIT_RESOURCE(utiles);
    // The toolbar needs to be created after the RichTextEditor
    tool_bar = m_editor->createToolBar( this );
    tool_bar->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

    QVBoxLayout *l = new QVBoxLayout( this );
    l->addWidget(tool_bar);
    l->addWidget(m_editor);

    connect( m_editor, SIGNAL( mostrarToolBar( bool ) ), this, SLOT( ocultarBarra( bool ) ) );
    tool_bar->setVisible( false );
}

/*!
    \fn EEditor::setDefaultFont( const QFont &font )
        Setea la fuente predefinida
        @param font Fuente a utilizar
 */
void EEditor::setDefaultFont(const QFont &font )
{  m_editor->setDefaultFont(font); }

/*!
    \fn EEditor::setText( const QString &text )
        Coloca el texto pasado como parametro en el editor
        @param text Texto a editar
 */
void EEditor::setText(const QString &text)
{ m_editor->setText(text); m_state = Clean; }


/*!
    \fn EEditor::contenido( Qt::TextFormat format ) const
        Devuelve el contenido del editor en el formato elegido
        @param format Formato de devolución ( Qt::TextFormat )
        @return Texto del editor
 */
QString EEditor::contenido(Qt::TextFormat format) const
{ return m_editor->text(format);  }



/*!
    \fn RichTextEditor::focusInEvent( QFocusEvent *event )
    Muestra la barra de edición cuando entra en foco
 */
void RichTextEditor::focusInEvent( QFocusEvent *event )
{
 emit mostrarToolBar( true );
 QTextEdit::focusInEvent( event );
}


/*!
    \fn RichTextEditor::focusOutEvent( QFocusEvent *event )
    Oculta la barra de edición cuando se sale de foco
 */
void RichTextEditor::focusOutEvent( QFocusEvent *event )
{
 emit mostrarToolBar( false );
 QTextEdit::focusOutEvent( event );
}


/*!
    \fn EEditor::ocultarBarra( bool estado )
    Muestra o oculta la barra de estado
    @param estado mostrar o no mostrar
 */
void EEditor::ocultarBarra( bool estado )
{
 if( estado == true )
 {
  // Muestro la barra
  tool_bar->setVisible( true );
 }
 else
 {
  // La oculto si el foco fue hacia un hijo que no es el gb
  if( this->parentWidget() != 0 && QApplication::focusWidget() != 0 )
  {
   // Busco si el gb tiene un hijo con el nombre del elemento que tiene foco actual
   if( this->parentWidget()->findChildren<QWidget*>( QApplication::focusWidget()->objectName() ).isEmpty() )
   {
     tool_bar->setVisible( false );
   }
  }
 }
}


/*!
    \fn EEditor::setHtml( QString contenido )
    Setea el contenido del editor con el codigo html pasado como parametro
    @param contenido Contenido a mostrar en codigo html
 */
void EEditor::setHtml( QString contenido )
{ m_editor->setHtml( contenido ); m_state = Clean; }
