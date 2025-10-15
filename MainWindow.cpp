#include <QtWidgets>
#include <QTextDocument>

#include "Mainwindow.h"
#include "MouseEvent.h"

#include "StyleSheet.h"
#include "ColorScheme.h"
#include "SchemeIcon.h"

#include "PunParser.h"
#include "PunSerializer.h"

namespace Property
{
    namespace ColorScheme
    {
        static void set(QObject* obj, const QString& scheme) { obj->setProperty("color_scheme", scheme); }
        static QString get(QObject* obj) { return obj->property("color_scheme").toString(); }
    }

    namespace FontSize
    {
        static void set(QObject* obj, uint32_t size) { obj->setProperty("font_size", size); }
        static uint32_t get(QObject* obj) { return obj->property("font_size").toUInt(); }
    }

    namespace Opacity
    {
        static void set(QObject* obj, float opacity) { obj->setProperty("opacity", opacity); }
        static float get(QObject* obj) { return obj->property("opacity").toFloat(); }
    }

    namespace FontFamily
    {
        static void set(QObject* obj, const QString& family) { obj->setProperty("font_family", family); }
        static QString get(QObject* obj) { return obj->property("font_family").toString(); }
    }
}

// Woraround for QTBUG-74655: chackabls vs. non-checkable menu items not aligned properly
template<typename T>
static inline void apply_qtbug_74655_workaround(T* item)
{
    static const QPixmap pixmap = [] {
        QPixmap p(1, 1);
        p.fill(Qt::transparent);
        return p;
	} ();
    static const QIcon ico(pixmap);
    item->setIcon(ico);
}

static constexpr float min_opacity{ .15f };

MainWindow::MainWindow()
: m_textEdit(new QPlainTextEdit)
, m_statusLabel (new QLabel)
{
    QCoreApplication::instance()->installEventFilter(this);
    setContentsMargins(0, 0, 0, 0);
    SetupTextEdit();
    UpdatePerStyle();

    UpdatePerOnTopState();
    SetupStatusLabel();
    SetupActions();

    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);

    SetupContextMenu();

    setCurrentFile(QString());

    m_opacityAdjustTimer.setSingleShot(true);
    m_opacityAdjustTimer.setInterval(1000);
    connect(&m_opacityAdjustTimer, &QTimer::timeout, this, &MainWindow::at_opacityAdjustTimer_expired);
}

