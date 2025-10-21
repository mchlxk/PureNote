#pragma once

#include <QToolButton>

class TopBar
: public QObject
{
	Q_OBJECT

public:
	TopBar(QWidget* parent, int buttonSize);
	TopBar() = delete;
	TopBar(const TopBar&) = delete;
	TopBar& operator=(const TopBar&) = delete;
	~TopBar() {}

	void UpdatePerParentGeometry();
	void SetColorScheme(const QString& colorScheme);
	void SetTopLockChecked(bool checked);

	void Show();
	void Hide();

	bool Contains(const QPoint& point) const;
	int GetMinimumWidth() const;

signals:
	void top_lock();
	void minimize();
	void close();

private:
	QWidget* const m_parent{ nullptr };
	const int m_buttonSize;

	QToolButton* ButtonClose();
	QToolButton* ButtonMinimize();
	QToolButton* ButtonTopLock();
};


