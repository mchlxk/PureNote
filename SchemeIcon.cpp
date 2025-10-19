#include <QPixmap>
#include <QPainter>

#include "SchemeIcon.h"


QIcon SchemeIcon::get(const color_scheme_t& scheme, const uint32_t size)
{
	QPixmap pixmap(size, size);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	//painter.setRenderHint(QPainter::HighQualityAntialiasing);
	painter.setPen(QColor(ColorScheme::color(scheme)));
	painter.setBrush(QBrush(QColor(ColorScheme::background(scheme))));
	painter.drawEllipse(0, 0, size, size);
	return QIcon(pixmap);
}


QPixmap SchemeIcon::get_warning(const color_scheme_t& scheme, const uint32_t size)
{
	QPixmap icon(size, size);
	icon.fill(Qt::transparent);
	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing);
	//painter.setRenderHint(QPainter::HighQualityAntialiasing);
	painter.setPen(QColor(ColorScheme::background(scheme)));
	painter.setBrush(QBrush(QColor(ColorScheme::background(scheme))));
	painter.drawEllipse(0, 0, size, size);
	const uint32_t characterHalfWidth = size / 10;
	const QBrush fillBrush(QColor(ColorScheme::accent(scheme)));
	painter.fillRect((size/2) - characterHalfWidth, characterHalfWidth, 2*characterHalfWidth, size - (5*characterHalfWidth), fillBrush);
	painter.fillRect((size/2) - characterHalfWidth, size - (3*characterHalfWidth), 2*characterHalfWidth, 2*characterHalfWidth, fillBrush);
	return icon;
}

