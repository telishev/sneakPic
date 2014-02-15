#ifndef SVG_WRITER_H
#define SVG_WRITER_H

class QString;
class QXmlStreamWriter;
class abstract_svg_item;

#include <map>

class svg_writer
{
public:
  svg_writer ();
  ~svg_writer ();

  bool write_to_file (const QString &filename, const abstract_svg_item *root);
  void write_item (const abstract_svg_item *root, QXmlStreamWriter &writer) const;
  std::map<QString, QString> get_used_namespaces (const abstract_svg_item *root) const;

private:
  void get_used_namespaces (const abstract_svg_item *root, std::map<QString, QString> &map) const;
};

#endif // SVG_WRITER_H
