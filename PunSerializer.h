#pragma once

#include <unordered_map>

#include <QString>
#include <QByteArray>

#include "Pun.h"

namespace PunSerializer
{
    void serialize(const pun_t& pun, QByteArray* output);
    void write_style(const style_t& style, QXmlStreamWriter& writer);
    void write_window(const window_t& window, QXmlStreamWriter& writer);
    void write_content(const content_t& content, QXmlStreamWriter& writer);

    static const std::unordered_map<bool, QString> serialize_bool = { { true, "true"} ,{ false, "false"} };

    class ElementScope
    {
    public:
        [[nodiscard]] ElementScope(QXmlStreamWriter& writer, const QString& name) : writer_(writer) { writer.writeStartElement(name); }
        ~ElementScope() { writer_.writeEndElement(); }

        ElementScope() = delete;
        ElementScope(const ElementScope&) = delete;
        ElementScope(ElementScope&&) = delete;
        ElementScope& operator=(const ElementScope&) = delete;
    private:
        QXmlStreamWriter& writer_;
    };
}

