#include "PostProcessExecutor.h"

PostProcessExecutor::PostProcessExecutor(QTextDocument* parent)
: QSyntaxHighlighter(parent)
{
}

void PostProcessExecutor::AddPass(post_process_pass_t ppp)
{
	m_postProcessStack.push_back(ppp);
}

void PostProcessExecutor::at_textBlock_aboutToBeDestroyed(uint32_t id)
{
	for (auto& ppp : m_postProcessStack)
		PostProcessPass::block_destroyed(ppp)(id);
}

void PostProcessExecutor::highlightBlock(const QString& text)
{
	if (nullptr == currentBlockUserData())
	{
		TextBlockTracker* tracker = new TextBlockTracker();
		connect(tracker, &TextBlockTracker::about_to_be_destroyed, this, &PostProcessExecutor::at_textBlock_aboutToBeDestroyed);
		setCurrentBlockUserData(tracker);
	}
	const auto blockId = static_cast<TextBlockTracker*>(currentBlockUserData())->GetId();
	for (const auto& ppp : m_postProcessStack)
		ExecutePass(ppp, blockId, text);
}

void PostProcessExecutor::ExecutePass(const post_process_pass_t& ppp, uint32_t blockId, const QString& blockText)
{
	PostProcessPass::reset_block(ppp)(blockId);
	auto it = PostProcessPass::phrase_regex(ppp).globalMatch(blockText);
	while (it.hasNext())
	{
		const auto match = it.next();
		const int start = match.capturedStart();
		const int length = match.capturedLength();
		const auto format = PostProcessPass::format_phrase(ppp)(blockId, start, start+length, match.captured());
		if (!format.isEmpty())
			setFormat(start, length, format);
	}
}

