#include <QVariant>

#include "ButtonBar.h"

#include "Style.h"
#include "SchemeIcon.h"

ButtonBar::ButtonBar(QWidget* parent, int buttonSize)
: m_parent(parent)
, m_buttonSize(buttonSize)
{ 
}

bool ButtonBar::Contains(const QPoint& globalPos) const
{
	const QPoint topLeft = m_parent->mapToGlobal(QPoint(0, 0));
	return QRect(topLeft.x(), topLeft.y(), m_parent->width(), m_buttonSize).contains(globalPos);
}

int ButtonBar::GetMinimumWidth() const
{
	return ((1.5*m_buttons.size()) - 0.5) * m_buttonSize;
}

void ButtonBar::AddButton(const QString& name)
{
	GetButton(name);
}

void ButtonBar::SetButtonIcon(const QString& name, const QIcon& icon)
{
	GetButton(name)->setIcon(icon);
}

void ButtonBar::SetButtonStyleSheet(const QString& name, const QString& styleSheet)
{
	GetButton(name)->setStyleSheet(styleSheet);
}

void ButtonBar::SetButtonTooltip(const QString& name, const QString& tooltip)
{
	GetButton(name)->setToolTip(tooltip);
}

void ButtonBar::UpdatePerParentGeometry()
{
	int x = m_parent->width() - m_buttonSize;
	for (auto& btn : m_buttons)
	{
		btn.second->setGeometry(x, 0, m_buttonSize, m_buttonSize);
		x -= 1.5*m_buttonSize;
	}
}

void ButtonBar::Show()
{
	std::for_each(m_buttons.begin(), m_buttons.end(), [] (std::pair<const QString, QToolButton*>& btn) {
		btn.second->show();
	});
}

void ButtonBar::Hide()
{
	std::for_each(m_buttons.begin(), m_buttons.end(), [] (std::pair<const QString, QToolButton*>& btn) {
		btn.second->hide();
	});
}

QToolButton* ButtonBar::GetButton(const QString& name)
{
	if (!m_buttons.count(name))
	{
		QToolButton* b = new QToolButton(this->m_parent);
		b->setIconSize(QSize(this->m_buttonSize, this->m_buttonSize));
		b->setProperty("name", name);
		connect(b, &QToolButton::clicked, this, &ButtonBar::at_buttonClicked);
		m_buttons[name] = b;
	}
	return m_buttons.at(name);
}

void ButtonBar::at_buttonClicked()
{
	emit button_clicked(sender()->property("name").toString());
}

