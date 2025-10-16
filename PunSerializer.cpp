#include <QXmlStreamWriter>
#include <QByteArray>

#include "PunSerializer.h"

void PunSerializer::serialize(const pun_t& pun, QByteArray* output)
{
    QXmlStreamWriter writer(output);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    { ElementScope punScope(writer, "pun");

        writer.writeAttribute("version", "1.0");

        write_style(Pun::style(pun), writer);

        { ElementScope windowScope(writer, "window");
            writer.writeAttribute("opacity", QString::number(Pun::opacity(pun)));
            writer.writeAttribute("is_opaque_on_context", serialize_bool.at(Pun::opaque_on_context(pun)));
            writer.writeAttribute("is_on_top", serialize_bool.at(Pun::on_top(pun)));
            writer.writeAttribute("is_fullscreen", serialize_bool.at(Pun::fullscreen(pun)));
            writer.writeTextElement("geometry", Pun::geometry(pun).toHex());
        }

        { ElementScope contentScope(writer, "content");
            writer.writeAttribute("is_locked", serialize_bool.at(Pun::locked(pun)));
            writer.writeCharacters(Pun::content(pun));
        }
    }

    writer.writeEndDocument();
}

void PunSerializer::write_style(const style_t& style, QXmlStreamWriter& writer)
{
    { ElementScope styleScope(writer, "style");
        writer.writeTextElement("color_scheme", Style::color_scheme(style));
        { ElementScope fontScope(writer, "font");
            writer.writeTextElement("size", QString::number(Style::font_size(style)));
            writer.writeTextElement("family", Style::font_family(style));
        }

    }
}
