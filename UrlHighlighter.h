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
        m_urlFormat.setFontUnderline(true);
        m_urlFormat.setUnderlineStyle(QTextCharFormat::DotLine);
        m_urlFormat.setAnchor(true);
    }

protected:
    /*
    void highlightBlock(const QString &text) override
    {
        auto it = m_urlRegex.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), m_urlFormat);
        }
    }
    */

void highlightBlock(const QString &text) override
{
    // Clear previous anchors in this block
    /*
    QTextCursor cursor(currentBlock());
    cursor.setPosition(currentBlock().position());
    cursor.setPosition(currentBlock().position() + text.length(), QTextCursor::KeepAnchor);

    QTextCharFormat clearFmt;
    clearFmt.setAnchor(false);
    clearFmt.setAnchorHref(QString());
    cursor.mergeCharFormat(clearFmt);
    */

		QTextCursor cursor(currentBlock());
    // Apply new links
    auto it = m_urlRegex.globalMatch(text);
    while (it.hasNext()) 
    {
        auto match = it.next();
        QTextCharFormat fmt(m_urlFormat);
        //fmt.setForeground(Qt::blue);
        //fmt.setFontUnderline(true);
        //fmt.setAnchor(true);
        fmt.setAnchorHref(match.captured());

        cursor.setPosition(/*currentBlock().position() + */ match.capturedStart());
        cursor.setPosition(/*currentBlock().position() + */ match.capturedEnd(), QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(fmt);
    }
}

/*
    void highlightBlock(const QString &text) override
    {
        auto it = m_urlRegex.globalMatch(text);
        while (it.hasNext()) 
        {
            auto match = it.next();
            QTextCharFormat fmt(m_urlFormat);
            fmt.setAnchor(true);
            fmt.setAnchorHref(match.captured());
            //setFormat(match.capturedStart(), match.capturedLength(), fmt);

			QTextCursor cursor(document());
			cursor.setPosition(currentBlock().position() + match.capturedStart());
			cursor.setPosition(currentBlock().position() + match.capturedEnd(), QTextCursor::KeepAnchor);
            //cursor.beginEditBlock();
			cursor.mergeCharFormat(fmt);
            //cursor.mergeBlockCharFormat(fmt);
            //cursor.endEditBlock();

        }
    }
    */

private:
    QRegularExpression m_urlRegex;
    QTextCharFormat m_urlFormat;
};

