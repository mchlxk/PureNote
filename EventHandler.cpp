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

		case EvtType::E::MmbPress:
			return { false, new EventHandler::MmbMove(m_parent, static_cast<QMouseEvent*>(evt)->globalPos()) };

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

