#include "UrlDetector.h"

UrlDetector::UrlDetector(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	m_urlRegex = QRegularExpression(R"(https?://[^\s]+|www\.[^\s]+)");
	m_formatUrlTemplate.setFontUnderline(true);
	m_formatUrlTemplate.setUnderlineStyle(QTextCharFormat::DotLine);
}

QString UrlDetector::GetUrl(int blockId, int pos) const
{
	const auto found = std::find_if(m_blockUrls.at(blockId).cbegin(), m_blockUrls.at(blockId).cend(), [pos] (const url_item_t& item) {
		return pos >= std::get<0>(item) && pos <= std::get<1>(item);
	});
	if (found == m_blockUrls.at(blockId).cend())
		return "";
	return std::get<2>(*found);
}

bool UrlDetector::HasUrl(int blockId, int pos) const
{
	const auto found = std::find_if(m_blockUrls.at(blockId).cbegin(), m_blockUrls.at(blockId).cend(), [pos] (const url_item_t& item) {
		return pos >= std::get<0>(item) && pos <= std::get<1>(item);
	});
	return found != m_blockUrls.at(blockId).cend();
}

void UrlDetector::highlightBlock(const QString &text)
{
	const int blockNum = currentBlock().blockNumber();
	m_blockUrls[blockNum].clear();

	auto it = m_urlRegex.globalMatch(text);
	while (it.hasNext()) 
	{
		auto match = it.next();
		QTextCharFormat fmt(m_formatUrlTemplate);
		const int start = match.capturedStart();
		const int length = match.capturedLength();
		setFormat(start, length, fmt);
		m_blockUrls[blockNum].emplace_back(start, start+length, match.captured());
	}
}


