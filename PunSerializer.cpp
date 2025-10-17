#include <QXmlStreamWriter>
#include <QByteArray>

#include "PunSerializer.h"

void PunSerializer::serialize(const pun_t& pun, QByteArray* output)
{
    QXmlStreamWriter writer(output);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    { ElementScope punScope(writer, "Pun");
        writer.writeAttribute("Version", "1.0");
        write_style(Pun::style(pun), writer);
        write_window(Pun::window(pun), writer);
		write_content(Pun::content(pun), writer);
    }

    writer.writeEndDocument();
}

void PunSerializer::write_style(const style_t& style, QXmlStreamWriter& writer)
{
    ElementScope styleScope(writer, "Style");
	writer.writeTextElement("ColorScheme", Style::color_scheme(style));
	{ ElementScope fontScope(writer, "Font");
		writer.writeTextElement("Size", QString::number(Style::font_size(style)));
		writer.writeTextElement("Family", Style::font_family(style));
	}
}

void PunSerializer::write_window(const window_t& window, QXmlStreamWriter& writer)
{
	ElementScope windowScope(writer, "Window");
	writer.writeAttribute("Opacity", QString::number(Window::opacity(window)));
	writer.writeAttribute("OpaqueWhenActive", serialize_bool.at(Window::opaque_when_active(window)));
	writer.writeAttribute("OnTop", serialize_bool.at(Window::on_top(window)));
	writer.writeAttribute("Fullscreen", serialize_bool.at(Window::fullscreen(window)));
	writer.writeTextElement("Geometry", Window::geometry(window).toHex());
}

void PunSerializer::write_content(const content_t& content, QXmlStreamWriter& writer)
{
	ElementScope contentScope(writer, "Content");
	writer.writeAttribute("Locked", serialize_bool.at(Content::locked(content)));
	writer.writeCharacters(Content::text(content));
}

