#include <QStatusBar>

#include "EventHandler.h"

#include "EvtType.h"

#include "MainWindow.h"

std::pair<bool, EventHandler::T*> EventHandler::Idle::operator()(QEvent* evt)
{
	switch (EvtType::get(evt))
	{
		case EvtType::E::MouseMove:
		{
			const auto globalPos = static_cast<QMouseEvent*>(evt)->globalPos();
			if(m_parent->m_buttonBar->Contains(globalPos))
				m_parent->m_buttonBar->Show();
			else
				m_parent->m_buttonBar->Hide();
			break;
		}

		case EvtType::E::Enter:
		{
			m_parent->m_state.Set(State::Tag::HasMouseContext);
			break;
		}

		case EvtType::E::Leave:
		{
			m_parent->m_buttonBar->Hide();
			m_parent->m_state.Clear(State::Tag::HasMouseContext);
			break;
		}

		case EvtType::E::Activate:
		{
			m_parent->m_state.Set(State::Tag::HasDialogContext);
			break;
		}

		case EvtType::E::Deactivate:
		{
			m_parent->m_state.Clear(State::Tag::HasDialogContext);
			break;
		}

		case EvtType::E::LmbPressAlt:
			return { false, new EventHandler::LmbMove(m_parent, static_cast<QMouseEvent*>(evt)->globalPos()) };

		case EvtType::E::LmbPress:
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt);
			QRect grabArea(m_parent->statusBar()->frameGeometry());
			grabArea.setWidth(grabArea.width() - grabArea.height()); // leave space for size-grip
			if (grabArea.contains(mouseEvent->pos()))
				return { true, new EventHandler::LmbMove(m_parent, mouseEvent->globalPos()) };
			break;
		}

		case EvtType::E::MmbPress:
			return { false, new EventHandler::MmbMove(m_parent, static_cast<QMouseEvent*>(evt)->globalPos()) };

		case EvtType::E::RmbPressAlt:
			return { false, new EventHandler::RmbResize(m_parent, static_cast<QMouseEvent*>(evt)->globalPos()) };

		case EvtType::E::WheelUpCtrl:
		{
			m_parent->IncreaseFontSize();
			return { true, nullptr };
		}

		case EvtType::E::WheelDownCtrl:
		{
			m_parent->DecreaseFontSize();
			return { true, nullptr };
		}

		case EvtType::E::WheelUpAlt:
		{
			m_parent->IncreaseOpacity();
			return { true, nullptr };
		}

		case EvtType::E::WheelDownAlt:
		{
			m_parent->DecreaseOpacity();
			return { true, nullptr };
		}
	}

	return { false, nullptr };
}



EventHandler::MmbMove::MmbMove(MainWindow* parent, const QPoint& globalPos)
: T(parent)
, m_mouseStartPos(globalPos)
, m_parentStartPos(m_parent->pos()) 
{
}


std::pair<bool, EventHandler::T*> EventHandler::MmbMove::operator()(QEvent* evt)
{
	switch (EvtType::get(evt))
	{
		case EvtType::E::MouseMove:
		{
			const auto globalPos = static_cast<QMouseEvent*>(evt)->globalPos();
			m_parent->move(m_parentStartPos + (globalPos - m_mouseStartPos));
			break;
		}
		case EvtType::E::MmbRelease:
			return { false, new EventHandler::Idle(m_parent) };
	}

	return { false, nullptr };
}

EventHandler::LmbMove::LmbMove(MainWindow* parent, const QPoint& globalPos)
: T(parent)
, m_mouseStartPos(globalPos)
, m_parentStartPos(m_parent->pos()) 
{
}


std::pair<bool, EventHandler::T*> EventHandler::LmbMove::operator()(QEvent* evt)
{
	switch (EvtType::get(evt))
	{
		case EvtType::E::MouseMove:
		{
			const auto globalPos = static_cast<QMouseEvent*>(evt)->globalPos();
			m_parent->move(m_parentStartPos + (globalPos - m_mouseStartPos));
			break;
		}
		case EvtType::E::LmbRelease:
			return { false, new EventHandler::Idle(m_parent) };
	}

	return { false, nullptr };
}


EventHandler::RmbResize::RmbResize(MainWindow* parent, const QPoint& globalPos)
: T(parent)
, m_mouseStartPos(globalPos)
, m_parentStartSize(m_parent->size()) 
{
}


std::pair<bool, EventHandler::T*> EventHandler::RmbResize::operator()(QEvent* evt)
{
	switch (EvtType::get(evt))
	{
		case EvtType::E::MouseMove:
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (evt);
			const auto sizeChange = mouseEvent->globalPos() - m_mouseStartPos;
			m_parent->resize(m_parentStartSize.width() + sizeChange.x(), m_parentStartSize.height() + sizeChange.y());
			break;
		}

		case EvtType::E::RmbRelease:
			return { true, new EventHandler::Idle(m_parent) }; // true = mouse release consumed, do not show context menu
	}

	return { false, nullptr };
}

