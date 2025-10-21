#pragma once

#include <unordered_map>

#include <QString>
#include <QToolButton>

#include "HashQString.h"

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
	void Show();
	void Hide();

	bool Contains(const QPoint& globalPos) const;
	int GetMinimumWidth() const;

	void AddButton(const QString& name);
	void SetButtonIcon(const QString& name, const QIcon& icon);

	int GetButtonSize() const { return m_buttonSize; }

signals:
	void button_clicked(const QString& name);

private slots:
	void at_buttonClicked();

private:
	QWidget* const m_parent{ nullptr };
	const int m_buttonSize;

	QToolButton* ButtonClose();
	QToolButton* ButtonMinimize();
	QToolButton* ButtonTopLock();

	QToolButton* GetButton(const QString& name);

	std::unordered_map<QString, QToolButton*, hash_qstring> m_buttons;
};


