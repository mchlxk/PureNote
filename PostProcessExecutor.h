
#include <QSyntaxHighlighter>

#include "PostProcessBlock.h"
#include "PostProcessSlot.h"

class PostProcessExecutor
: public QSyntaxHighlighter
{

public:

	PostProcessExecutor(QTextDocument* parent = nullptr)
	: QSyntaxHighlighter(parent)
	{
	}

	void AppendSlot(post_process_slot_t slot)
	{
		m_postProcessStack.push_back(slot);
	}

private slots:
	void at_postProcessBlock_aboutToBeDestroyed(uint32_t id)
	{
		for (auto& slot : m_postProcessStack)
			PostProcessSlot::block_destroyed(slot)(id);
	}

protected:
	void highlightBlock(const QString& text) override
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
			ProcessSlot(blockId, slot, text);
	}

private:
	std::vector<post_process_slot_t> m_postProcessStack;

	void ProcessSlot(uint32_t blockId, post_process_slot_t& slot, const QString& text)
	{
		auto it = PostProcessSlot::phrase_regex(slot).globalMatch(text);
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
};

