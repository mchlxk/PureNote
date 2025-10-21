#include "TopBar.h"

#include "Style.h"
#include "SchemeIcon.h"

TopBar::TopBar(QWidget* parent, int buttonSize)
: m_parent(parent)
, m_buttonSize(buttonSize)
{
	m_buttonClose = new QToolButton(m_parent);
	m_buttonClose->setStyleSheet("QToolButton{ border: 0;}");
	connect(m_buttonClose, &QToolButton::clicked, this, &TopBar::close);


	m_buttonClose->setGeometry(640 - m_buttonSize, 0, m_buttonSize, m_buttonSize);
	m_buttonClose->setIcon(SchemeIcon::get_close_icon(ColorScheme::schemas.at("Peach"), m_buttonSize));
	m_buttonClose->setIconSize(QSize(m_buttonSize, m_buttonSize));
	m_buttonClose->show();


	m_buttonMinimize = new QToolButton(m_parent);
	m_buttonMinimize->setStyleSheet("QToolButton{ border: 0;}");
	connect(m_buttonMinimize, &QToolButton::clicked, this, &TopBar::minimize);

	m_buttonTopLock = new QToolButton(m_parent);
	m_buttonTopLock->setStyleSheet("QToolButton{ border: 0;}");
	connect(m_buttonTopLock, &QToolButton::clicked, this, &TopBar::top_lock);
}

void TopBar::UpdatePerParentGeometry()
{
	m_buttonClose->setGeometry(m_parent->width() - m_buttonSize, 0, m_buttonSize, m_buttonSize);
	m_buttonMinimize->setGeometry(m_parent->width() - m_buttonSize - m_buttonSize - (m_buttonSize / 2), 0, m_buttonSize, m_buttonSize);
	m_buttonTopLock->setGeometry(m_parent->width() - m_buttonSize - m_buttonSize - (m_buttonSize / 2) - m_buttonSize - (m_buttonSize / 2), 0, m_buttonSize, m_buttonSize);
}

void TopBar::Show()
{
	m_buttonClose->show();
	m_buttonMinimize->show();
	m_buttonTopLock->show();
}

void TopBar::Hide()
{
	m_buttonClose->hide();
	m_buttonMinimize->hide();
	m_buttonTopLock->hide();
}


void TopBar::SetColorScheme(const QString& colorScheme)
{
	m_buttonClose->setIcon(SchemeIcon::get_close_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));
	m_buttonClose->setIconSize(QSize(m_buttonSize, m_buttonSize));

	m_buttonMinimize->setIcon(SchemeIcon::get_minimize_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));
	m_buttonMinimize->setIconSize(QSize(m_buttonSize, m_buttonSize));

	m_buttonTopLock->setIcon(SchemeIcon::get_top_lock_off_icon(ColorScheme::schemas.at(colorScheme), m_buttonSize));
	m_buttonTopLock->setIconSize(QSize(m_buttonSize, m_buttonSize));
}


