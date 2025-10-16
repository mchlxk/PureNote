#include <QXmlStreamReader>

#include "PunParser.h"

Pun::expected<pun_t, QString> PunParser::parse(const QByteArray& input)
{
    QString content;
    QXmlStreamReader reader(input);

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


    reader.readNextStartElement();
    if (reader.name() != "pun")
        return QString("parser error 1");

    reader.readNextStartElement();
    if(reader.name() != "content")
        return QString("parser error 2");

	content = reader.readElementText();

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

    return content;
}