void MainWindow::SetupActions()
{
    m_actionSave = new QAction("Save", this);
    m_actionSave->setShortcut(QKeySequence("Ctrl+S"));
    connect(m_actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);
    addAction(m_actionSave);

    m_actionSaveAs = new QAction("Save As...", this);
    connect(m_actionSaveAs, &QAction::triggered, this, &MainWindow::at_actionSaveAs_triggered);
    addAction(m_actionSaveAs);

    m_actionUndo = new QAction("Undo", this);
    m_actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
    connect(m_actionUndo, &QAction::triggered, this, &MainWindow::at_actionUndo_triggered);
    addAction(m_actionUndo);

    m_actionRedo = new QAction("Redo", this);
    m_actionRedo->setShortcut(QKeySequence("Ctrl+Shift+Y"));
    connect(m_actionRedo, &QAction::triggered, this, &MainWindow::at_actionRedo_triggered);
    addAction(m_actionRedo);

    m_actionNextColorScheme = new QAction("Next ColorScheme", this);
    m_actionNextColorScheme->setShortcut(QKeySequence("F5"));
    connect(m_actionNextColorScheme, &QAction::triggered, this, &MainWindow::at_actionNextColorScheme_triggered);
    addAction(m_actionNextColorScheme);

    m_actionNextFont = new QAction("Next Font", this);
    m_actionNextFont->setShortcut(QKeySequence("F4"));
    connect(m_actionNextFont, &QAction::triggered, this, &MainWindow::at_actionNextFont_triggered);
    addAction(m_actionNextFont);

    m_actionDecreaseFontsize = new QAction("Decrease Font Size", this);
    m_actionDecreaseFontsize->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Minus));
    connect(m_actionDecreaseFontsize, &QAction::triggered, this, &MainWindow::at_actionDecreaseFontSize_triggered);
    addAction(m_actionDecreaseFontsize);

    m_actionIncreaseFontsize = new QAction("Increase Font Size", this);
    m_actionIncreaseFontsize->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Plus));
    connect(m_actionIncreaseFontsize, &QAction::triggered, this, &MainWindow::at_actionIncreaseFontSize_triggered);
    addAction(m_actionIncreaseFontsize);

    m_actionDecreaseOpacity = new QAction("Decrease Opacity", this);
    m_actionDecreaseOpacity->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_Down));
    connect(m_actionDecreaseOpacity, &QAction::triggered, this, &MainWindow::at_actionDecreaseOpacity_triggered);
    addAction(m_actionDecreaseOpacity);

    m_actionIncreaseOpacity = new QAction("Increase Opacity", this);
    m_actionIncreaseOpacity->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_Up));
    connect(m_actionIncreaseOpacity, &QAction::triggered, this, &MainWindow::at_actionIncreaseOpacity_triggered);
    addAction(m_actionIncreaseOpacity);

    m_actionToggleOnTop = new QAction("Stay On Top", this);
    m_actionToggleOnTop->setCheckable(true);
    m_actionToggleOnTop->setChecked(State::has_tag<State::Tag::OnTop>(m_stateTags));
    connect(m_actionToggleOnTop, &QAction::triggered, this, &MainWindow::at_actionToggleOnTop_triggered);
    addAction(m_actionToggleOnTop);

    m_actionToggleLocked = new QAction("Lock Edits", this);
    m_actionToggleLocked->setCheckable(true);
    m_actionToggleLocked->setChecked(State::has_tag<State::Tag::Locked>(m_stateTags));
    connect(m_actionToggleLocked, &QAction::triggered, this, &MainWindow::at_actionToggleLocked_triggered);
    addAction(m_actionToggleLocked);

    m_actionToggleFullscreen = new QAction("Fullscreen", this);
    m_actionToggleFullscreen->setCheckable(true);
    m_actionToggleFullscreen->setShortcut(QKeySequence("F11"));
    connect(m_actionToggleFullscreen, &QAction::triggered, this, &MainWindow::at_actionToggleFullscreen_triggered);
    addAction(m_actionToggleFullscreen);

    m_actionToggleOpaqueOnContext = new QAction("Opaque If Active", this);
    m_actionToggleOpaqueOnContext->setCheckable(true);
    connect(m_actionToggleOpaqueOnContext, &QAction::triggered, this, &MainWindow::at_actionToggleOpaqueOnContext_triggered);
    addAction(m_actionToggleOpaqueOnContext);

    m_actionExit = new QAction("Exit", this);
    m_actionExit->setShortcut(QKeySequence("Alt+X"));
    connect(m_actionExit, &QAction::triggered, this, &MainWindow::at_actionExit_triggered);
    addAction(m_actionExit);
}


void MainWindow::SetupStatusLabel()
{
    m_statusLabel->setAlignment(Qt::AlignLeft);
    statusBar()->addPermanentWidget(m_statusLabel, 100);
}

void MainWindow::SetupTextEdit()
{
    setCentralWidget(m_textEdit);
    m_textEdit->document()->setDocumentMargin(10);
    connect(m_textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::at_document_contentsChanged);
}

void MainWindow::SetupContextMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::at_customContextMenuRequested);
}



/*
void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
    */

    /*
    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box")) ;


    #ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}
*/



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ResolveUnsavedChanges()) 
    {
        writeSettings();
        event->accept();
    }
    else 
    {
        event->ignore();
    }
}


