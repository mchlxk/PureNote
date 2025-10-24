#pragma once

#include <unordered_map>
#include <tuple>
#include <vector>

#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class UrlDetector 
: public QSyntaxHighlighter
{
    using url_item_t = std::tuple<int, int, QString>;
    using url_array_t = std::vector<url_item_t>;
    using block_url_array_t = std::unordered_map<int, url_array_t>;

public:
    UrlDetector(QTextDocument* parent = nullptr);

    QString GetUrl(int blockId, int pos) const;

    bool HasUrl(int blockId, int pos) const;

protected:

    void highlightBlock(const QString& text) override;

private:
    QRegularExpression m_urlRegex;
    QTextCharFormat m_formatUrlTemplate;
    block_url_array_t m_blockUrls;
};

