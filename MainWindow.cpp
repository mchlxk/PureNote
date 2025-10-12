#include <QtWidgets>
#include <QTextDocument>

#include "Mainwindow.h"
#include "MouseEvent.h"

#include "StyleSheet.h"
#include "ColorScheme.h"
#include "SchemeIcon.h"


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
}

MainWindow::MainWindow()
: textEdit(new QPlainTextEdit)
, statusLabel (new QLabel)
{
    QCoreApplication::instance()->installEventFilter(this);
    setContentsMargins(0, 0, 0, 0);
    SetupTextEdit();
    UpdatePerStyle();

    UpdateOnTopPerState();
    SetupStatusLabel();
    SetupActions();

    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);

    SetupContextMenu();

    setCurrentFile(QString());
}

void MainWindow::SetupActions()
{
    actionSave = new QAction("Save", this);
    actionSave->setShortcut(QKeySequence("Ctrl+S"));
    connect(actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);
    addAction(actionSave);

    actionSaveAs = new QAction("Save As...", this);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::at_actionSaveAs_triggered);
    addAction(actionSaveAs);

    actionUndo = new QAction("Undo", this);
    actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
    connect(actionUndo, &QAction::triggered, this, &MainWindow::at_actionUndo_triggered);
    addAction(actionUndo);

    actionRedo = new QAction("Redo", this);
    actionRedo->setShortcut(QKeySequence("Ctrl+Shift+Y"));
    connect(actionRedo, &QAction::triggered, this, &MainWindow::at_actionRedo_triggered);
    addAction(actionRedo);

    actionToggleOnTop = new QAction("Stay On Top", this);
    actionToggleOnTop->setCheckable(true);
    actionToggleOnTop->setChecked(State::has_tag<State::Tag::OnTop>(m_stateTags));
    connect(actionToggleOnTop, &QAction::triggered, this, &MainWindow::at_actionToggleOnTop_triggered);
    addAction(actionToggleOnTop);

    actionToggleLocked = new QAction("Lock Edits", this);
    actionToggleLocked->setCheckable(true);
    actionToggleLocked->setChecked(State::has_tag<State::Tag::Locked>(m_stateTags));
    connect(actionToggleLocked, &QAction::triggered, this, &MainWindow::at_actionToggleLocked_triggered);
    addAction(actionToggleLocked);

    actionToggleFullscreen = new QAction("Fullscreen", this);
    actionToggleLocked->setCheckable(true);
    actionToggleFullscreen->setShortcut(QKeySequence("F11"));
    connect(actionToggleFullscreen, &QAction::triggered, this, &MainWindow::at_actionToggleFullscreen_triggered);
    addAction(actionToggleFullscreen);

    actionExit = new QAction("Exit", this);
    actionExit->setShortcut(QKeySequence("Ctrl+X"));
    connect(actionExit, &QAction::triggered, this, &MainWindow::at_actionExit_triggered);
    addAction(actionExit);
}


void MainWindow::SetupStatusLabel()
{
    statusLabel->setAlignment(Qt::AlignLeft);
    statusBar()->addPermanentWidget(statusLabel, 100);
}

void MainWindow::SetupTextEdit()
{
    setCentralWidget(textEdit);
    textEdit->document()->setDocumentMargin(10);
    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::at_document_contentsChanged);
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

    if (MouseEvent::is_rmb_release(evt) && action == MouseEvent::ActionE::None)
    {
        emit customContextMenuRequested(static_cast<QMouseEvent*>(evt)->globalPos());
        return true;
    }

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
        m_opacity = std::max(m_opacity - .01f, .1f);
        UpdatePerOpacity();
    }

    if (MouseEvent::is_alt_wheel_up(evt))
    {
        m_opacity = std::min(m_opacity + .01f, 1.f);
        UpdatePerOpacity();
    }

    return false;
}

void MainWindow::at_customContextMenuRequested(const QPoint& pos)
{
    QMenu* menu = new QMenu(this);
    menu->setWindowFlags(menu->windowFlags() | Qt::NoDropShadowWindowHint);

    actionSave->setEnabled(textEdit->document()->isModified() && HasFile());
    connect(actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);
    menu->addAction(actionSave);

    menu->addAction(actionSaveAs);

    menu->addSeparator();

    // defaults -> submenu

    // Text edit actions
    menu->addSeparator();

    actionUndo->setEnabled(!State::has_tag<State::Tag::Locked>(m_stateTags) && textEdit->document()->isUndoAvailable());
    menu->addAction(actionUndo);

    actionRedo->setEnabled(!State::has_tag<State::Tag::Locked>(m_stateTags) && textEdit->document()->isRedoAvailable());
    menu->addAction(actionRedo);



    // save
    // save-as
    // new note
    // (un)lock
    // display on top
    // set transparent
    // next color scheme


    menu->addSeparator();

    QMenu* colorSchemesSubmenu = new QMenu("Select Color Scheme", this);
    colorSchemesSubmenu->setWindowFlags(colorSchemesSubmenu->windowFlags() | Qt::NoDropShadowWindowHint);
    for (const auto& scheme : ColorScheme::schemas)
    {
        const QString name = scheme.first;
        QAction* actionScheme = new QAction(name);
        Property::ColorScheme::set(actionScheme, name);
        connect(actionScheme, &QAction::triggered, this, &MainWindow::at_actionSetColorScheme_triggered);
        if (name == Style::color_scheme(m_style))
            actionScheme->setEnabled(false);
        actionScheme->setIcon(SchemeIcon::get(scheme.second, 24));
        colorSchemesSubmenu->addAction(actionScheme);
    }
    menu->addMenu(colorSchemesSubmenu);


    QMenu* fontSizeSubmenu = new QMenu("Select Font Size", this);
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
    menu->addMenu(fontSizeSubmenu);

    menu->addSeparator();

    actionToggleOnTop->setChecked(State::has_tag<State::Tag::OnTop>(m_stateTags));
    menu->addAction(actionToggleOnTop);

    actionToggleLocked->setChecked(State::has_tag<State::Tag::Locked>(m_stateTags));
    menu->addAction(actionToggleLocked);

    actionToggleFullscreen->setChecked(State::has_tag<State::Tag::Fullscreen>(m_stateTags));
    menu->addAction(actionToggleFullscreen);

    menu->addAction(actionExit);

    menu->exec(pos);
}


