#pragma once

#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class UrlHighlighter 
: public QSyntaxHighlighter
{
public:
    UrlHighlighter(QTextDocument *parent = nullptr)
	: QSyntaxHighlighter(parent)
    {
        m_urlRegex = QRegularExpression(R"((https?://[^\s]+))");
        m_formatUrlTemplate.setFontUnderline(true);
        m_formatUrlTemplate.setUnderlineStyle(QTextCharFormat::DotLine);
        m_formatUrlTemplate.setAnchor(true);
    }

protected:

	void highlightBlock(const QString &text) override
	{
        QTextCursor blockCursor(currentBlock());
        // Clear existing anchors in this block
        blockCursor.select(QTextCursor::BlockUnderCursor);
        QTextCharFormat clearFmt;
        clearFmt.setAnchor(false);
        clearFmt.setAnchorHref(QString());
        blockCursor.mergeCharFormat(clearFmt);

		auto it = m_urlRegex.globalMatch(text);
		while (it.hasNext()) 
		{
			QTextCursor cursor(currentBlock());
			auto match = it.next();
			QTextCharFormat fmt(m_formatUrlTemplate);
			fmt.setAnchorHref(match.captured());
			cursor.setPosition(match.capturedStart());
			cursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
			cursor.mergeCharFormat(fmt);
		}
	}

private:
    QRegularExpression m_urlRegex;
    QTextCharFormat m_formatUrlTemplate;
};