bool MainWindow::eventFilter(QObject* obj, QEvent* evt)
{
    static QPoint startPos;
    static QSize startSize;
    static QPoint mouseStartPos;
    static MouseEvent::ActionE action{ MouseEvent::ActionE::None };

    // Exit early on plain mouse-move
    if (evt->type() == QEvent::MouseMove && action == MouseEvent::ActionE::None)
        return false;

    if (MouseEvent::is_rmb_release(evt) && action == MouseEvent::ActionE::None)
    {
        emit customContextMenuRequested(static_cast<QMouseEvent*>(evt)->globalPos());
        return true;
    }

    if (evt->type() == QEvent::WindowActivate)
    {
        State::set_tag<State::Tag::HasDialogContext>(m_stateTags);
        UpdatePerOpacity();
        return false;
    }

    if (evt->type() == QEvent::WindowDeactivate)
    {
        State::clear_tag<State::Tag::HasDialogContext>(m_stateTags);
        UpdatePerOpacity();
        return false;
    }

    if (evt->type() == QEvent::Enter)
    {
        State::set_tag<State::Tag::HasMouseContext>(m_stateTags);
        UpdatePerOpacity();
        return false;
    }

    if (evt->type() == QEvent::Leave)
    {
        State::clear_tag<State::Tag::HasMouseContext>(m_stateTags);
        UpdatePerOpacity();
        return false;
    }

    if (MouseEvent::is_ctrl_wheel_up(evt))
    {
        IncreaseFontSize();
		return true;
    }

    if (MouseEvent::is_ctrl_wheel_down(evt))
    {
        DecreaseFontSize();
		return true;
    }


    // Do not process any of the following events in fullscreen mode
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
        return false;

    if (MouseEvent::is_mmb_press(evt))
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        mouseStartPos = mouseEvent->globalPos();
        startPos = pos();
        action = MouseEvent::ActionE::MoveMmb;
        return true;
    }

    if (MouseEvent::is_alt_lmb_press(evt))
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        mouseStartPos = mouseEvent->globalPos();
        startPos = pos();
        action = MouseEvent::ActionE::MoveAltLmb;
        return true;
    }

    if (MouseEvent::is_alt_rmb_press(evt))
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        mouseStartPos = mouseEvent->globalPos();
        startSize = size();
        action = MouseEvent::ActionE::ResizeAltRmb;
        return true;
    }

    if (evt->type() == QEvent::MouseMove)
    {
        switch (action)
        {
            case MouseEvent::ActionE::None:
                break;

            case MouseEvent::ActionE::MoveMmb:
            case MouseEvent::ActionE::MoveAltLmb:
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (evt);
                move(startPos + (mouseEvent->globalPos() - mouseStartPos));
                return true;
            }

            case MouseEvent::ActionE::ResizeAltRmb:
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (evt);
                const auto sizeChange = mouseEvent->globalPos() - mouseStartPos;
                resize(startSize.width() + sizeChange.x(), startSize.height() + sizeChange.y());
                return true;
            }
        }
    }

    if (MouseEvent::is_lmb_release(evt) && action == MouseEvent::ActionE::MoveAltLmb)
    {
        action = MouseEvent::ActionE::None;
        return true;
    }

    if (MouseEvent::is_mmb_release(evt) && action == MouseEvent::ActionE::MoveMmb)
    {
        action = MouseEvent::ActionE::None;
        return true;
    }

    if (MouseEvent::is_rmb_release(evt) && action == MouseEvent::ActionE::ResizeAltRmb)
    {
        action = MouseEvent::ActionE::None;
        return true;
    }

    if (MouseEvent::is_alt_wheel_down(evt))
    {
        DecreaseOpacity();
        return true;
    }

    if (MouseEvent::is_alt_wheel_up(evt))
    {
        IncreaseOpacity();
        return true;
    }

    return false;
}