void MainWindow::at_actionSave_triggered()
{
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


void MainWindow::at_actionExit_triggered()
{
    emit close();
}


void MainWindow::at_actionToggleOnTop_triggered()
{
    State::toggle_tag<State::Tag::OnTop>(m_stateTags);
    UpdateOnTopPerState();
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
    textEdit->document()->undo();
}

void MainWindow::at_actionRedo_triggered()
{
    if (State::has_tag<State::Tag::Locked>(m_stateTags))
        return;
    textEdit->document()->redo();
}


void MainWindow::at_actionCut_triggered() {}
void MainWindow::at_actionCopy_triggered() {}
void MainWindow::at_actionPaste_triggered() {}
void MainWindow::at_actionSelectAll_triggered() {}


void MainWindow::at_actionSetColorScheme_triggered()
{
    const QString schemeName = Property::ColorScheme::get(sender());
    SetStyle({schemeName, std::get<1>(m_style)});
}

void MainWindow::at_actionSetFontSize_triggered()
{
    const uint32_t size = Property::FontSize::get(sender());
    SetStyle({std::get<0>(m_style), size});
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
    const QString& schema = std::get<0>(m_style);
    const uint32_t fontSize = std::get<1>(m_style);

    if (!ColorScheme::schemas.count(schema))
        return;
    setStyleSheet(StyleSheet::format_global(ColorScheme::schemas.at(schema), fontSize));
    textEdit->setStyleSheet(StyleSheet::format_text_edit(ColorScheme::schemas.at(schema), fontSize));
    statusBar()->setStyleSheet(StyleSheet::format_status_bar(ColorScheme::schemas.at(schema), fontSize));
    statusLabel->setStyleSheet(StyleSheet::format_status_label(ColorScheme::schemas.at(schema), fontSize));
}



void MainWindow::UpdatePerFullscreen()
{
    if (State::has_tag<State::Tag::Fullscreen>(m_stateTags))
    {
        UpdatePerOpacity();
        UpdateOnTopPerState();
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
        UpdateOnTopPerState();
        UpdatePerOpacity();
    }
}


void MainWindow::UpdatePerLocked()
{
	textEdit->setReadOnly(State::has_tag<State::Tag::Locked>(m_stateTags));
}




void MainWindow::newFile()
{
    if (!ResolveUnsavedChanges())
        return;

	textEdit->clear();
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
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
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
    if (textEdit->document()->isModified())
        State::set_tag<State::Tag::Unsaved>(m_stateTags);
    else
        State::clear_tag<State::Tag::Unsaved>(m_stateTags);
    UpdatePerUnsaved();
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

    const auto ret = msgBox.exec();

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
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
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
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    return true;
}



void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
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
    const QString tit = HasTitle()
        ? title
        : (HasFile() ? QFileInfo(curFile).fileName() : "Untitled");

    const QString filePath = HasFile()
        ? curFile
        : "[No file]";

    const QString unsaved = (State::has_tag<State::Tag::Unsaved>(m_stateTags))
        ? "*"
        : "";

    setWindowTitle(tit + unsaved + " [PureNote]");
    //statusBar()->showMessage(filePath + unsavedTag);
    statusLabel->setText(filePath + unsaved);
}


QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::UpdateOnTopPerState()
{
    const bool setOnTop = State::has_tag<State::Tag::OnTop>(m_stateTags)
        || State::has_tag<State::Tag::Fullscreen>(m_stateTags);

    SetupWindowFlags(setOnTop);
}

void MainWindow::UpdatePerOpacity()
{
    if(State::has_tag<State::Tag::Fullscreen>(m_stateTags))
    {
		setWindowOpacity(1);
        return;
    }
	setWindowOpacity(m_opacity);
}

void MainWindow::SetupWindowFlags(bool onTop)
{
    if(onTop)
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    else
        setWindowFlags(Qt::FramelessWindowHint);
    show();
}


