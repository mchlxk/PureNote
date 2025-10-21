#include "ButtonBar.h"

#include "Style.h"
#include "SchemeIcon.h"

ButtonBar::ButtonBar(QWidget* parent, int buttonSize)
: m_parent(parent)
, m_buttonSize(buttonSize)
{ 
}

bool ButtonBar::Contains(const QPoint& point) const
{
	return QRect(0, 0, m_parent->width(), m_buttonSize).contains(point);
}


int ButtonBar::GetMinimumWidth() const
{
	return 4*m_buttonSize;
}


QToolButton* ButtonBar::ButtonClose()
{
	static QToolButton* btn = [this] {
		QToolButton* b = new QToolButton(this->m_parent);
		b->setStyleSheet("QToolButton{ border: 0;}");
		b->setIconSize(QSize(this->m_buttonSize, this->m_buttonSize));
		connect(b, &QToolButton::clicked, this, &ButtonBar::close);
		return b;
	} ();
	return btn;
}

QToolButton* ButtonBar::ButtonMinimize()
{
	static QToolButton* btn = [this] {
		QToolButton* b = new QToolButton(this->m_parent);
		b->setStyleSheet("QToolButton{ border: 0;}");
		b->setIconSize(QSize(this->m_buttonSize, this->m_buttonSize));
		connect(b, &QToolButton::clicked, this, &ButtonBar::minimize);
		return b;
	} ();
	return btn;
}

QToolButton* ButtonBar::ButtonTopLock()
{
	static QToolButton* btn = [this] {
		QToolButton* b = new QToolButton(this->m_parent);
		b->setStyleSheet("QToolButton{ border: 0;}");
		b->setIconSize(QSize(this->m_buttonSize, this->m_buttonSize));
		b->setCheckable(true);
		connect(b, &QToolButton::clicked, this, &ButtonBar::top_lock);
		return b;
	} ();
	return btn;
}

void ButtonBar::UpdatePerParentGeometry()
{
	ButtonClose()->setGeometry(m_parent->width() - m_buttonSize, 0, m_buttonSize, m_buttonSize);
	ButtonMinimize()->setGeometry(m_parent->width() - m_buttonSize - m_buttonSize - (m_buttonSize / 2), 0, m_buttonSize, m_buttonSize);
	ButtonTopLock()->setGeometry(m_parent->width() - m_buttonSize - m_buttonSize - (m_buttonSize / 2) - m_buttonSize - (m_buttonSize / 2), 0, m_buttonSize, m_buttonSize);
}

void ButtonBar::Show()
{
	ButtonClose()->show();
	ButtonMinimize()->show();
	ButtonTopLock()->show();
}

void ButtonBar::Hide()
{
	ButtonClose()->hide();
	ButtonMinimize()->hide();
	ButtonTopLock()->hide();
}

void ButtonBar::SetColorScheme(const QString& colorScheme)
{
	ButtonClose()->setIcon(SchemeIcon::get_close_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));
	ButtonMinimize()->setIcon(SchemeIcon::get_minimize_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));

	if(ButtonTopLock()->isChecked())
		ButtonTopLock()->setIcon(SchemeIcon::get_top_lock_on_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));
	else
		ButtonTopLock()->setIcon(SchemeIcon::get_top_lock_off_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));
}

void ButtonBar::SetTopLockChecked(bool checked)
{
	ButtonTopLock()->setChecked(checked);
}



