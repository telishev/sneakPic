#include "svg_attribute_path_data.h"

#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/shape/svg_shape_cubic.h"
#include "svg/shape/svg_shape_elliptic_arc.h"
#include "svg/shape/svg_shape_line.h"
#include "svg/shape/svg_shape_quadratic.h"
#include "svg/shape/svg_subpath.h"

#include <functional>
#include <QString>

svg_attribute_path_data::svg_attribute_path_data (abstract_svg_item *item)
  : abstract_attribute (item)
{

}

svg_attribute_path_data::~svg_attribute_path_data ()
{
  clear ();
}

bool svg_attribute_path_data::read (const QString &data, bool /*from_css*/)
{
  return read_data (data.toUtf8 ().constData ());

}

bool svg_attribute_path_data::write (QString &/*data*/, bool /*to_css*/) const 
{
  /// TODO: support
  return true;
}

bool svg_attribute_path_data::read_data (const char *data)
{
  point_2d subpath_start_point;
  point_2d current_point;
  const char *cur = data;
  char cur_command = 0;
  char prev_command = 0;

  typedef bool (svg_attribute_path_data::*func_type) (const char *&, point_2d &, point_2d &, bool);
  func_type commands[255] = {};

  commands['M'] = &svg_attribute_path_data::read_move;
  commands['Z'] = &svg_attribute_path_data::read_end_subpath;
  commands['L'] = &svg_attribute_path_data::read_line;
  commands['H'] = &svg_attribute_path_data::read_h_line;
  commands['V'] = &svg_attribute_path_data::read_v_line;
  commands['C'] = &svg_attribute_path_data::read_curve;
  commands['S'] = &svg_attribute_path_data::read_curve_short;
  commands['Q'] = &svg_attribute_path_data::read_quadratic;
  commands['T'] = &svg_attribute_path_data::read_quadratic_short;
  commands['A'] = &svg_attribute_path_data::read_arc;

  m_subpaths.push_back (new svg_subpath);

  trim_separators_left (cur);
  while (*cur)
    {
      cur_command = *cur;

      /// if there no corresponding command, try to use previous used command
      if (commands[to_command (cur_command)] != 0)
        cur++;
      else
        cur_command = prev_command;

      /// subsequent M or m commands are transformed into 'L' and 'l'
      /// TODO: do it better
      if (cur_command == prev_command)
        {
          if (cur_command == 'M')
            cur_command = 'L';
          else if (cur_command == 'm')
            cur_command = 'l';
        }

      bool relative = !is_upper (cur_command);
      func_type cur_command_func = commands[to_command (cur_command)];
      /// if still no command, return an error
      if (cur_command_func == nullptr)
        return false;

      if (!(this->*cur_command_func) (cur, subpath_start_point, current_point, relative))
        return false;

      prev_command = cur_command;
      trim_separators_left (cur);
    }

  if (m_subpaths.back ()->empty ())
    {
      delete m_subpaths.back ();
      m_subpaths.pop_back ();
    }
  return true;
}

bool svg_attribute_path_data::is_upper (char v) const
{
  return QChar (v).isUpper ();
}

unsigned char svg_attribute_path_data::to_command (char v) const
{
  return (unsigned char)QChar (v).toUpper ().toLatin1 ();
}

void svg_attribute_path_data::clear ()
{
  for (svg_subpath *subpath : m_subpaths)
    delete subpath;

  m_subpaths.clear ();
}

bool svg_attribute_path_data::read_move (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative)
{
  double point_x = 0, point_y = 0;
  CHECK (str_to_double (data, point_x));
  CHECK (str_to_double (data, point_y));

  point_2d new_current_point (point_x, point_y);
  if (relative)
    new_current_point += current_point;

  current_point = subpath_start_point = new_current_point;

  /// create new path if previous is not empty
  if (!m_subpaths.back ()->empty ())
    m_subpaths.push_back (new svg_subpath);
  return true;
}

bool svg_attribute_path_data::read_end_subpath (const char *&/*data*/, point_2d &subpath_start_point, point_2d &current_point, bool /*relative*/)
{
  current_point = subpath_start_point;

  /// create new path if previous is not empty
  if (!m_subpaths.back ()->empty ())
    m_subpaths.push_back (new svg_subpath);
  else
    return false;

  return true;
}

bool svg_attribute_path_data::read_line (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double point_x = 0, point_y = 0;
  CHECK (str_to_double (data, point_x));
  CHECK (str_to_double (data, point_y));

  point_2d new_current_point (point_x, point_y);
  if (relative)
    new_current_point += current_point;

  svg_shape_line *new_line = new svg_shape_line (current_point, new_current_point);
  m_subpaths.back ()->add_shape (new_line);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_h_line (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double point_y = 0;
  CHECK (str_to_double (data, point_y));

  point_2d new_current_point (current_point.x (), relative ? current_point.y () + point_y : point_y);

  svg_shape_line *new_line = new svg_shape_line (current_point, new_current_point);
  m_subpaths.back ()->add_shape (new_line);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_v_line (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double point_x = 0;
  CHECK (str_to_double (data, point_x));

  point_2d new_current_point (relative ? current_point.x () + point_x : point_x, current_point.y ());

  svg_shape_line *new_line = new svg_shape_line (current_point, new_current_point);
  m_subpaths.back ()->add_shape (new_line);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_curve (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double x1, y1, x2, y2, x, y;
  CHECK (str_to_double (data, x1));
  CHECK (str_to_double (data, y1));
  CHECK (str_to_double (data, x2));
  CHECK (str_to_double (data, y2));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  point_2d new_current_point (x, y);
  point_2d control1 (x1, y1);
  point_2d control2 (x2, y2);

  if (relative)
    {
      new_current_point += current_point;
      control1 += current_point;
      control2 += current_point;
    }

  svg_shape_cubic *new_shape = new svg_shape_cubic (current_point, new_current_point, control1, control2);
  m_subpaths.back ()->add_shape (new_shape);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_curve_short (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double x2, y2, x, y;
  CHECK (str_to_double (data, x2));
  CHECK (str_to_double (data, y2));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  point_2d new_current_point (x, y);
  point_2d control1;
  point_2d control2 (x2, y2);

  if (relative)
    {
      new_current_point += current_point;
      control2 += current_point;
    }

  abstract_svg_shape *prev_shape = 0;
  if (m_subpaths.size ())
    prev_shape = m_subpaths.back ()->back ();
  if (prev_shape)
    {
      /// TODO: do it without dyncast
      svg_shape_cubic *prev_cubic_shape = dynamic_cast<svg_shape_cubic *> (prev_shape);
      if (prev_cubic_shape)
        control1 = (current_point + (current_point - prev_cubic_shape->control2 ()));
    }

  svg_shape_cubic *new_shape = new svg_shape_cubic (current_point, new_current_point, control1, control2);
  m_subpaths.back ()->add_shape (new_shape);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_quadratic (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative)
{
  FIX_UNUSED (data, subpath_start_point, current_point, relative);
  /// TODO: support it
  return false;
}

bool svg_attribute_path_data::read_quadratic_short (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative)
{
  FIX_UNUSED (data, subpath_start_point, current_point, relative);
  /// TODO: support it
  return false;
}

bool svg_attribute_path_data::read_arc (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative)
{
  FIX_UNUSED (data, subpath_start_point, current_point, relative);
  /// TODO: support it
  return false;
}



