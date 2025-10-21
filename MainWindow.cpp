#include <QtWidgets>
#include <QTextDocument>
#include <QHboxLayout>

#include "Mainwindow.h"
#include "MouseEvent.h"

#include "StyleSheet.h"
#include "ColorScheme.h"
#include "SchemeIcon.h"

#include "PunParser.h"
#include "PunSerializer.h"
#include "Window.h"

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

MainWindow::MainWindow()
: m_textEdit(new QPlainTextEdit)
, m_statusLabel (new QLabel)
, m_topBar(new TopBar(this, 24))
{
    QCoreApplication::instance()->installEventFilter(this);
    setContentsMargins(0, 0, 0, 0);
    SetupTextEdit();
    UpdatePerStyle();

    UpdatePerOnTopState();
    SetupStatusBar();
    SetupActions();

    setUnifiedTitleAndToolBarOnMac(true);

    SetupContextMenu();

    SetSave(Save::no_save);

    m_opacityAdjustTimer.setSingleShot(true);
    m_opacityAdjustTimer.setInterval(1000);
    connect(&m_opacityAdjustTimer, &QTimer::timeout, this, &MainWindow::at_opacityAdjustTimer_expired);

    m_delayedUnsavedUpdateTimer.setSingleShot(true);
    m_delayedUnsavedUpdateTimer.setInterval(1000);
    connect(&m_delayedUnsavedUpdateTimer, &QTimer::timeout, this, &MainWindow::at_delayedUnsavedUpdateTimer_expired);

    m_topBar->Hide();
    connect(m_topBar, &TopBar::close, this, &MainWindow::at_topBar_close);
    connect(m_topBar, &TopBar::minimize, this, &MainWindow::at_topBar_minimize);
    connect(m_topBar, &TopBar::top_lock, this, &MainWindow::at_topBar_topLock);
    setMinimumWidth(m_topBar->GetMinimumWidth());
    setMinimumHeight(1.5*m_topBar->GetMinimumWidth());
}

void MainWindow::SetupActions()
{
    m_actionSave = new QAction("Save", this);
    m_actionSave->setShortcut(QKeySequence("Ctrl+S"));
    connect(m_actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);
    apply_qtbug_74655_workaround(m_actionSave);
    addAction(m_actionSave);

    m_actionSaveAs = new QAction("Save As...", this);
    m_actionSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(m_actionSaveAs, &QAction::triggered, this, &MainWindow::at_actionSaveAs_triggered);
    apply_qtbug_74655_workaround(m_actionSaveAs);
    addAction(m_actionSaveAs);

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
    m_actionToggleLocked->setChecked(!IsLocked());
    connect(m_actionToggleLocked, &QAction::triggered, this, &MainWindow::at_actionToggleLocked_triggered);
    addAction(m_actionToggleLocked);

    m_actionToggleFullscreen = new QAction("Fullscreen", this);
    m_actionToggleFullscreen->setCheckable(true);
    m_actionToggleFullscreen->setShortcut(QKeySequence("F11"));
    connect(m_actionToggleFullscreen, &QAction::triggered, this, &MainWindow::at_actionToggleFullscreen_triggered);
    addAction(m_actionToggleFullscreen);

    m_actionToggleOpaqueWhenActive = new QAction("Opaque When Active", this);
    m_actionToggleOpaqueWhenActive->setCheckable(true);
    connect(m_actionToggleOpaqueWhenActive, &QAction::triggered, this, &MainWindow::at_actionToggleOpaqueWhenActive_triggered);
    addAction(m_actionToggleOpaqueWhenActive);

    m_actionExit = new QAction("Exit", this);
    m_actionExit->setShortcut(QKeySequence("Alt+X"));
    connect(m_actionExit, &QAction::triggered, this, &MainWindow::at_actionExit_triggered);
    apply_qtbug_74655_workaround(m_actionExit);
    addAction(m_actionExit);

    for (const auto& scheme : ColorScheme::schemas)
    {
        const QString name = scheme.first;
        QAction* action = new QAction(name);
        Property::ColorScheme::set(action, name);
        connect(action, &QAction::triggered, this, &MainWindow::at_actionSetColorScheme_triggered);
        action->setIcon(SchemeIcon::get_menu_icon(scheme.second, 24));
        m_colorSchemeActions.append(action);
    }

    for (const uint32_t size : Style::font_sizes)
    {
        QAction* action = new QAction(QString::number(size));
        Property::FontSize::set(action, size);
        connect(action, &QAction::triggered, this, &MainWindow::at_actionSetFontSize_triggered);
        m_fontSizeActions.append(action);
    }

    for (const auto& font : Style::font_families)
    {
        const QString name = font.first;
        QAction* action = new QAction(name);
        Property::FontFamily::set(action, name);
        connect(action, &QAction::triggered, this, &MainWindow::at_actionSetFont_triggered);
        m_fontFamilyActions.append(action);
    }

	QAction* actionOpauqe = new QAction("Opaque");
	Property::Opacity::set(actionOpauqe, 1.f);
	connect(actionOpauqe, &QAction::triggered, this, &MainWindow::at_actionSetOpacity_triggered);
	m_opacityActions.append(actionOpauqe);
    for (const float opacity : std::vector<float>({ .9f, .8f, .7f, .6f, .5f, .4f, .3f, .15f}))
    {
        QAction* action = new QAction(QString::number(opacity));
        Property::Opacity::set(action, opacity);
        connect(action, &QAction::triggered, this, &MainWindow::at_actionSetOpacity_triggered);
        m_opacityActions.append(action);
    }
}


