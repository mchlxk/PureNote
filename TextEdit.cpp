#include "TextEdit.h"

TextEdit::TextEdit(QWidget* parent)
: QPlainTextEdit(parent)
, m_postProcess(new PostProcessExecutor(document()))
{
}

PostProcessExecutor* TextEdit::PostProcess()
{
	return m_postProcess;
}

pun_optional_t<std::pair<uint32_t, int>> TextEdit::GetBlockPosition(const QPoint& globalPos) const
{
	const QTextCursor cursor = cursorForPosition(mapFromGlobal(globalPos));
	if(cursor.isNull())
		return Pun::Optional::none;
	const auto* blockData = cursor.block().userData();
	if (!blockData)
		return Pun::Optional::none;
	const uint32_t blockId = static_cast<const TextBlockTracker*>(blockData)->GetId();
	const int posInBlock = cursor.positionInBlock();
	return std::pair<uint32_t, int>{blockId, posInBlock};
}


