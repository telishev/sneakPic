#include "svg/attributes/svg_attribute_transform.h"

#include <QStringList>

#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/items/abstract_svg_item.h"


svg_attribute_transform::svg_attribute_transform (abstract_svg_item *item)
  : abstract_attribute (item)
{

}

svg_attribute_transform::~svg_attribute_transform ()
{

}

bool svg_attribute_transform::read (const QString &data, bool /*from_css*/)
{
  return read (data.toUtf8 ().constData ());
}

bool svg_attribute_transform::write (QString &data, bool /*to_css*/) const 
{
  double m11 = m_transform.m11 (), m21 = m_transform.m21 (), m31 = m_transform.m31 ();
  double m12 = m_transform.m12 (), m22 = m_transform.m22 (), m32 = m_transform.m32 ();
  data = QString ("matrix(%1,%2,%3,%4,%5,%6)").arg (m11).arg (m12).arg (m21).arg (m22).arg (m31).arg (m32);
  return true;
}

QTransform svg_attribute_transform::computed_transform () const
{
  const abstract_svg_item *parent = item () ? item ()->parent () : nullptr;
  if (!parent)
    return m_transform;

  const svg_attribute_transform *base_transform = parent->get_computed_attribute <svg_attribute_transform> ();
  return m_additional_transform * m_transform * base_transform->computed_transform ();
}

void svg_attribute_transform::set_additional_transform (const QTransform &additional_transform)
{
  m_additional_transform = additional_transform;
}

bool svg_attribute_transform::read (const char *data)
{
  trim_whitespace_left (data);
  std::vector<double> transform_data;
  while (*data)
    {
      std::string transform, transform_data_string;
      CHECK (extract_chunk ('(', data, transform));
      CHECK (extract_chunk (')', data, transform_data_string));
      CHECK (get_transfrom_data (transform_data_string.c_str (), transform_data));
      CHECK (apply_transform (transform, transform_data));
    }

  return true;
}

bool svg_attribute_transform::get_transfrom_data (const char *data, std::vector<double> &dest) const
{
  QStringList splitted_data = QString (data).split (",\0x20\0x9\0xD\0xA");
  dest.resize (splitted_data.size ());

  for (int i = 0; i < splitted_data.size (); i++)
    {
      bool ok;
      dest[i] = splitted_data[i].toDouble (&ok);
      if (!ok)
        return false;
    }

  return true;
}

bool svg_attribute_transform::apply_transform (const std::string &name, std::vector<double> &data)
{
  const double degree_to_rad = 0.0174532925;
  QTransform new_transform;
  if (name == "matrix")
    {
      if (data.size () != 6)
        return false;

      new_transform = QTransform (data[0], data[1], data[2], data[3], data[4], data[5]);
    }
  else if (name == "translate")
    {
      if (data.size () != 2 && data.size () != 1)
        return false;

      double tx, ty;
      tx = data[0];
      ty = data.size () == 1 ? tx : data[1];

      new_transform.translate (tx, ty);
    }
  else if (name == "scale")
    {
      if (data.size () != 2 && data.size () != 1)
        return false;

      double sx, sy;
      sx = data[0];
      sy = data.size () == 1 ? sx : data[1];

      new_transform.scale (sx, sy);
    }
  else if (name == "rotate")
    {
      if (data.size () == 1)
        new_transform.rotate (data[0]);
      else if (data.size () == 3)
        {
          new_transform.translate (data[1], data[2]);
          new_transform.rotate (data[0]);
          new_transform.translate (-data[1], -data[2]);
        }
      else
        return false;
    }
  else if (name == "skewX")
    {
      if (data.size () != 1)
        return false;

      new_transform.shear (tan (data[0] * degree_to_rad), 0);
    }
  else if (name == "skewY")
    {
      new_transform.shear (0, tan (data[0] * degree_to_rad));
    }
  else
    return false;

  m_transform = new_transform * m_transform;
  return true;
}

