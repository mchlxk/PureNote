#include <QtWidgets>

#include "Mainwindow.h"
#include "MouseEvent.h"

MainWindow::MainWindow()
: textEdit(new QPlainTextEdit)
{
    QCoreApplication::instance()->installEventFilter(this);

    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QMainWindow{font-size: 18px; color: #5a5255; background: #fae0ad}");
    setCentralWidget(textEdit);
    
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::at_customContextMenuRequested);

    /*
    #f0adb0 	(240,173,176)
	#f5c2ab 	(245,194,171)
	#fae0ad 	(250,224,173)
	#c6d7b2 	(198,215,178)
	#c4def0 	(196,222,240)
    
    
    QLabel {
        background-image: url(dense6pattern.png);
        background-repeat: repeat-xy;
    }


    */

    SetOnTop(true);

    
    textEdit->setStyleSheet("QPlainTextEdit{font-size: 18px; color: #5a5255; background: #fae0ad}");
    textEdit->document()->setDocumentMargin(10);

    createActions();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::at_document_contentsChanged);

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}


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

    QAction* saveAction = new QAction("Save", this);
    saveAction->setEnabled(textEdit->document()->isModified() && HasFile());
    menu->addAction(saveAction);


    QAction* toggleOnTopAction = new QAction("Display On Top", this);
    toggleOnTopAction->setCheckable(true);
    toggleOnTopAction->setChecked(IsOnTop());
    connect(toggleOnTopAction, &QAction::triggered, this, &MainWindow::at_contextMenu_toggleOnTop);
    menu->addAction(toggleOnTopAction);


    // save
    // save-as
    // new note
    // (un)lock
    // display on top
    // set transparent
    // next color scheme

    menu->addSeparator();

    // defaults -> submenu

    menu->addSeparator();

    // text edit actions

    menu->addSeparator();

    QAction* exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::at_contextMenu_exit);
    menu->addAction(exitAction);

    menu->exec(pos);
}


void MainWindow::at_contextMenu_exit()
{
    emit close();
}


void MainWindow::at_contextMenu_toggleOnTop()
{
    SetOnTop(!IsOnTop());
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
            loadFile(fileName);
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

 
void MainWindow::createActions()
{
    /*
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
    */
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
    statusBar()->setStyleSheet("QStatusBar{font-size: 18px; color: #5a5255; background: #fae0ad}");
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

 
 void MainWindow::loadFile(const QString &fileName)
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
    statusBar()->showMessage(tr("File loaded"), 2000);
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
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}



void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


bool MainWindow::IsOnTop()
{
    return windowFlags() & Qt::WindowStaysOnTopHint;
}

void MainWindow::SetOnTop(bool enabled)
{
    if(enabled)
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    else
    {
        setWindowFlags(((windowFlags() & Qt::WindowStaysOnTopHint) ^ Qt::WindowStaysOnTopHint));
        raise();
    }
}

