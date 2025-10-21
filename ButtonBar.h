#pragma once

#include <QToolButton>

class ButtonBar
: public QObject
{
	Q_OBJECT

public:
	ButtonBar(QWidget* parent, int buttonSize);
	ButtonBar() = delete;
	ButtonBar(const ButtonBar&) = delete;
	ButtonBar& operator=(const ButtonBar&) = delete;
	~ButtonBar() {}

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


