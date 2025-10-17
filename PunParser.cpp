#include <QXmlStreamReader>

#include "PunParser.h"

Pun::expected<pun_t, QString> PunParser::parse(const QByteArray& input)
{
    pun_t pun;
    QXmlStreamReader reader(input);

    reader.readNextStartElement();
    if (reader.name() != "Pun")
        return QString("pun element not found");

    if(!reader.attributes().hasAttribute("Version"))
        return QString("unknown version");

    if(reader.attributes().value("Version") != "1.0")
        return QString("wrong version");

    while (!(reader.isEndElement() && reader.name() == "Pun") && !reader.atEnd())
    {
        reader.readNext();
        if (reader.hasError())
            return reader.errorString();

        if (reader.isStartElement() && reader.name() == "Style")
        {
            const auto style = parse_style(reader);
            if (!style)
                return style.get_error();
            Pun::style(pun) = *style;
            continue;
        }

        if (reader.isStartElement() && reader.name() == "Window")
        {
			parse_window(reader, pun);
            continue;
        }

		if (reader.isStartElement() && reader.name() == "Content")
		{
			const auto content = parse_content(reader);
            if (!content)
                return content.get_error();
            Pun::content(pun) = (*content).first;
            Pun::locked(pun) = (*content).second;
            continue;
		}

        if (reader.isStartElement())
            reader.skipCurrentElement();
	}

    return pun;
}


Pun::expected<style_t, QString> PunParser::parse_style(QXmlStreamReader& reader)
{
    style_t style{ Style::defaults };
    while (!(reader.isEndElement() && reader.name() == "Style") && !reader.atEnd())
    {
        reader.readNext();
        if (reader.hasError())
            return reader.errorString();

        if (reader.isStartElement() && reader.name() == "ColorScheme")
        {
            Style::color_scheme(style) = reader.readElementText();
            continue;
        }

        if (reader.isStartElement() && reader.name() == "Font")
        {
            const auto font = parse_font(reader);
            if (!font)
                return font.get_error();
            Style::font_family(style) = (*font).first;
            Style::font_size(style) = (*font).second;
            continue;
        }

        if(reader.isStartElement())
			reader.skipCurrentElement();
    }
    return style;
}

Pun::expected<std::pair<QString, int>, QString> PunParser::parse_font(QXmlStreamReader& reader)
{
    std::pair<QString, int> font{Style::font_family(Style::defaults), Style::font_size(Style::defaults)};
    while (!(reader.isEndElement() && reader.name() == "Font") && !reader.atEnd())
    {
        reader.readNext();
        if (reader.hasError())
            return reader.errorString();

        if (reader.isStartElement() && reader.name() == "Size")
        {
            bool ok{ true };
            font.second = reader.readElementText().toUInt(&ok);
            if (!ok)
                return QString("error parsing font size (not a number)");
            continue;
        }

        if (reader.isStartElement() && reader.name() == "Family")
        {
            font.first = reader.readElementText();
            continue;
        }

        if(reader.isStartElement())
			reader.skipCurrentElement();
    }
    return font;
}


QString PunParser::parse_window(QXmlStreamReader& reader, pun_t& output)
{
    Pun::opaque_when_active(output) = parse_bool_attribute(reader, "OpaqueWhenActive");
    Pun::on_top(output) = parse_bool_attribute(reader, "OnTop");
    Pun::fullscreen(output) = parse_bool_attribute(reader, "Fullscreen");

    if (reader.attributes().hasAttribute("Opacity"))
        Pun::opacity(output) = reader.attributes().value("Opacity").toFloat();
    else
        Pun::opacity(output) = 1.f;

    while (!(reader.isEndElement() && reader.name() == "Window") && !reader.atEnd())
    {
        reader.readNext();
        if (reader.hasError())
            return reader.errorString();

        if (reader.isStartElement() && reader.name() == "Geometry")
        {
            const QString geometryText = reader.readElementText();
            if (geometryText.isEmpty())
                continue;
            Pun::geometry(output) = QByteArray::fromHex(geometryText.toUtf8());
			continue;
        }

		if (reader.isStartElement())
			reader.skipCurrentElement();
    }

    return "";
}

Pun::expected<std::pair<QString, bool>, QString> PunParser::parse_content(QXmlStreamReader& reader)
{
    std::pair<QString, bool> content;
    content.second = parse_bool_attribute(reader, "Locked");
    content.first = reader.readElementText();
    return content;
}


bool PunParser::parse_bool_attribute(QXmlStreamReader& reader, const QString& attributeName)
{
    return reader.attributes().hasAttribute(attributeName) && parse_bool(reader.attributes().value(attributeName).toString());
}

bool PunParser::parse_bool(const QString& value) 
{
    return value.toLower() == "true"; 
}