void MainWindow::SetupStatusBar()
{
    m_statusLabel->setAlignment(Qt::AlignLeft);
    statusBar()->addPermanentWidget(m_statusLabel, 90);
    statusBar()->setSizeGripEnabled(true);
}

void MainWindow::SetupTextEdit()
{
    setCentralWidget(m_textEdit);
    m_textEdit->document()->setDocumentMargin(10);
    connect(m_textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::at_document_contentsChanged);
    connect(m_textEdit, &QPlainTextEdit::customContextMenuRequested, this, &MainWindow::at_textEdit_customContextMenuRequested);
    m_textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::SetupContextMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::at_customContextMenuRequested);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ResolveUnsavedChanges()) 
    {
        WriteSettings();
        event->accept();
    }
    else 
    {
        event->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    ScheduleUpdatePerUnsaved();
    m_topBar->UpdatePerParentGeometry();
}

void MainWindow::moveEvent(QMoveEvent* event)
{
    ScheduleUpdatePerUnsaved();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* evt)
{
    static QPoint startPos;
    static QSize startSize;
    static QPoint mouseStartPos;
    static MouseEvent::ActionE action{ MouseEvent::ActionE::None };

    // Exit early on plain mouse-move
    if (evt->type() == QEvent::MouseMove && action == MouseEvent::ActionE::None)
    {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (evt);
        if(m_topBar->Contains(mouseEvent->pos()))
            m_topBar->Show();
        else
            m_topBar->Hide();
        return false;
    }

    // let QT emit context-menu-requested
    if (MouseEvent::is_rmb_release(evt) && action == MouseEvent::ActionE::None)
        return false;

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
        return false;
    }

    if (MouseEvent::is_alt_lmb_press(evt))
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        mouseStartPos = mouseEvent->globalPos();
        startPos = pos();
        action = MouseEvent::ActionE::MoveLmb;
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

    if (MouseEvent::is_lmb_press(evt))
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
        QRect grabArea(statusBar()->frameGeometry());
        // leave space for size-grip
        grabArea.setWidth(grabArea.width() - grabArea.height());
        if (grabArea.contains(mouseEvent->pos()))
        {
			mouseStartPos = mouseEvent->globalPos();
			startPos = pos();
			action = MouseEvent::ActionE::MoveLmb;
			return true;
        }
    }

    if (evt->type() == QEvent::MouseMove)
    {
        switch (action)
        {
            case MouseEvent::ActionE::None:
                break;

            case MouseEvent::ActionE::MoveMmb:
            case MouseEvent::ActionE::MoveLmb:
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

    if (MouseEvent::is_lmb_release(evt) && action == MouseEvent::ActionE::MoveLmb)
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



void MainWindow::at_textEdit_customContextMenuRequested(const QPoint& pos)
{
    ShowContextMenu(m_textEdit->mapToGlobal(pos));
}


void MainWindow::at_customContextMenuRequested(const QPoint& pos)
{
    ShowContextMenu(pos);
}


void MainWindow::ShowContextMenu(const QPoint& pos)
{
    QMenu* menu = new QMenu(this);
    menu->setWindowFlags(menu->windowFlags() | Qt::NoDropShadowWindowHint);

    m_actionSave->setEnabled(CanSave());
    menu->addAction(m_actionSave);

    menu->addAction(m_actionSaveAs);

    menu->addSeparator();

    // defaults -> submenu

    menu->addActions(m_textEdit->createStandardContextMenu()->actions());

    menu->addSeparator();

    QMenu* colorSchemesSubmenu = new QMenu("Color Scheme\t(F5)", this);
    colorSchemesSubmenu->setWindowFlags(colorSchemesSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    colorSchemesSubmenu->addActions(m_colorSchemeActions);
    apply_qtbug_74655_workaround(colorSchemesSubmenu);
    menu->addMenu(colorSchemesSubmenu);

    QMenu* fontSubmenu = new QMenu("Font\t(F4)", this);
    fontSubmenu->setWindowFlags(fontSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    fontSubmenu->addActions(m_fontFamilyActions);
    apply_qtbug_74655_workaround(fontSubmenu);
    menu->addMenu(fontSubmenu);

    QMenu* fontSizeSubmenu = new QMenu("Font Size\t(Ctrl+Wheel)", this);
    fontSizeSubmenu->setWindowFlags(fontSizeSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    fontSizeSubmenu->addActions(m_fontSizeActions);
    apply_qtbug_74655_workaround(fontSizeSubmenu);
    menu->addMenu(fontSizeSubmenu);

    QMenu* opacitySubmenu = new QMenu("Opacity\t(Alt+Wheel)", this);
    opacitySubmenu->setWindowFlags(opacitySubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    opacitySubmenu->addActions(m_opacityActions);
    apply_qtbug_74655_workaround(opacitySubmenu);
    menu->addMenu(opacitySubmenu);

    m_actionToggleOpaqueWhenActive->setChecked(State::has_tag<State::Tag::OpaqueWhenActive>(m_stateTags));
    menu->addAction(m_actionToggleOpaqueWhenActive);

    menu->addSeparator();

    m_actionToggleOnTop->setChecked(State::has_tag<State::Tag::OnTop>(m_stateTags));
    menu->addAction(m_actionToggleOnTop);

    m_actionToggleLocked->setChecked(IsLocked());
    menu->addAction(m_actionToggleLocked);

    m_actionToggleFullscreen->setChecked(State::has_tag<State::Tag::Fullscreen>(m_stateTags));
    menu->addAction(m_actionToggleFullscreen);

    menu->addSeparator();

    menu->addAction(m_actionExit);

    menu->exec(pos);
}

bool MainWindow::Save()
{
    const QString filePath = HasFile()
        ? Save::file_path(m_save)
        : GetBrowseFilename();
    if (filePath.isEmpty())
        return false;
    const auto savedPun = Save(filePath);
    if(!savedPun)
        return false;
	SetSave(save_t(filePath, savedPun));
    return true;
}

bool MainWindow::SaveAs()
{
    const QString filePath = GetBrowseFilename();
    if (filePath.isEmpty())
        return false;
    const auto savedPun = Save(filePath);
    if(!savedPun)
        return false;
	SetSave(save_t(filePath, savedPun));
	return true;
}

void MainWindow::at_actionSave_triggered()
{
    Save();
}

void MainWindow::at_actionSaveAs_triggered()
{
    SaveAs();
}

void MainWindow::at_actionToggleFullscreen_triggered()
{
    State::toggle_tag<State::Tag::Fullscreen>(m_stateTags);
    UpdatePerFullscreen();
    UpdatePerUnsaved();
}

void MainWindow::at_actionToggleOpaqueWhenActive_triggered()
{
    State::toggle_tag<State::Tag::OpaqueWhenActive>(m_stateTags);
    UpdatePerOpacity();
    UpdatePerUnsaved();
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

void MainWindow::at_delayedUnsavedUpdateTimer_expired()
{
    UpdatePerUnsaved();
}

void MainWindow::at_actionToggleOnTop_triggered()
{
    ToggleOnTop();
}

void MainWindow::ToggleOnTop()
{
    State::toggle_tag<State::Tag::OnTop>(m_stateTags);
    UpdatePerOnTopState();
    UpdatePerUnsaved();
}

void MainWindow::at_actionToggleLocked_triggered()
{
    m_textEdit->setReadOnly(!IsLocked());
    UpdatePerUnsaved();
}

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
    style_t newStyle{m_style};
    if (found == Style::font_families.end())
        Style::font_family(newStyle) = Style::font_families.begin()->first;
    else if (++found == Style::font_families.end())
        Style::font_family(newStyle) = Style::font_families.begin()->first;
    else
        Style::font_family(newStyle) = found->first;
    SetStyle(newStyle);
}

void MainWindow::at_actionSetFont_triggered()
{
    const QString name = Property::FontFamily::get(sender());
    if(!Style::font_families.count(name))
        return;
    style_t newStyle{m_style};
    Style::font_family(newStyle) = name;
    SetStyle(newStyle);
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
    SetOpacity(Property::Opacity::get(sender()));
}

void MainWindow::at_topBar_close()
{
    emit close();
}

void MainWindow::at_topBar_minimize()
{
    setWindowState(Qt::WindowState::WindowMinimized);
}

void MainWindow::at_topBar_topLock()
{
    ToggleOnTop();
}


void MainWindow::SetStyle(const style_t& style)
{
    if (m_style == style)
        return;
    m_style = style;
    UpdatePerStyle();
    UpdatePerUnsaved();
}

bool MainWindow::HasUnsavedMeta() const
{
    if (!HasFile() && m_textEdit->document()->isEmpty())
        return false;
    if (!Save::pun(m_save))
        return true;
    if (Content::locked(Pun::content(*Save::pun(m_save))) != IsLocked())
        return true;
    if (!Window::equal(Pun::window(*Save::pun(m_save)), GetWindow()))
        return true;
    if (Pun::style(*Save::pun(m_save)) != GetStyle())
        return true;
    return false;
}

bool MainWindow::HasUnsavedText() const
{
    return m_textEdit->document()->isModified();
}

bool MainWindow::CanSave() const
{
    return HasFile() && (HasUnsavedMeta() || HasUnsavedText());
}


void MainWindow::SetOpacity(float opacity)
{
    if (Window::equal_opacity(m_opacity, Window::clamp_opacity(opacity)))
        return;
    m_opacity = Window::clamp_opacity(opacity);
    StartOpacityAdjustPeriod();
    UpdatePerOpacity();
    UpdatePerUnsaved();
}

void MainWindow::StartOpacityAdjustPeriod()
{
    State::set_tag<State::Tag::OpacityAdjust>(m_stateTags);
    m_opacityAdjustTimer.start();
}

void MainWindow::UpdatePerStyle()
{
    if (!ColorScheme::schemas.count(Style::color_scheme(m_style)))
        return;
    setStyleSheet(StyleSheet::format_global(m_style));
    m_textEdit->setStyleSheet(StyleSheet::format_text_edit(m_style));
    statusBar()->setStyleSheet(StyleSheet::format_status_bar(m_style));
    m_statusLabel->setStyleSheet(StyleSheet::format_status_label(m_style));
    setWindowIcon(SchemeIcon::get_window_icon(ColorScheme::schemas.at(Style::color_scheme(m_style)), 32));
    m_topBar->SetColorScheme(Style::color_scheme(m_style));
}


void MainWindow::UpdatePerFullscreen()
{
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
    {
        PushGeometry();
        UpdatePerOpacity();
        SetupWindowFlags(true);
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


void MainWindow::DecreaseFontSize()
{
	const uint32_t sz = Style::font_size(m_style);
	auto found = std::lower_bound(Style::font_sizes.cbegin(), Style::font_sizes.cend(), sz);
	if (found == Style::font_sizes.cbegin())
		return;
    style_t newStyle{ m_style };
	Style::font_size(newStyle) = *(--found);
    SetStyle(newStyle);
}

void MainWindow::IncreaseFontSize()
{
	const uint32_t sz = Style::font_size(m_style);
	const auto found = std::upper_bound(Style::font_sizes.cbegin(), Style::font_sizes.cend(), sz);
	if (found == Style::font_sizes.cend())
		return;
    style_t newStyle{ m_style };
	Style::font_size(newStyle) = *found;
    SetStyle(newStyle);
}


void MainWindow::DecreaseOpacity()
{
    SetOpacity(Window::clamp_opacity(m_opacity - .08f));
}

void MainWindow::IncreaseOpacity()
{
    SetOpacity(Window::clamp_opacity(m_opacity + .08f));
}

QString MainWindow::GetBrowseFilename()
{
    QFileDialog dialog(this, "Choose file to save...", "", "*.pun");
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return "";
    const QString filePath = dialog.selectedFiles().first();
    if(!filePath.endsWith(".pun"))
        return filePath + ".pun";
    return filePath;
}

void MainWindow::at_document_contentsChanged()
{
    UpdatePerUnsaved();
}

content_t MainWindow::GetContent() const
{
    content_t content{ Content::defaults };
    Content::locked(content) = IsLocked();
    Content::text(content) = m_textEdit->toPlainText();
    return content;
}

void MainWindow::SetContent(const content_t& content)
{
    m_textEdit->document()->setPlainText(Content::text(content));
    SetLocked(Content::locked(content));
}

void MainWindow::SetLocked(bool locked) 
{
    m_textEdit->setReadOnly(locked); 
}


window_t MainWindow::GetWindow() const
{
    window_t window{ Window::defaults };
    Window::geometry(window) = GetGeometry();
    Window::opacity(window) = m_opacity;
    Window::opaque_when_active(window) = State::has_tag<State::Tag::OpaqueWhenActive>(m_stateTags);
	Window::on_top(window) = State::has_tag<State::Tag::OnTop>(m_stateTags);
    Window::fullscreen(window) = State::has_tag<State::Tag::Fullscreen>(m_stateTags);
    return window;
}

pun_t MainWindow::GetPun() const
{
    pun_t pun;
    Pun::window(pun) = GetWindow();
    Pun::style(pun) = m_style;
    Pun::content(pun) = GetContent();
    return pun;
}

void MainWindow::SetWindow(const window_t& window)
{
    if (!Window::geometry(window).isEmpty())
    {
        PushGeometry(Window::geometry(window));
        PopGeometry();
    }

    m_opacity = Window::clamp_opacity(Window::opacity(window));

	if (Window::opaque_when_active(window))
        State::set_tag<State::Tag::OpaqueWhenActive>(m_stateTags);

    if (Window::on_top(window))
    {
        State::set_tag<State::Tag::OnTop>(m_stateTags);
        UpdatePerOnTopState();
    }

    UpdatePerOpacity();

    if (Window::fullscreen(window))
    {
        State::set_tag<State::Tag::Fullscreen>(m_stateTags);
		UpdatePerFullscreen();
    }
}

void MainWindow::SetPun(const pun_t& pun, const QString& filePath)
{
    SetStyle(Pun::style(pun));
    SetContent(Pun::content(pun));
    SetWindow(Pun::window(pun));
    SetSave(save_t(filePath, pun));
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


 void MainWindow::ReadSettings()
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

 
 void MainWindow::WriteSettings()
{
 /*
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
*/
}



int MainWindow::ShowMessageBox(QMessageBox& msgBox)
{
    State::set_tag<State::Tag::MsgBox>(m_stateTags);
    UpdatePerOpacity();
    const int ret = msgBox.exec();
    State::clear_tag<State::Tag::MsgBox>(m_stateTags);
    UpdatePerOpacity();
    return ret;
}


bool MainWindow::ResolveUnsavedChanges()
{
    if(!HasUnsavedMeta() && !HasUnsavedText())
        return true;

    QMessageBox msgBox(this);
    msgBox.setText("The document has been modified.\nDo you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);
    const int ret = ShowMessageBox(msgBox);

    switch (ret) 
    {
		case QMessageBox::Save:
			return Save();
		case QMessageBox::Cancel:
			return false;
		case QMessageBox::Discard: // fallthrough
		default:
			break;
    }
    return true;
}

 
 void MainWindow::LoadFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) 
    {
		QMessageBox msgBox(this);
        const QString schemaName = Style::color_scheme(m_style);
        msgBox.setIconPixmap(SchemeIcon::get_warning_icon(ColorScheme::schemas.at(schemaName), 32));
		msgBox.setText(QString("Cannot read file %1\nDetails: %2").arg(QDir::toNativeSeparators(filePath), file.errorString()));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);
        ShowMessageBox(msgBox);
        return;
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    const auto pun = PunParser::parse(file.readAll());

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    if (!pun)
    {
		QMessageBox msgBox(this);
        const QString schemaName = Style::color_scheme(m_style);
        msgBox.setIconPixmap(SchemeIcon::get_warning_icon(ColorScheme::schemas.at(schemaName), 32));
		msgBox.setText("Cannot load input file (parser error)\nDetails: " + pun.get_error());
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);
        ShowMessageBox(msgBox);
        return;
    }

    SetPun(*pun, filePath);
}

 
 
pun_optional_t<pun_t> MainWindow::Save(const QString& filePath)
{
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    const auto savedPun = SaveImpl(filePath);
    QGuiApplication::restoreOverrideCursor();
    if (savedPun)
        return *savedPun;

	QMessageBox msgBox(this);
	const QString schemaName = Style::color_scheme(m_style);
	msgBox.setIconPixmap(SchemeIcon::get_warning_icon(ColorScheme::schemas.at(schemaName), 32));
	msgBox.setText(QString("Error saveing file %1\nDetails: %2").arg(QDir::toNativeSeparators(filePath), savedPun.get_error()));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);
	ShowMessageBox(msgBox);

    return Pun::Optional::none;
}

pun_expected_t<pun_t, QString> MainWindow::SaveImpl(const QString& filePath)
{
    QSaveFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) 
		return QString("Cannot open file %1 for writing\nDetails: %2").arg(QDir::toNativeSeparators(filePath), file.errorString());

	const auto pun = GetPun();
	QByteArray saveData;
	PunSerializer::serialize(pun, &saveData);
	file.write(saveData);

	if (!file.commit()) 
		return QString("Cannot write file %1\nDetails: %2").arg(QDir::toNativeSeparators(filePath), file.errorString());

	return pun;
}


void MainWindow::SetSave(const save_t& save)
{
    m_save = save;
    m_textEdit->document()->setModified(false);
    UpdatePerUnsaved();
    UpdateStatusBar(HasUnsavedMeta(), HasUnsavedText());
}

void MainWindow::ScheduleUpdatePerUnsaved()
{
    m_delayedUnsavedUpdateTimer.start();
}


void MainWindow::UpdatePerUnsaved()
{
    const bool hasUnsavedMeta = HasUnsavedMeta();
    const bool hasUnsavedText = HasUnsavedText();
    if(!hasUnsavedMeta && !hasUnsavedText)
		m_actionSave->setEnabled(false);
    else
		m_actionSave->setEnabled(HasFile());
    UpdateStatusBarPerUnsaved(hasUnsavedMeta, hasUnsavedText);
}

void MainWindow::UpdateStatusBarPerUnsaved(bool hasUnsavedMeta, bool hasUnsavedText)
{
    static bool lastUnsavedMeta{ false };
    static bool lastUnsavedText{ false };

    if ((lastUnsavedMeta == hasUnsavedMeta) && (lastUnsavedText == hasUnsavedText))
        return;

    lastUnsavedMeta = hasUnsavedMeta;
	lastUnsavedText = hasUnsavedText;

    UpdateStatusBar(hasUnsavedMeta, hasUnsavedText);
}

void MainWindow::UpdateStatusBar(bool hasUnsavedMeta, bool hasUnsavedText)
{
    const QString filePath = HasFile()
        ? Save::file_path(m_save)
        : "[No file]";
    const QString unsavedMetaMark = hasUnsavedMeta ? "^" : "";
    const QString unsavedTextMark = hasUnsavedText ? "*" : "";
    const QString decoratedPath = unsavedMetaMark + unsavedTextMark + filePath;
    setWindowTitle(decoratedPath + " | PureNote");
    m_statusLabel->setText(decoratedPath);
    m_statusLabel->setToolTip(decoratedPath);
}

void MainWindow::UpdatePerOnTopState()
{
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
        return;
    SetupWindowFlags(State::has_tag<State::Tag::OnTop>(m_stateTags));
    m_topBar->SetTopLockChecked(State::has_tag<State::Tag::OnTop>(m_stateTags));
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

    if (State::has_tag<State::Tag::OpaqueWhenActive>(m_stateTags)
        && (State::has_tag<State::Tag::HasMouseContext>(m_stateTags) || State::has_tag<State::Tag::HasDialogContext>(m_stateTags)))
    {
		setWindowOpacity(1.f);
        return;
    }

	setWindowOpacity(m_opacity);
}

void MainWindow::SetupWindowFlags(bool onTop)
{
    if(!State::has_tag<State::Tag::Fullscreen>(m_stateTags))
		PushGeometry();
    if(onTop)
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    else
        setWindowFlags(Qt::FramelessWindowHint);
    show();
    if(!State::has_tag<State::Tag::Fullscreen>(m_stateTags))
		PopGeometry();
}


