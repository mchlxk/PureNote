#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>

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

    void at_contextMenu_exit();
    void at_contextMenu_toggleOnTop();

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    bool HasFile() { return !curFile.isEmpty(); }

    bool IsOnTop();
    void SetupWindowFlags(bool enabled);

    QPlainTextEdit *textEdit;
    QString curFile;
};
