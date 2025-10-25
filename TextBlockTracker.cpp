#include "TextBlockTracker.h"

uint32_t TextBlockTracker::next_id{ 0 };


TextBlockTracker::TextBlockTracker(QObject* parent)
: QObject(parent)
, m_id(next_id++)
{
	connect(this, &QObject::destroyed, this, &TextBlockTracker::at_qobject_destroyed);
}

uint32_t TextBlockTracker::GetId() const 
{
	return m_id; 
}

void TextBlockTracker::at_qobject_destroyed() 
{
	emit about_to_be_destroyed(m_id); 
}

