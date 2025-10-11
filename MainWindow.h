#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>

using style_t = std::tuple<QString, uint32_t>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void LoadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject* obj, QEvent* e) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();

    void at_document_contentsChanged();
    void at_customContextMenuRequested(const QPoint&);

    void at_actionSaveAs_triggered();
    void at_actionExit_triggered();
    void at_actionToggleOnTop_triggered();
    void at_actionToggleLock_triggered();
    void at_actionSave_triggered();
    void at_actionUndo_triggered();
    void at_actionRedo_triggered();
    void at_actionCut_triggered();
    void at_actionCopy_triggered();
    void at_actionPaste_triggered();
    void at_actionSelectAll_triggered();
    void at_actionSetColorScheme_triggered();


private:
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    bool HasFile() const { return !curFile.isEmpty(); }
    bool HasUnsavedChanges() const { return textEdit->document()->isModified(); }
    bool HasTitle() const { return !title.isEmpty(); }

    bool IsOnTop();
    void SetupWindowFlags(bool enabled);
    void SetLocked(bool locked) { textEdit->setReadOnly(locked); }
    bool IsLocked() { return textEdit->isReadOnly(); }

    void SetupActions();
    void SetupStatusLabel();
    void SetupTextEdit();
    void SetupContextMenu();

    void UpdatePerFile();
    void SetStyle(const style_t& style);
    void UpdatePerStyle();

    QAction* actionSave{ nullptr };
    QAction* actionSaveAs{ nullptr };
    QAction* actionUndo{ nullptr };
    QAction* actionRedo{ nullptr };
    QAction* actionCut{ nullptr };
    QAction* actionCopy{ nullptr };
    QAction* actionPaste{ nullptr };
    QAction* actionSelectAll{ nullptr };
    QAction* actionToggleOnTop{ nullptr };
    QAction* actionToggleLock{ nullptr };
    QAction* actionExit{ nullptr };

    QPlainTextEdit *textEdit{ nullptr };
    QLabel* statusLabel{ nullptr };

    QString curFile;
    QString title;

    style_t m_style{ "Flamingo", 18 };
};
