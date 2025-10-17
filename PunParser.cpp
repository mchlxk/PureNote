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

    while (!Utility::at_element_end(reader, "Pun"))
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
			const auto window = parse_window(reader);
            if (!window)
                return window.get_error();
            Pun::window(pun) = *window;
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
    while (!Utility::at_element_end(reader, "Style"))
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
    while (!Utility::at_element_end(reader, "Font"))
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


Pun::expected<window_t, QString> PunParser::parse_window(QXmlStreamReader& reader)
{
    window_t window{ Window::defaults };

    Window::opaque_when_active(window) = Utility::parse_bool_attribute(reader, "OpaqueWhenActive");
    Window::on_top(window) = Utility::parse_bool_attribute(reader, "OnTop");
    Window::fullscreen(window) = Utility::parse_bool_attribute(reader, "Fullscreen");

    if (reader.attributes().hasAttribute("Opacity"))
    {
        bool ok{ true };
        const float opacity = reader.attributes().value("Opacity").toFloat(&ok);
        if(ok)
			Window::opacity(window) = opacity;
    }

    while (!Utility::at_element_end(reader, "Window"))
    {
        reader.readNext();
        if (reader.hasError())
            return reader.errorString();

        if (reader.isStartElement() && reader.name() == "Geometry")
        {
            const QString geometryText = reader.readElementText();
            if (geometryText.isEmpty())
                continue;
            Window::geometry(window) = QByteArray::fromHex(geometryText.toUtf8());
			continue;
        }

		if (reader.isStartElement())
			reader.skipCurrentElement();
    }

    return window;
}

Pun::expected<std::pair<QString, bool>, QString> PunParser::parse_content(QXmlStreamReader& reader)
{
    std::pair<QString, bool> content{"", false};
    content.second = Utility::parse_bool_attribute(reader, "Locked");
    content.first = reader.readElementText();
    return content;
}


bool PunParser::Utility::at_element_end(QXmlStreamReader& reader, const QString& name)
{
    return (reader.isEndElement() && reader.name() == name) || reader.atEnd();
}

bool PunParser::Utility::parse_bool_attribute(const QXmlStreamReader& reader, const QString& attributeName)
{
    return reader.attributes().hasAttribute(attributeName) && parse_bool(reader.attributes().value(attributeName).toString());
}

bool PunParser::Utility::parse_bool(const QString& value) 
{
    return value.toLower() == "true"; 
}

