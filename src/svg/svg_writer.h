#ifndef SVG_WRITER_H
#define SVG_WRITER_H

class QString;
class QXmlStreamWriter;
class abstract_svg_item;

#include <map>

class svg_writer
{
  const abstract_svg_item *m_root;
public:
  svg_writer (const abstract_svg_item *root);
  ~svg_writer ();

  bool write (const QString &filename_arg);

private:
  void get_used_namespaces (const abstract_svg_item *root, std::map<QString, QString> &map) const;
  void write_item (const abstract_svg_item *root, QXmlStreamWriter &writer) const;
};

#endif // SVG_WRITER_H
