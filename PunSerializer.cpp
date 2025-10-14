#include <QXmlStreamWriter>
#include <QByteArray>

#include "PunSerializer.h"

void PunSerializer::serialize(const QString& text, QByteArray* output)
{
    QXmlStreamWriter writer(output);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("pun");

	writer.writeTextElement("content", text);

    writer.writeEndElement(); // root
    writer.writeEndDocument();
}

