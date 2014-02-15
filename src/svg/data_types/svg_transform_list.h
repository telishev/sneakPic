#ifndef SVG_TRANSFORM_LIST_H
#define SVG_TRANSFORM_LIST_H

class QString;

#include <QTransform>

class svg_transform_list
{
  QTransform m_transform;
public:
  bool read (const char *data);
  bool write (QString &data) const;

  QTransform transform () const { return m_transform; }
  void set_transform (const QTransform &transform) { m_transform = transform; }

private:
  bool get_transfrom_data (const char *data, vector<double> &dest) const;
  bool apply_transform (const std::string &name, vector<double> &data);
};

#endif // SVG_TRANSFORM_LIST_H
