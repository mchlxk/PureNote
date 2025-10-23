#include "State.h"


void State::T::Set(uint8_t tag)
{
	if (m_tags.count(tag))
		return;
	m_tags.insert(tag);
	emit state_changed(tag);
}

void State::T::Set(uint8_t tag, bool set)
{
	if (set == Get(tag))
		return;
	if (set)
		Set(tag);
	else
		Clear(tag);
	emit state_changed(tag);
}

void State::T::Clear(uint8_t tag)
{
	if (!m_tags.count(tag))
		return;
	m_tags.erase(tag);
	emit state_changed(tag);
}

void State::T::Toggle(uint8_t tag)
{
	if (m_tags.count(tag))
		m_tags.erase(tag);
	else
		m_tags.insert(tag);
	emit state_changed(tag);
}

bool State::T::Get(uint8_t tag) const
{
	return m_tags.count(tag);
}



