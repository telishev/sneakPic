#include "svg/svg_writer.h"

#include <QFile>
#include <QString>
#include <QXmlStreamWriter>
#include <map>

#include "svg/items/abstract_svg_item.h"
#include "svg/svg_namespaces.h"
#include "items/svg_character_data.h"

svg_writer::svg_writer (const abstract_svg_item *root)
{
  m_root = root;
}

svg_writer::~svg_writer ()
{

}

bool svg_writer::write (const QString &filename)
{
  QFile file (filename);
  if (!file.open (QIODevice::WriteOnly))
    return false;

  QXmlStreamWriter writer (&file);
  writer.setAutoFormatting(true);
  std::map<QString, QString> namespaces;
  get_used_namespaces (m_root, namespaces);
  for (auto namespace_pair : namespaces)
    writer.writeNamespace (namespace_pair.first, namespace_pair.second);

  writer.writeDefaultNamespace (svg_namespaces::svg_uri ());
  writer.writeStartDocument();
  write_item (m_root, writer);
  writer.writeEndDocument();
  return false;
}

void svg_writer::get_used_namespaces (const abstract_svg_item *root, std::map<QString, QString> &map) const
{
  if (!root)
    return;

  if (*root->namespace_uri ())
    map.insert (std::make_pair (root->namespace_uri (), root->namespace_name ()));

  std::vector<const abstract_attribute *> attributes_list = root->attributes_list ();

  for (const abstract_attribute *attribute : attributes_list)
    {
      if (*attribute->namespace_uri ())
        map.insert (std::make_pair (attribute->namespace_uri (), attribute->namespace_name ()));
    }

  for (int i = 0; i < root->children_count (); i++)
    get_used_namespaces (root->child (i), map);
}

void svg_writer::write_item (const abstract_svg_item *root, QXmlStreamWriter &writer) const
{
  if (root->is_cloned ())
    return;

  if (root->is_character_data ())
    {
      const svg_character_data *data = static_cast<const svg_character_data *> (root);
      writer.writeCharacters (data->char_data ());
      return;
    }

  writer.writeStartElement (root->namespace_uri (), root->type_name ());

  std::vector<const abstract_attribute *> attributes_list = root->attributes_list ();
  for (const abstract_attribute *attribute : attributes_list)
    {
      QString value;
      if (attribute->is_inherited ())
        value = "inherit";
      else
        attribute->write (value);

      /// Qt ignores default namespace for atributes, force it manually
      if (attribute->namespace_type () == svg_namespaces_t::SVG)
        writer.writeAttribute (attribute->type_name (), value);
      else
        writer.writeAttribute (attribute->namespace_uri (), attribute->type_name (), value);
    }

  for (int i = 0; i < root->children_count (); i++)
    {
      write_item (root->child (i), writer);
    }

  writer.writeEndElement ();
}