void MainWindow::at_customContextMenuRequested(const QPoint& pos)
{
    QMenu* menu = new QMenu(this);
    menu->setWindowFlags(menu->windowFlags() | Qt::NoDropShadowWindowHint);

    m_actionSave->setEnabled(m_textEdit->document()->isModified() && HasFile());
    connect(m_actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);
    apply_qtbug_74655_workaround(m_actionSave);
    menu->addAction(m_actionSave);

    apply_qtbug_74655_workaround(m_actionSaveAs);
    menu->addAction(m_actionSaveAs);

    menu->addSeparator();

    // defaults -> submenu

    // Text edit m_actions
    menu->addSeparator();

    m_actionUndo->setEnabled(!State::has_tag<State::Tag::Locked>(m_stateTags) && m_textEdit->document()->isUndoAvailable());
    apply_qtbug_74655_workaround(m_actionUndo);
    menu->addAction(m_actionUndo);

    m_actionRedo->setEnabled(!State::has_tag<State::Tag::Locked>(m_stateTags) && m_textEdit->document()->isRedoAvailable());
    apply_qtbug_74655_workaround(m_actionRedo);
    menu->addAction(m_actionRedo);

    menu->addSeparator();

    QMenu* colorSchemesSubmenu = new QMenu("Color Scheme\t(F5)", this);
    colorSchemesSubmenu->setWindowFlags(colorSchemesSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    for (const auto& scheme : ColorScheme::schemas)
    {
        const QString name = scheme.first;
        QAction* actionScheme = new QAction(name);
        Property::ColorScheme::set(actionScheme, name);
        connect(actionScheme, &QAction::triggered, this, &MainWindow::at_actionSetColorScheme_triggered);
        actionScheme->setIcon(SchemeIcon::get(scheme.second, 24));
        colorSchemesSubmenu->addAction(actionScheme);
    }
    apply_qtbug_74655_workaround(colorSchemesSubmenu);
    menu->addMenu(colorSchemesSubmenu);

    QMenu* fontSubmenu = new QMenu("Font\t(F4)", this);
    fontSubmenu->setWindowFlags(fontSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    for (const auto& font : Style::font_families)
    {
        const QString name = font.first;
        QAction* actionFont = new QAction(name);
        Property::FontFamily::set(actionFont, name);
        connect(actionFont, &QAction::triggered, this, &MainWindow::at_actionSetFont_triggered);
        if (name == Style::font_family(m_style))
            actionFont->setEnabled(false);
        fontSubmenu->addAction(actionFont);
    }
    apply_qtbug_74655_workaround(fontSubmenu);
    menu->addMenu(fontSubmenu);

    QMenu* fontSizeSubmenu = new QMenu("Font Size\t(Ctrl+Wheel)", this);
    fontSizeSubmenu->setWindowFlags(fontSizeSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    for (const uint32_t size : Style::font_sizes)
    {
        QAction* actionSize = new QAction(QString::number(size));
        Property::FontSize::set(actionSize, size);
        connect(actionSize, &QAction::triggered, this, &MainWindow::at_actionSetFontSize_triggered);
        if (size == Style::font_size(m_style))
            actionSize->setEnabled(false);
        fontSizeSubmenu->addAction(actionSize);
    }
    apply_qtbug_74655_workaround(fontSizeSubmenu);
    menu->addMenu(fontSizeSubmenu);

    QMenu* opacitySubmenu = new QMenu("Opacity\t(Alt+Wheel)", this);
    opacitySubmenu->setWindowFlags(opacitySubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
	QAction* actionOpauqe = new QAction("Opaque");
	Property::Opacity::set(actionOpauqe, 1.f);
	connect(actionOpauqe, &QAction::triggered, this, &MainWindow::at_actionSetOpacity_triggered);
	opacitySubmenu->addAction(actionOpauqe);
    for (const float opacity : std::vector<float>({ .8f, .6f, .5f, .4f, .3f, .15f}))
    {
        QAction* actionOpacity = new QAction(QString::number(opacity));
        Property::Opacity::set(actionOpacity, opacity);
        connect(actionOpacity, &QAction::triggered, this, &MainWindow::at_actionSetOpacity_triggered);
        opacitySubmenu->addAction(actionOpacity);
    }
    apply_qtbug_74655_workaround(opacitySubmenu);
    menu->addMenu(opacitySubmenu);

    apply_qtbug_74655_workaround(m_actionToggleOpaqueOnContext);
    menu->addAction(m_actionToggleOpaqueOnContext);

    menu->addSeparator();

    m_actionToggleOnTop->setChecked(State::has_tag<State::Tag::OnTop>(m_stateTags));
    menu->addAction(m_actionToggleOnTop);

    m_actionToggleLocked->setChecked(State::has_tag<State::Tag::Locked>(m_stateTags));
    menu->addAction(m_actionToggleLocked);

    m_actionToggleFullscreen->setChecked(State::has_tag<State::Tag::Fullscreen>(m_stateTags));
    menu->addAction(m_actionToggleFullscreen);

    apply_qtbug_74655_workaround(m_actionExit);
    menu->addAction(m_actionExit);

    menu->exec(pos);
}


void MainWindow::at_actionSave_triggered()
{
    save();
// TBD
}

void MainWindow::at_actionSaveAs_triggered()
{
    // TBD
}

void MainWindow::at_actionToggleFullscreen_triggered()
{
    State::toggle_tag<State::Tag::Fullscreen>(m_stateTags);
    UpdatePerFullscreen();
}

void MainWindow::at_actionToggleOpaqueOnContext_triggered()
{
    State::toggle_tag<State::Tag::OpaqueOnContext>(m_stateTags);
    UpdatePerOpacity();
}


void MainWindow::at_actionExit_triggered()
{
    emit close();
}

void MainWindow::at_opacityAdjustTimer_expired()
{
    State::clear_tag<State::Tag::OpacityAdjust>(m_stateTags);
    UpdatePerOpacity();
}

void MainWindow::at_actionToggleOnTop_triggered()
{
    State::toggle_tag<State::Tag::OnTop>(m_stateTags);
    UpdatePerOnTopState();
}

void MainWindow::at_actionToggleLocked_triggered()
{
    State::toggle_tag<State::Tag::Locked>(m_stateTags);
    UpdatePerLocked();
}


void MainWindow::at_actionUndo_triggered()
{
    if(State::has_tag<State::Tag::Locked>(m_stateTags))
        return;
    m_textEdit->document()->undo();
}

void MainWindow::at_actionRedo_triggered()
{
    if (State::has_tag<State::Tag::Locked>(m_stateTags))
        return;
    m_textEdit->document()->redo();
}


void MainWindow::at_actionCut_triggered() {}
void MainWindow::at_actionCopy_triggered() {}
void MainWindow::at_actionPaste_triggered() {}
void MainWindow::at_actionSelectAll_triggered() {}


void MainWindow::at_actionSetColorScheme_triggered()
{
    const QString schemeName = Property::ColorScheme::get(sender());
    style_t newStyle(m_style);
    Style::color_scheme(newStyle) = schemeName;
    SetStyle(newStyle);
}

void MainWindow::at_actionNextColorScheme_triggered()
{
    auto found = ColorScheme::schemas.find(Style::color_scheme(m_style));
    auto next = ++found;
    if (next == ColorScheme::schemas.end())
        next = ColorScheme::schemas.begin();
    style_t newStyle(m_style);
    Style::color_scheme(newStyle) = next->first;
    SetStyle(newStyle);
}

void MainWindow::at_actionNextFont_triggered()
{
    const QString& currentFont = Style::font_family(m_style);
    auto found = Style::font_families.find(currentFont);
    if (found == Style::font_families.end())
        Style::font_family(m_style) = Style::font_families.begin()->first;
    else if (++found == Style::font_families.end())
        Style::font_family(m_style) = Style::font_families.begin()->first;
    else
        Style::font_family(m_style) = found->first;
    UpdatePerStyle();
}

void MainWindow::at_actionSetFont_triggered()
{
    const QString name = Property::FontFamily::get(sender());
    if(!Style::font_families.count(name))
        return;
    Style::font_family(m_style) = name;
    UpdatePerStyle();
}


void MainWindow::at_actionDecreaseFontSize_triggered()
{
    DecreaseFontSize();
}

void MainWindow::at_actionIncreaseFontSize_triggered()
{
    IncreaseFontSize();
}


void MainWindow::at_actionDecreaseOpacity_triggered()
{
    DecreaseOpacity();
}

void MainWindow::at_actionIncreaseOpacity_triggered()
{
    IncreaseOpacity();
}


void MainWindow::at_actionSetFontSize_triggered()
{
    const uint32_t size = Property::FontSize::get(sender());
    style_t newStyle(m_style);
    Style::font_size(newStyle) = size;
    SetStyle(newStyle);
}


void MainWindow::at_actionSetOpacity_triggered()
{
    m_opacity = Property::Opacity::get(sender());
    State::set_tag<State::Tag::OpacityAdjust>(m_stateTags);
    m_opacityAdjustTimer.start();
    UpdatePerOpacity();
}


void MainWindow::SetStyle(const style_t& style)
{
    if (m_style == style)
        return;
    m_style = style;
    UpdatePerStyle();
}

void MainWindow::UpdatePerStyle()
{
    if (!ColorScheme::schemas.count(Style::color_scheme(m_style)))
        return;
    setStyleSheet(StyleSheet::format_global(m_style));
    m_textEdit->setStyleSheet(StyleSheet::format_text_edit(m_style));
    statusBar()->setStyleSheet(StyleSheet::format_status_bar(m_style));
    m_statusLabel->setStyleSheet(StyleSheet::format_status_label(m_style));
}



void MainWindow::UpdatePerFullscreen()
{
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
    {
        PushGeometry();
        UpdatePerOpacity();
        UpdatePerOnTopState();
        QMainWindow::showFullScreen();

        const auto gm = geometry();
        if (gm.width() > gm.height())
        {
            const int hMargin = gm.width() / 5;
            const int vMargin = gm.height() / 20;
			setContentsMargins(hMargin, vMargin, hMargin, vMargin);
        }
        else
        {
            const int hMargin = gm.width() / 20;
            const int vMargin = gm.height() / 5;
			setContentsMargins(hMargin, vMargin, hMargin, vMargin);
        }
    }
    else
    {
        QMainWindow::showNormal();
        setContentsMargins(0, 0, 0, 0);
        UpdatePerOnTopState();
        UpdatePerOpacity();
        if (CanPopGeometry())
            PopGeometry();
    }
}


void MainWindow::UpdatePerLocked()
{
	m_textEdit->setReadOnly(State::has_tag<State::Tag::Locked>(m_stateTags));
}


void MainWindow::DecreaseFontSize()
{
	const uint32_t sz = Style::font_size(m_style);
	auto found = std::lower_bound(Style::font_sizes.cbegin(), Style::font_sizes.cend(), sz);
	if (found == Style::font_sizes.cbegin())
		return;
	Style::font_size(m_style) = *(--found);
	UpdatePerStyle();
}

void MainWindow::IncreaseFontSize()
{
	const uint32_t sz = Style::font_size(m_style);
	const auto found = std::upper_bound(Style::font_sizes.cbegin(), Style::font_sizes.cend(), sz);
	if (found == Style::font_sizes.cend())
		return;
	Style::font_size(m_style) = *found;
	UpdatePerStyle();
}


void MainWindow::DecreaseOpacity()
{
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
        return;
	m_opacity = std::max(m_opacity - .08f, min_opacity);
    State::set_tag<State::Tag::OpacityAdjust>(m_stateTags);
    m_opacityAdjustTimer.start();
	UpdatePerOpacity();
}

void MainWindow::IncreaseOpacity()
{
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
        return;
	m_opacity = std::min(m_opacity + .08f, 1.f);
    State::set_tag<State::Tag::OpacityAdjust>(m_stateTags);
    m_opacityAdjustTimer.start();
	UpdatePerOpacity();
}


void MainWindow::newFile()
{
    if (!ResolveUnsavedChanges())
        return;

	m_textEdit->clear();
	setCurrentFile(QString());
}


void MainWindow::open()
{
    if (!ResolveUnsavedChanges())
        return;

	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
		LoadFile(fileName);
}


 
bool MainWindow::save()
{
	return saveFile(m_filePath);

	/*
    if (m_filePath.isEmpty()) 
    {
        return saveAs();
    }
    else 
    {
        return saveFile(m_filePath);
    }
    */
}



bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::at_document_contentsChanged()
{
    if (m_textEdit->document()->isModified())
        State::set_tag<State::Tag::Unsaved>(m_stateTags);
    else
        State::clear_tag<State::Tag::Unsaved>(m_stateTags);
    UpdatePerUnsaved();
}

pun_t MainWindow::GetPun() const
{
    pun_t pun;
    Pun::geometry(pun) = GetGeometry();
    Pun::style(pun) = m_style;
    Pun::opacity(pun) = m_opacity;
    Pun::opaque_on_context(pun) = State::has_tag<State::Tag::OpaqueOnContext>(m_stateTags);
    Pun::locked(pun) = State::has_tag<State::Tag::Locked>(m_stateTags);
    Pun::on_top(pun) = State::has_tag<State::Tag::OnTop>(m_stateTags);
    Pun::fullscreen(pun) = State::has_tag<State::Tag::Fullscreen>(m_stateTags);
    Pun::content(pun) = m_textEdit->toPlainText();
    return pun;
}

QByteArray MainWindow::GetGeometry() const
{
    if (CanPopGeometry())
        return PeekGeometry();
	return saveGeometry();
}

void MainWindow::PushGeometry(const QByteArray& geom)
{
    m_geometryStack.push(geom);
}

void MainWindow::PushGeometry()
{
    m_geometryStack.push(saveGeometry());
}

void MainWindow::PopGeometry()
{
    restoreGeometry(m_geometryStack.top());
    m_geometryStack.pop();
}

bool MainWindow::CanPopGeometry() const
{
    return m_geometryStack.size();
}

QByteArray MainWindow::PeekGeometry() const
{
    return m_geometryStack.top();
}


 void MainWindow::readSettings()
{
     /*
     QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    */
}

 
 void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}



bool MainWindow::ResolveUnsavedChanges()
{
    if (!State::has_tag<State::Tag::Unsaved>(m_stateTags))
        return true;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("PureNote");
    msgBox.setText("The document has been modified.\nDo you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);

    State::set_tag<State::Tag::MsgBox>(m_stateTags);
    UpdatePerOpacity();
    const auto ret = msgBox.exec();
    State::clear_tag<State::Tag::MsgBox>(m_stateTags);
    UpdatePerOpacity();

    switch (ret) 
    {
		case QMessageBox::Save:
			return save();
		case QMessageBox::Cancel:
			return false;
		default:
			break;
    }
    return true;
}

 
 void MainWindow::LoadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) 
    {
        QMessageBox::warning(this, tr("Application"), tr("Cannot read file %1:\n%2.") .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    //QTextStream in(&file);

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    const QString content = PunParser::parse(file.readAll());

    //in.readAll();
    m_textEdit->setPlainText(content);


#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
}

 
 
bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) 
    {
        QByteArray saveData;
        PunSerializer::serialize(GetPun(), &saveData);
        file.write(saveData);

        if (!file.commit()) 
        {
            errorMessage = tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    }
    else 
    {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) 
    {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    return true;
}



void MainWindow::setCurrentFile(const QString &fileName)
{
    m_filePath = fileName;
    m_textEdit->document()->setModified(false);
    UpdatePerFile();
}

void MainWindow::UpdatePerUnsaved()
{
    static bool lastValue{false};
    if (lastValue == State::has_tag<State::Tag::Unsaved>(m_stateTags))
        return;
    lastValue = State::has_tag<State::Tag::Unsaved>(m_stateTags);
    UpdatePerFile();
}

void MainWindow::UpdatePerFile()
{
    const QString tit = (HasFile() ? QFileInfo(m_filePath).fileName() : "Untitled");

    const QString filePath = HasFile()
        ? m_filePath
        : "[No file]";

    const QString unsaved = (State::has_tag<State::Tag::Unsaved>(m_stateTags))
        ? "*"
        : "";

    setWindowTitle(tit + unsaved + " [PureNote]");
    //statusBar()->showMessage(filePath + unsavedTag);
    m_statusLabel->setText(filePath + unsaved);
    m_statusLabel->setToolTip(filePath + unsaved);
}


QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::UpdatePerOnTopState()
{
    const bool setOnTop = State::has_tag<State::Tag::OnTop>(m_stateTags)
        || State::has_tag<State::Tag::Fullscreen>(m_stateTags);
    SetupWindowFlags(setOnTop);
}

void MainWindow::UpdatePerOpacity()
{
    if(State::has_tag<State::Tag::Fullscreen>(m_stateTags) || State::has_tag<State::Tag::MsgBox>(m_stateTags))
    {
		setWindowOpacity(1.f);
        return;
    }

    if (State::has_tag<State::Tag::OpacityAdjust>(m_stateTags))
    {
		setWindowOpacity(m_opacity);
        return;
    }

    if (State::has_tag<State::Tag::OpaqueOnContext>(m_stateTags)
        && (State::has_tag<State::Tag::HasMouseContext>(m_stateTags) || State::has_tag<State::Tag::HasDialogContext>(m_stateTags)))
    {
		setWindowOpacity(1.f);
        return;
    }

	setWindowOpacity(m_opacity);
}

void MainWindow::SetupWindowFlags(bool onTop)
{
    PushGeometry();
    if(onTop)
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    else
    const QByteArray geom = State::has_tag<State::Tag::Fullscreen>(m_stateTags)
        ? QByteArray()
        : saveGeometry();
        setWindowFlags(Qt::FramelessWindowHint);
    show();
    PopGeometry();
}


