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

        { ElementScope windowScope(writer, "Window");
            writer.writeAttribute("Opacity", QString::number(Pun::opacity(pun)));
            writer.writeAttribute("OpaqueWhenActive", serialize_bool.at(Pun::opaque_when_active(pun)));
            writer.writeAttribute("OnTop", serialize_bool.at(Pun::on_top(pun)));
            writer.writeAttribute("Fullscreen", serialize_bool.at(Pun::fullscreen(pun)));
            writer.writeTextElement("Geometry", Pun::geometry(pun).toHex());
        }

        { ElementScope contentScope(writer, "Content");
            writer.writeAttribute("Locked", serialize_bool.at(Pun::locked(pun)));
            writer.writeCharacters(Pun::content(pun));
        }
    }

    writer.writeEndDocument();
}

void PunSerializer::write_style(const style_t& style, QXmlStreamWriter& writer)
{
    { ElementScope styleScope(writer, "Style");
        writer.writeTextElement("ColorScheme", Style::color_scheme(style));
        { ElementScope fontScope(writer, "Font");
            writer.writeTextElement("Size", QString::number(Style::font_size(style)));
            writer.writeTextElement("Family", Style::font_family(style));
        }

    }
}
