#pragma once

#include <unordered_map>

#include <QString>
#include <QToolButton>

#include "HashQString.h"

namespace ButtonBar
{
	namespace Geometry
	{
		constexpr int default_button_size{ 24 };
		int get_width(int numButtons, int buttonSize);
		QPoint get_button_pos(int barWidth, int buttonIdx, int buttonSize);
	}

	namespace Property
	{
		void set_name(QObject* target, const QString& name);
		QString get_name(const QObject* target);
	}

	class T
	: public QObject
	{
		Q_OBJECT

	public:
		T(QWidget* parent, int buttonSize);
		T() = delete;
		T(const T&) = delete;
		T& operator=(const T&) = delete;
		~T() {}

		void UpdatePerParentGeometry();
		void Show();
		void Hide();

		bool Contains(const QPoint& globalPos) const;
		int GetMinimumWidth() const;

		void AddButton(const QString& name);
		void SetButtonIcon(const QString& name, const QIcon& icon);
		void SetButtonStyleSheet(const QString& name, const QString& styleSheet);
		void SetButtonTooltip(const QString& name, const QString& tooltip);

		int GetButtonSize() const { return m_buttonSize; }

	signals:
		void button_clicked(const QString& name);

	private slots:
		void at_buttonClicked();

	private:
		QWidget* const m_parent{ nullptr };
		const int m_buttonSize;

		QToolButton* GetButton(const QString& name);

		std::unordered_map<QString, QToolButton*, hash_qstring> m_buttons;
	};
}
using button_bar_t = ButtonBar::T;


