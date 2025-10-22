#include <QVariant>

#include "ButtonBar.h"

#include "Style.h"
#include "SchemeIcon.h"


int ButtonBar::Geometry::get_width(int numButtons, int buttonSize)
{
	return ((1.5*numButtons) - 0.5) * buttonSize;
}

QPoint ButtonBar::Geometry::get_button_pos(int barWidth, int buttonIdx, int buttonSize)
{
	return QPoint(barWidth - buttonSize - (buttonIdx * 1.5 * buttonSize), 0);
}

void ButtonBar::Property::set_name(QObject* target, const QString& name)
{
	target->setProperty("name", name);
}

QString ButtonBar::Property::get_name(const QObject* target)
{
	return target->property("name").toString();
}

ButtonBar::T::T(QWidget* parent, int buttonSize)
: m_parent(parent)
, m_buttonSize(buttonSize)
{ 
}

bool ButtonBar::T::Contains(const QPoint& globalPos) const
{
	const QPoint topLeft = m_parent->mapToGlobal(QPoint(0, 0));
	return QRect(topLeft.x(), topLeft.y(), m_parent->width(), m_buttonSize).contains(globalPos);
}

int ButtonBar::T::GetMinimumWidth() const
{
	return Geometry::get_width((int)m_buttons.size(), m_buttonSize);
}

void ButtonBar::T::AddButton(const QString& name)
{
	GetButton(name);
}

void ButtonBar::T::SetButtonIcon(const QString& name, const QIcon& icon)
{
	GetButton(name)->setIcon(icon);
}

void ButtonBar::T::SetButtonStyleSheet(const QString& name, const QString& styleSheet)
{
	GetButton(name)->setStyleSheet(styleSheet);
}

void ButtonBar::T::SetButtonTooltip(const QString& name, const QString& tooltip)
{
	GetButton(name)->setToolTip(tooltip);
}

void ButtonBar::T::UpdatePerParentGeometry()
{
	int idx = 0;
	for (auto& btn : m_buttons)
	{
		const auto pos = Geometry::get_button_pos(m_parent->width(), idx, m_buttonSize);
		btn.second->setGeometry(pos.x(), pos.y(), m_buttonSize, m_buttonSize);
		++idx;
	}
}

void ButtonBar::T::Show()
{
	std::for_each(m_buttons.begin(), m_buttons.end(), [] (std::pair<const QString, QToolButton*>& btn) {
		btn.second->show();
	});
}

void ButtonBar::T::Hide()
{
	std::for_each(m_buttons.begin(), m_buttons.end(), [] (std::pair<const QString, QToolButton*>& btn) {
		btn.second->hide();
	});
}

QToolButton* ButtonBar::T::GetButton(const QString& name)
{
	if (!m_buttons.count(name))
	{
		QToolButton* b = new QToolButton(this->m_parent);
		b->setIconSize(QSize(this->m_buttonSize, this->m_buttonSize));
		Property::set_name(b, name);
		connect(b, &QToolButton::clicked, this, &T::at_buttonClicked);
		m_buttons[name] = b;
	}
	return m_buttons.at(name);
}

void ButtonBar::T::at_buttonClicked()
{
	emit button_clicked(Property::get_name(sender()));
}

