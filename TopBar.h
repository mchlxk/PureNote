#pragma once

#include <QToolButton>

class TopBar
: public QObject
{
	Q_OBJECT

public:
	TopBar(QWidget* parent, int buttonSize);

	void UpdatePerParentGeometry();
	void SetColorScheme(const QString& colorScheme);

	void Show();
	void Hide();

	bool Contains(const QPoint& point) const;

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


