#include "internal_clipboard_format.h"

#include <QXmlStreamWriter>
#include <QTransform>


#include "editor/items_selection.h"
#include "items/items_comparison.h"
#include "items/abstract_svg_item.h"
#include "svg_writer.h"
#include "svg_document.h"
#include "svg_namespaces.h"
#include "svg_reader.h"
#include "editor/operations/add_item_operation.h"
#include "items/svg_graphics_item.h"
#include "editor/operations/transform_item_operation.h"
#include "renderer/svg_painter.h"
#include "common/memory_deallocation.h"




internal_clipboard_format::internal_clipboard_format (svg_document *document)
{
  m_document = document;
}

internal_clipboard_format::~internal_clipboard_format ()
{

}

void internal_clipboard_format::fill_by_selection (items_selection *selection)
{
  m_items = std::vector<abstract_svg_item *> (selection->begin (), selection->end ());
  std::sort (m_items.begin (), m_items.end (), items_comparison_z_order ());
}

QByteArray internal_clipboard_format::pack () const
{
  svg_writer writer;
  QBuffer buf;
  buf.open (QIODevice::WriteOnly);
  QXmlStreamWriter xml_writer (&buf);
  std::map<QString, QString> namespaces = writer.get_used_namespaces (m_document->root ());
  for (auto namespace_pair : namespaces)
    xml_writer.writeNamespace (namespace_pair.first, namespace_pair.second);

  xml_writer.writeDefaultNamespace (svg_namespaces::svg_uri ());
  xml_writer.writeStartDocument ();
  {
    xml_writer.writeStartElement (svg_namespaces::sneakpic_uri (), "clipboard");
    {
      write_version (xml_writer);
      write_items (xml_writer);
    }
    xml_writer.writeEndElement ();

  }
  xml_writer.writeEndDocument();

  return buf.data ();
}

void internal_clipboard_format::unpack (const QByteArray &data)
{
  QBuffer buf;
  buf.setData (data);
  buf.open (QIODevice::ReadOnly);
  QXmlStreamReader xml_reader (&buf);
  while (!xml_reader.atEnd ())
    {
      if (xml_reader.readNext () == QXmlStreamReader::StartElement)
        {
          if (xml_reader.namespaceUri () != svg_namespaces::sneakpic_uri ())
            continue;

          if (xml_reader.name () == "version")
            {
              if (!check_version (xml_reader))
                return;
            }
          else if (xml_reader.name () == "items")
            {
              if (!read_items (xml_reader))
                return;
            }
        }
    }

}

QString internal_clipboard_format::mime_type () const
{
  return "application/sneakpic";
}

void internal_clipboard_format::write_version (QXmlStreamWriter &xml_writer) const
{
  xml_writer.writeStartElement (svg_namespaces::sneakpic_uri (), "version");
  {
    xml_writer.writeAttribute (svg_namespaces::sneakpic_uri (), "value", QString::number (version ()));
  }
  xml_writer.writeEndElement ();
}

void internal_clipboard_format::write_items (QXmlStreamWriter &xml_writer) const
{
  svg_writer writer;
  xml_writer.writeStartElement (svg_namespaces::sneakpic_uri (), "items");
  {
    for (auto &&item : m_items)
      {
        writer.write_item (item, xml_writer);
      }
  }
  xml_writer.writeEndElement ();

}

int internal_clipboard_format::version () const
{
  return 1;
}

void internal_clipboard_format::apply_to_doc (svg_painter *painter, QPointF cur_pos)
{
  if (!painter->document ()->can_add_items ())
    return;
  QRectF bbox = calculate_bbox ();
  QPointF offset = !cur_pos.isNull () ? cur_pos - bbox.center () : QPointF (0.0f, 0.0f);
  QTransform transform = QTransform::fromTranslate (offset.x (), offset.y ());
  painter->selection ()->clear ();
  add_item_operation add_op (painter, false);
  add_op.set_apply_style (false);

  for (auto &&item : m_items)
    {
      add_op.apply (item);
      transform_item_operation (m_document).apply_transform (transform, item);
    }

  m_document->apply_changes ("Paste");
}

bool internal_clipboard_format::check_version (QXmlStreamReader &xml_reader)
{
  QString version_str = xml_reader.attributes ().value (svg_namespaces::sneakpic_uri (), "value").toString ();
  int xml_version = version_str.toInt ();
  return xml_version == version ();
}

bool internal_clipboard_format::read_items (QXmlStreamReader &xml_reader)
{
  svg_reader reader (m_document->get_undo_handler (), m_document->item_factory (), m_document);
  if (!reader.read_steam (xml_reader))
    return false;

  auto roots = reader.all_roots ();
  for (auto &&item : roots)
    item->process_after_read ();

  m_items.insert (m_items.end (), roots.begin (), roots.end ());

  return true;
}

QRectF internal_clipboard_format::calculate_bbox () const
{
  QRectF bbox;
  for (auto &&item : m_items)
    {
      svg_graphics_item *graphics_item = item->to_graphics_item ();
      if (!graphics_item)
        continue;

      graphics_item->update_bbox ();
      bbox = bbox.united (graphics_item->bbox ());
    }

  return bbox;
}
