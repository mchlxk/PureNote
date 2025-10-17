#pragma once

#include <stack>

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QTimer>

#include "State.h"
#include "Style.h"
#include "Pun.h"

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
    void at_document_contentsChanged();
    void at_customContextMenuRequested(const QPoint&);
    void at_textEdit_customContextMenuRequested(const QPoint&);

    // file actions
    void at_actionSave_triggered();
    void at_actionSaveAs_triggered();

    // text edit actions
    void at_actionUndo_triggered();
    void at_actionRedo_triggered();
    void at_actionCut_triggered();
    void at_actionCopy_triggered();
    void at_actionPaste_triggered();
    void at_actionDelete_triggered();
    void at_actionSelectAll_triggered();

    void at_actionSetColorScheme_triggered();
    void at_actionNextColorScheme_triggered();
    void at_actionNextFont_triggered();
    void at_actionSetFont_triggered();
    void at_actionDecreaseFontSize_triggered();
    void at_actionIncreaseFontSize_triggered();
    void at_actionDecreaseOpacity_triggered();
    void at_actionIncreaseOpacity_triggered();
    void at_actionSetFontSize_triggered();
    void at_actionSetOpacity_triggered();
    void at_actionToggleOnTop_triggered();
    void at_actionToggleLocked_triggered();
    void at_actionToggleFullscreen_triggered();
    void at_actionToggleOpaqueWhenActive_triggered();
    void at_actionExit_triggered();

    void at_opacityAdjustTimer_expired();

private:
    pun_t GetPun() const;
    void SetPun(const pun_t& pun, const QString& filePath);
    window_t GetWindow() const;
    void SetWindow(const window_t& window);

    QByteArray GetGeometry() const;
    void PushGeometry(const QByteArray&);
    void PushGeometry();
    void PopGeometry();
    bool CanPopGeometry() const;
    QByteArray PeekGeometry() const;

    void readSettings();
    void writeSettings();

    bool ResolveUnsavedChanges();
    bool Save();
    bool SaveAs();
    bool Save(const QString filePath);
    void SetFile(const QString &filePath);
    void About();
    QString GetBrowseFilename();

    bool HasFile() const { return !m_filePath.isEmpty(); }

    void SetupWindowFlags(bool onTop);

    void SetupActions();
    void SetupStatusLabel();
    void SetupTextEdit();
    void SetupContextMenu();

    void ShowContextMenu(const QPoint& pos);

    void UpdatePerUnsaved();
    void UpdateStatusBar();
    void SetStyle(const style_t& style);
    void UpdatePerStyle();
    void UpdatePerOpacity();

    void UpdatePerOnTopState();
    void UpdatePerFullscreen();
    void UpdatePerLocked();

    void DecreaseFontSize();
    void IncreaseFontSize();
    void DecreaseOpacity();
    void IncreaseOpacity();

    // file actions
    QAction* m_actionSave{ nullptr };
    QAction* m_actionSaveAs{ nullptr };

    // text edit actions
    QAction* m_actionUndo{ nullptr };
    QAction* m_actionRedo{ nullptr };
    QAction* m_actionCut{ nullptr };
    QAction* m_actionCopy{ nullptr };
    QAction* m_actionPaste{ nullptr };
    QAction* m_actionDelete{ nullptr };
    QAction* m_actionSelectAll{ nullptr };

    QAction* m_actionNextColorScheme{ nullptr };
    QAction* m_actionNextFont{ nullptr };
    QAction* m_actionDecreaseFontsize{ nullptr };
    QAction* m_actionIncreaseFontsize{ nullptr };
    QAction* m_actionDecreaseOpacity{ nullptr };
    QAction* m_actionIncreaseOpacity{ nullptr };
    QAction* m_actionToggleOnTop{ nullptr };
    QAction* m_actionToggleLocked{ nullptr };
    QAction* m_actionToggleFullscreen{ nullptr };
    QAction* m_actionToggleOpaqueWhenActive{ nullptr };
    QAction* m_actionExit{ nullptr };

    QPlainTextEdit *m_textEdit{ nullptr };
    QLabel* m_statusLabel{ nullptr };
    QString m_filePath;

    style_t m_style{ Style::defaults };
    float m_opacity{ 1.f };
    std::stack<QByteArray> m_geometryStack;

    State::tags_t m_stateTags;
    QTimer m_opacityAdjustTimer;
};


