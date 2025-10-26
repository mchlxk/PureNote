#pragma once

#include <memory>

#include <QEvent>
#include <QPoint>
#include <QSize>

class MainWindow;

namespace EventHandler
{
	struct T
	{
		T(MainWindow* parent) : m_parent(parent) {}
		virtual ~T() {}
		virtual std::pair<bool, T*> operator()(QEvent* evt) { return { false, nullptr }; }
	protected:
		MainWindow* m_parent;
	};

	struct Idle
	: public T
	{
		Idle(MainWindow* parent) : T(parent) {}
		std::pair<bool, T*> operator()(QEvent* evt) override;
	};

	struct MmbMove
	: public T
	{
		MmbMove(MainWindow* parent, const QPoint& globalPos);
		std::pair<bool, T*> operator()(QEvent* evt) override;
	private:
		const QPoint m_mouseStartPos;
		const QPoint m_parentStartPos;
	};

	struct LmbMove
	: public T
	{
		LmbMove(MainWindow* parent, const QPoint& globalPos);
		std::pair<bool, T*> operator()(QEvent* evt) override;
	private:
		const QPoint m_mouseStartPos;
		const QPoint m_parentStartPos;
	};

	struct RmbResize
	: public T
	{
		RmbResize(MainWindow* parent, const QPoint& globalPos);
		std::pair<bool, T*> operator()(QEvent* evt) override;
	private:
		const QPoint m_mouseStartPos;
		const QSize m_parentStartSize;
	};
}
using event_handler_t = EventHandler::T;


