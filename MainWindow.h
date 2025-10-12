#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>

#include "State.h"
#include "Style.h"

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
    void at_actionToggleOnTop_triggered();
    void at_actionToggleLocked_triggered();
    void at_actionSave_triggered();
    void at_actionUndo_triggered();
    void at_actionRedo_triggered();
    void at_actionCut_triggered();
    void at_actionCopy_triggered();
    void at_actionPaste_triggered();
    void at_actionSelectAll_triggered();
    void at_actionSetColorScheme_triggered();
    void at_actionSetFontSize_triggered();
    void at_actionToggleFullscreen_triggered();
    void at_actionExit_triggered();

private:
    void readSettings();
    void writeSettings();
    bool ResolveUnsavedChanges();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    bool HasFile() const { return !curFile.isEmpty(); }
    bool HasTitle() const { return !title.isEmpty(); }

    void SetupWindowFlags(bool onTop);

    void SetupActions();
    void SetupStatusLabel();
    void SetupTextEdit();
    void SetupContextMenu();

    void UpdatePerUnsaved();
    void UpdatePerFile();
    void SetStyle(const style_t& style);
    void UpdatePerStyle();

    void UpdateOnTopPerState();
    void UpdatePerFullscreen();
    void UpdatePerLocked();

    QAction* actionSave{ nullptr };
    QAction* actionSaveAs{ nullptr };
    QAction* actionUndo{ nullptr };
    QAction* actionRedo{ nullptr };
    QAction* actionCut{ nullptr };
    QAction* actionCopy{ nullptr };
    QAction* actionPaste{ nullptr };
    QAction* actionSelectAll{ nullptr };
    QAction* actionToggleOnTop{ nullptr };
    QAction* actionToggleLocked{ nullptr };
    QAction* actionToggleFullscreen{ nullptr };
    QAction* actionExit{ nullptr };

    QPlainTextEdit *textEdit{ nullptr };
    QLabel* statusLabel{ nullptr };

    QString curFile;
    QString title;

    style_t m_style{ Style::defaults };

    State::tags_t m_stateTags;
};

