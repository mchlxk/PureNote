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

    while (!reader.atEnd() && !reader.hasError()) 
    {
        reader.readNext();

        if (reader.isStartElement() && reader.name() == "Style")
        {
			parse_style(reader, pun);
            // parse
        }
        else if (reader.isStartElement() && reader.name() == "Window")
        {
			parse_window(reader, pun);
        }
		else if (reader.isStartElement() && reader.name() == "Content")
		{
			parse_content(reader, pun);
		}
        else
        {
            return QString("unexpected element:" + reader.name());
        }
	}

    /*
    while (!reader.atEnd() && !reader.hasError()) 
    {
        reader.readNext();

        // Detect <book> start element
        if (reader.isStartElement() && reader.name() == "pun") 
        {
            // Read until </book>
            while (!(reader.isEndElement() && reader.name() == "content")) 
            {
                reader.readNext();


                if (xml.isStartElement()) 
                {
                    if (xml.name() == "title") 
                    {
                        book.title = xml.readElementText();
                    } else if (xml.name() == "author") 
                    {
                        book.author = xml.readElementText();
                    } else 
                    {
                        xml.skipCurrentElement(); // ignore unknown elements
                    }
                }
            }

            books.append(book);
        }
    }
    */



    /*
        reader.readNextStartElement();
    if(reader.name() != "content")
        return QString("parser error 2");

	content = reader.readElementText();
    */

    /*
    while (reader.readNextStartElement()) 
    {
        if (reader.name() == "content") 
        {
            content = reader.readElementText();
        }
        else 
        {
            reader.skipCurrentElement();
        }
    }
    */

    return pun;
}


QString PunParser::parse_style(QXmlStreamReader& reader, pun_t& output)
{
    return "";
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

    return "";
}

QString PunParser::parse_content(QXmlStreamReader& reader, pun_t& output)
{
    Pun::locked(output) = parse_bool_attribute(reader, "Locked");
    Pun::content(output) = reader.readElementText();
    return "";
}


bool PunParser::parse_bool_attribute(QXmlStreamReader& reader, const QString& attributeName)
{
    return reader.attributes().hasAttribute(attributeName) && parse_bool(reader.attributes().value(attributeName).toString());
}

bool PunParser::parse_bool(const QString& value) 
{
    return value.toLower() == "true"; 
}

