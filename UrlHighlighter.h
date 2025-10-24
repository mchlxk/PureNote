#pragma once

#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class UrlHighlighter 
: public QSyntaxHighlighter
{
    using url_item_t = std::tuple<int, int, QString>;
    using url_array_t = std::vector<url_item_t>;
    using block_url_array_t = std::unordered_map<int, url_array_t>;

public:
    UrlHighlighter(QTextDocument *parent = nullptr)
	: QSyntaxHighlighter(parent)
    {
        m_urlRegex = QRegularExpression(R"((https?://[^\s]+))");
        m_formatUrlTemplate.setFontUnderline(true);
        m_formatUrlTemplate.setUnderlineStyle(QTextCharFormat::DotLine);
        m_formatUrlTemplate.setAnchor(true);
    }

    QString GetUrl(int blockId, int pos) const
    {
        const auto found = std::find_if(m_blockUrls.at(blockId).cbegin(), m_blockUrls.at(blockId).cend(), [pos] (const url_item_t& item) {
            return pos >= std::get<0>(item) && pos <= std::get<1>(item);
		});
        if (found == m_blockUrls.at(blockId).cend())
            return "";
        return std::get<2>(*found);
    }

    bool HasUrl(int blockId, int pos) const
    {
        const auto found = std::find_if(m_blockUrls.at(blockId).cbegin(), m_blockUrls.at(blockId).cend(), [pos] (const url_item_t& item) {
            return pos >= std::get<0>(item) && pos <= std::get<1>(item);
		});
        return found != m_blockUrls.at(blockId).cend();
    }

protected:

	void highlightBlock(const QString &text) override
	{
        /*
        QTextCursor blockCursor(currentBlock());
        // Clear existing anchors in this block
        blockCursor.select(QTextCursor::BlockUnderCursor);
        QTextCharFormat clearFmt;
        clearFmt.setAnchor(false);
        clearFmt.setAnchorHref(QString());
        blockCursor.mergeCharFormat(clearFmt);
        */

        const int blockNum = currentBlock().blockNumber();
        m_blockUrls[blockNum].clear();

		auto it = m_urlRegex.globalMatch(text);
		while (it.hasNext()) 
		{
			//QTextCursor cursor(currentBlock());
			auto match = it.next();
			QTextCharFormat fmt(m_formatUrlTemplate);
			//fmt.setAnchorHref(match.captured());
            const int start = match.capturedStart();
            const int length = match.capturedLength();
            setFormat(start, length, fmt);
            m_blockUrls[blockNum].emplace_back(start, start+length, match.captured());
			//cursor.setPosition(match.capturedStart(), QTextCursor::MoveAnchor);
			//cursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
			//cursor.mergeCharFormat(fmt);
		}
	}

private:
    QRegularExpression m_urlRegex;
    QTextCharFormat m_formatUrlTemplate;
    block_url_array_t m_blockUrls;
};

