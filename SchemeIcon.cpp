#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QPolygon>

#include "SchemeIcon.h"


QPixmap SchemeIcon::get_menu_icon(const color_scheme_t& scheme, int size)
{
	QPixmap pixmap(size, size);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(ColorScheme::color(scheme)));
	painter.setBrush(QBrush(QColor(ColorScheme::background(scheme))));
	painter.drawEllipse(0, 0, size, size);
	return pixmap;
}


QPixmap SchemeIcon::get_warning_icon(const color_scheme_t& scheme, int size)
{
	QPixmap icon(size, size);
	icon.fill(Qt::transparent);
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(ColorScheme::background(scheme)));
	painter.setBrush(QBrush(QColor(ColorScheme::background(scheme))));
	painter.drawEllipse(0, 0, size, size);
	const uint32_t characterHalfWidth = size / 10;
	const QBrush fillBrush(QColor(ColorScheme::accent(scheme)));
	painter.fillRect((size/2) - characterHalfWidth, characterHalfWidth, 2*characterHalfWidth, size - (5*characterHalfWidth), fillBrush);
	painter.fillRect((size/2) - characterHalfWidth, size - (3*characterHalfWidth), 2*characterHalfWidth, 2*characterHalfWidth, fillBrush);
	return icon;
}

QPixmap SchemeIcon::get_window_icon(const color_scheme_t& scheme, int size)
{
	QPixmap icon(size, size);
	icon.fill(QColor(ColorScheme::background(scheme)));
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);
	QVector<QPoint> foldShape;
	const int foldSize = size / 2;
	foldShape.append({size, 0});
	foldShape.append({size - foldSize, 0});
	foldShape.append({size, foldSize});
	QPolygon foldPolygon(foldShape);
	QPainterPath foldPath;
	foldPath.addPolygon(foldPolygon);
	const QBrush accentBrush(QColor(ColorScheme::accent(scheme)));
	painter.fillPath(foldPath, accentBrush);
	return icon;
}

QPixmap SchemeIcon::get_close_icon(const color_scheme_t& scheme, int size)
{
	QPixmap icon(size, size);
	icon.fill(QColor(ColorScheme::accent(scheme)));
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);

	const QBrush fillBrush(QColor(ColorScheme::background(scheme)));
	const int margin = size / 8;
	const int squareSize = (size-2*margin) / 3;
	painter.fillRect(margin, margin, squareSize, squareSize, fillBrush);
	painter.fillRect(margin+2*squareSize, margin, squareSize, squareSize, fillBrush);
	painter.fillRect(margin+squareSize, margin+squareSize, squareSize, squareSize, fillBrush);
	painter.fillRect(margin, margin+2*squareSize, squareSize, squareSize, fillBrush);
	painter.fillRect(margin+2*squareSize, margin+2*squareSize, squareSize, squareSize, fillBrush);

	return icon;
}

QPixmap SchemeIcon::get_minimize_icon(const color_scheme_t& scheme, int size)
{
	QPixmap icon(size, size);
	icon.fill(QColor(ColorScheme::accent(scheme)));
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);

	const QBrush fillBrush(QColor(ColorScheme::background(scheme)));
	const int margin = size / 8;
	const int squareSize = (size-2*margin) / 3;
	painter.fillRect(margin, margin+2*squareSize, 3*squareSize, squareSize, fillBrush);

	return icon;
}

QPixmap SchemeIcon::get_top_lock_on_icon(const color_scheme_t& scheme, int size)
{
	QPixmap icon(size, size);
	icon.fill(QColor(ColorScheme::accent(scheme)));
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);

	const int margin = size / 8;
	const int squareSize = (size-2*margin) / 3;

	const QBrush fillBrush(QColor(ColorScheme::background(scheme)));
	painter.fillRect(margin, margin, 3*squareSize, squareSize, fillBrush);
	painter.fillRect(margin+squareSize, margin+squareSize, squareSize, 2*squareSize, fillBrush);

	return icon;
}

QPixmap SchemeIcon::get_top_lock_off_icon(const color_scheme_t& scheme, int size)
{
	QPixmap icon(size, size);
	icon.fill(QColor(ColorScheme::accent(scheme)));
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);

	const int margin = size / 8;
	const int squareSize = (size-2*margin) / 3;

	const QBrush fillBrush(QColor(ColorScheme::background(scheme)));
	painter.fillRect(margin+squareSize, margin, squareSize, 2*squareSize, fillBrush);
	painter.fillRect(margin, margin+(2*squareSize), 3*squareSize, squareSize, fillBrush);

	return icon;
}


