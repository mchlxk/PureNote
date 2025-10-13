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
	painter.setBrush(QBrush(QColor(ColorScheme::background(scheme))));
	painter.drawEllipse(0, 0, size, size);
	return QIcon(pixmap);
}


