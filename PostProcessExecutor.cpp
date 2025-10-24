#include "PostProcessExecutor.h"

PostProcessExecutor::PostProcessExecutor(QTextDocument* parent)
: QSyntaxHighlighter(parent)
{
}

void PostProcessExecutor::AppendSlot(post_process_slot_t slot)
{
	m_postProcessStack.push_back(slot);
}

void PostProcessExecutor::at_postProcessBlock_aboutToBeDestroyed(uint32_t id)
{
	for (auto& slot : m_postProcessStack)
		PostProcessSlot::block_destroyed(slot)(id);
}

void PostProcessExecutor::highlightBlock(const QString& text)
{
	if (nullptr == currentBlockUserData())
	{
		PostProcessBlock* data = new PostProcessBlock();
		connect(data, &PostProcessBlock::about_to_be_destroyed, this, &PostProcessExecutor::at_postProcessBlock_aboutToBeDestroyed);
		setCurrentBlockUserData(data);
	}

	const auto blockId = static_cast<PostProcessBlock*>(currentBlockUserData())->GetId();
	for (auto& slot : m_postProcessStack)
		PostProcessSlot::block_init(slot)(blockId);

	for (auto& slot : m_postProcessStack)
		ProcessPerSlot(slot, blockId, text);
}

void PostProcessExecutor::ProcessPerSlot(post_process_slot_t& slot, uint32_t blockId, const QString& blockText)
{
	auto it = PostProcessSlot::phrase_regex(slot).globalMatch(blockText);
	while (it.hasNext())
	{
		auto match = it.next();
		const int start = match.capturedStart();
		const int length = match.capturedLength();
		const auto format = PostProcessSlot::format_phrase(slot)(blockId, start, start+length, match.captured());
		if (!format.isEmpty())
			setFormat(start, length, format);
	}
}

