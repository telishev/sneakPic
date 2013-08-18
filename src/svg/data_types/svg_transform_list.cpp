#include "svg/data_types/svg_transform_list.h"

#include <QStringList>

#include "common/string_utils.h"
#include "common/common_utils.h"

bool svg_transform_list::write (QString &data) const 
{
  QString m11 = double_to_str (m_transform.m11 ()), m21 = double_to_str (m_transform.m21 ()), m31 = double_to_str (m_transform.m31 ());
  QString m12 = double_to_str (m_transform.m12 ()), m22 = double_to_str (m_transform.m22 ()), m32 = double_to_str (m_transform.m32 ());
  data = QString ("matrix(%1,%2,%3,%4,%5,%6)").arg (m11, m12, m21, m22, m31, m32);
  return true;
}

bool svg_transform_list::read (const char *data)
{
  trim_whitespace_left (data);
  std::vector<double> transform_data;
  m_transform = QTransform ();
  while (*data)
    {
      transform_data.clear ();
      std::string transform, transform_data_string;
      CHECK (extract_chunk ('(', data, transform));
      CHECK (extract_chunk (')', data, transform_data_string));
      CHECK (get_transfrom_data (transform_data_string.c_str (), transform_data));
      CHECK (apply_transform (transform, transform_data));
    }

  return true;
}

bool svg_transform_list::get_transfrom_data (const char *data, std::vector<double> &dest) const
{
  trim_separators_left (data);
  while (*data)
    {
      double new_double = 0.0;
      CHECK (str_to_double (data, new_double));
      dest.push_back (new_double);
    }

  return true;
}

bool svg_transform_list::apply_transform (const std::string &name, std::vector<double> &data)
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
      ty = data.size () == 1 ? 0 : data[1];

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

