#ifndef INTERNAL_CLIPBOARD_FORMAT_H
#define INTERNAL_CLIPBOARD_FORMAT_H

class svg_document;
class abstract_svg_item;
class items_selection;
class QByteArray;
class QXmlStreamWriter;
class svg_painter;

class internal_clipboard_format
{
  svg_document *m_document;
  std::vector<abstract_svg_item *> m_items;
public:
  internal_clipboard_format (svg_document *document);
  ~internal_clipboard_format ();

  void fill_by_selection (items_selection *selection);
  void apply_to_doc (svg_painter *painter, QPointF cur_pos);

  QByteArray pack ();
  void unpack (QByteArray &data);

  QString mime_type () const;

private:
  void write_version (QXmlStreamWriter &xml_writer);
  void write_items (QXmlStreamWriter &xml_writer);
  int version () const;
  bool check_version (QXmlStreamReader &xml_reader);
  bool read_items (QXmlStreamReader &xml_reader);
  QRectF calculate_bbox () const;
};

#endif // INTERNAL_CLIPBOARD_FORMAT_H
