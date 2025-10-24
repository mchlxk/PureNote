#include "PostProcessBlock.h"

uint32_t PostProcessBlock::next_id{ 0 };


PostProcessBlock::PostProcessBlock(QObject* parent)
: QObject(parent)
, m_id(next_id++)
{
	connect(this, &QObject::destroyed, this, &PostProcessBlock::at_qobject_destroyed);
}

uint32_t PostProcessBlock::GetId() const 
{
	return m_id; 
}

void PostProcessBlock::at_qobject_destroyed() 
{
	emit about_to_be_destroyed(m_id); 
}

