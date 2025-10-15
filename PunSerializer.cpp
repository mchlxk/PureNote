#include <QXmlStreamWriter>
#include <QByteArray>

#include "PunSerializer.h"

void PunSerializer::serialize(const pun_t& pun, QByteArray* output)
{
    QXmlStreamWriter writer(output);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("pun");
    writer.writeAttribute("version", "1.0");

    write_style(Pun::style(pun), writer);

    writer.writeStartElement("window");
    writer.writeTextElement("geometry", Pun::geometry(pun).toHex());
	writer.writeTextElement("opacity", QString::number(Pun::opacity(pun)));
	writer.writeTextElement("opaque_on_context", serialize_bool.at(Pun::opaque_on_context(pun)));
	writer.writeTextElement("on_top", serialize_bool.at(Pun::on_top(pun)));
	writer.writeTextElement("fullscreen", serialize_bool.at(Pun::fullscreen(pun)));
    writer.writeEndElement(); // window

    writer.writeStartElement("content");
	writer.writeTextElement("locked", serialize_bool.at(Pun::locked(pun)));
	writer.writeTextElement("text", Pun::content(pun));
    writer.writeEndElement(); // content


    writer.writeEndElement(); // pun
    writer.writeEndDocument();
}

void PunSerializer::write_style(const style_t& style, QXmlStreamWriter& writer)
{
    writer.writeStartElement("style");
	writer.writeTextElement("color_scheme", Style::color_scheme(style));
	writer.writeTextElement("font_family", Style::font_family(style));
	writer.writeTextElement("font_size", QString::number(Style::font_size(style)));
    writer.writeEndElement();
}
