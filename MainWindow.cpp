#include <QtWidgets>
#include <QTextDocument>

#include "Mainwindow.h"
#include "MouseEvent.h"

#include "StyleSheet.h"
#include "ColorScheme.h"


static void store_color_scheme(QObject* obj, const QString& scheme) { obj->setProperty("color_scheme", scheme); }
static QString extract_color_scheme(QObject* obj) { return obj->property("color_scheme").toString(); }


MainWindow::MainWindow()
: textEdit(new QPlainTextEdit)
{

    QCoreApplication::instance()->installEventFilter(this);

    setContentsMargins(0, 0, 0, 0);

    SetColorScheme("Flamingo");

    setCentralWidget(textEdit);

 
    statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel);

    
    setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::at_customContextMenuRequested);

    SetupWindowFlags(true);

    textEdit->document()->setDocumentMargin(10);

    SetupActions();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::at_document_contentsChanged);

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::SetupActions()
{

    actionSave = new QAction("Save", this);
    actionSave->setShortcut(QKeySequence("Ctrl+S"));
    connect(actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);

    actionSaveAs = new QAction("Save As...", this);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::at_actionSaveAs_triggered);

    actionUndo = new QAction("Undo", this);
    actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
    connect(actionUndo, &QAction::triggered, this, &MainWindow::at_actionUndo_triggered);

    actionRedo = new QAction("Redo", this);
    actionRedo->setShortcut(QKeySequence("Ctrl+Shift+Y"));
    connect(actionRedo, &QAction::triggered, this, &MainWindow::at_actionRedo_triggered);

    actionToggleOnTop = new QAction("Stay On Top", this);
    actionToggleOnTop->setCheckable(true);
    actionToggleOnTop->setChecked(IsOnTop());
    connect(actionToggleOnTop, &QAction::triggered, this, &MainWindow::at_actionToggleOnTop_triggered);

    actionToggleLock = new QAction("Lock Edits", this);
    actionToggleLock->setCheckable(true);
    actionToggleLock->setChecked(IsLocked());
    connect(actionToggleLock, &QAction::triggered, this, &MainWindow::at_actionToggleLock_triggered);

    actionExit = new QAction("Exit", this);
    actionExit->setShortcut(QKeySequence("Ctrl+X"));
    connect(actionExit, &QAction::triggered, this, &MainWindow::at_actionExit_triggered);
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
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}


bool MainWindow::eventFilter(QObject* obj, QEvent* evt)
{
    static QPoint startPos;
    static QSize startSize;
    static QPoint mouseStartPos;
    static MouseEvent::ActionE action{ MouseEvent::ActionE::None };

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

    if (MouseEvent::is_rmb_press(evt))
    {
        emit customContextMenuRequested(static_cast<QMouseEvent*>(evt)->globalPos());
        return true;
    }

    return false;
}

void MainWindow::at_customContextMenuRequested(const QPoint& pos)
{
    QMenu* menu = new QMenu;
    menu->setStyleSheet(StyleSheet::format_menu());

    actionSave->setEnabled(textEdit->document()->isModified() && HasFile());
    connect(actionSave, &QAction::triggered, this, &MainWindow::at_actionSave_triggered);
    menu->addAction(actionSave);

    menu->addAction(actionSaveAs);

    menu->addSeparator();

    // defaults -> submenu

    // Text edit actions
    menu->addSeparator();

    actionUndo->setEnabled(!IsLocked() && textEdit->document()->isUndoAvailable());
    menu->addAction(actionUndo);

    actionRedo->setEnabled(!IsLocked() && textEdit->document()->isRedoAvailable());
    menu->addAction(actionRedo);



    // save
    // save-as
    // new note
    // (un)lock
    // display on top
    // set transparent
    // next color scheme


    menu->addSeparator();

    QMenu* colorSchemesSubmenu = new QMenu("Select Color Scheme");
    colorSchemesSubmenu->setStyleSheet(StyleSheet::format_menu());
    for (const auto& scheme : ColorScheme::schemas)
    {
        const QString name = scheme.first;
        QAction* actionScheme = new QAction(name);
        store_color_scheme(actionScheme, name);
        connect(actionScheme, &QAction::triggered, this, &MainWindow::at_actionSetColorScheme_triggered);
        colorSchemesSubmenu->addAction(actionScheme);
    }
    menu->addMenu(colorSchemesSubmenu);

    menu->addSeparator();

    actionToggleOnTop->setChecked(IsOnTop());
    menu->addAction(actionToggleOnTop);

    actionToggleLock->setChecked(IsLocked());
    menu->addAction(actionToggleLock);

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



void MainWindow::at_actionExit_triggered()
{
    emit close();
}


void MainWindow::at_actionToggleOnTop_triggered()
{
    SetupWindowFlags(!IsOnTop());
}

void MainWindow::at_actionToggleLock_triggered()
{
    SetLocked(!IsLocked());
}


void MainWindow::at_actionUndo_triggered()
{
    if(IsLocked())
        return;
    textEdit->document()->undo();
}

void MainWindow::at_actionRedo_triggered()
{
    if (IsLocked())
        return;
    textEdit->document()->redo();
}


void MainWindow::at_actionCut_triggered() {}
void MainWindow::at_actionCopy_triggered() {}
void MainWindow::at_actionPaste_triggered() {}
void MainWindow::at_actionSelectAll_triggered() {}


void MainWindow::at_actionSetColorScheme_triggered()
{
    const QString schemeName = extract_color_scheme(sender());
    SetColorScheme(schemeName);
}


void MainWindow::SetColorScheme(const QString& name)
{
    if (!ColorScheme::schemas.count(name))
        return;
    setStyleSheet(StyleSheet::format_main_window(ColorScheme::schemas.at(name), 18));
    textEdit->setStyleSheet(StyleSheet::format_text_edit(ColorScheme::schemas.at(name), 18));
    statusBar()->setStyleSheet(StyleSheet::format_status_bar(ColorScheme::schemas.at(name), 18));
}


void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
}


void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            LoadFile(fileName);
    }
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
    setWindowModified(textEdit->document()->isModified());
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



bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
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
    setWindowModified(false);
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

    const QString unsavedTag = (HasUnsavedChanges() || !HasFile())
        ? "*"
        : "";

    setWindowTitle(tit + unsavedTag + " [PureNote]");
    //statusBar()->showMessage(filePath + unsavedTag);
    statusLabel->setText(filePath + unsavedTag);
}


QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

bool MainWindow::IsOnTop()
{
    return windowFlags() & Qt::WindowStaysOnTopHint;
}

void MainWindow::SetupWindowFlags(bool onTop)
{
    if(onTop)
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    else
        setWindowFlags(Qt::FramelessWindowHint);
    show();
}


