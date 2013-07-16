#include "svg_attribute_path_data.h"

#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/point_2d.h"

#include <functional>
#include <QString>
#include <QPainterPath>
#include "svg/svg_arc_data.h"



svg_attribute_path_data::svg_attribute_path_data (abstract_svg_item *item)
  : abstract_attribute (item)
{

}

svg_attribute_path_data::~svg_attribute_path_data ()
{
}

bool svg_attribute_path_data::read (const QString &data, bool /*from_css*/)
{
  return read_data (data.toUtf8 ().constData ());

}

bool svg_attribute_path_data::write (QString &data, bool /*to_css*/) const 
{
  size_t data_offset = 0;
  for (size_t i = 0; i < m_commands.size (); i++)
    {
      data += command_svg_name (m_commands[i]);
      int data_count = command_data_count (m_commands[i]);
      for (int j = 0; j < data_count; j++)
        data += double_to_str (m_path_data[data_offset++]) + " ";
    }
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

bool svg_attribute_path_data::read_move (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative)
{
  double point_x = 0, point_y = 0;
  CHECK (str_to_double (data, point_x));
  CHECK (str_to_double (data, point_y));

  point_2d new_current_point (point_x, point_y);
  if (relative)
    new_current_point += current_point;

  current_point = subpath_start_point = new_current_point;

  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::MOVE);
  return true;
}

bool svg_attribute_path_data::read_end_subpath (const char *&/*data*/, point_2d &subpath_start_point, point_2d &current_point, bool /*relative*/)
{
  current_point = subpath_start_point;
  m_commands.push_back (path_command::CLOSE_PATH);

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

  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::LINE);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_h_line (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double point_x = 0;
  CHECK (str_to_double (data, point_x));

  point_2d new_current_point (relative ? current_point.x () + point_x : point_x, current_point.y ());

  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::LINE);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_v_line (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double point_y = 0;
  CHECK (str_to_double (data, point_y));

  point_2d new_current_point (current_point.x (), relative ? current_point.y () + point_y : point_y);

  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::LINE);

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
  m_path_data.push_back (control1.x ());
  m_path_data.push_back (control1.y ());
  m_path_data.push_back (control2.x ());
  m_path_data.push_back (control2.y ());
  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::CURVE);

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

  if (m_commands.size () < 6 || m_commands.back () != path_command::CURVE)
    control1 = new_current_point;
  else
    {
      point_2d prev_control_point = point_2d (m_path_data[m_path_data.size () - 4], m_path_data[m_path_data.size () - 3]);
      control1 = (current_point + (current_point - prev_control_point));
    }

  m_path_data.push_back (control1.x ());
  m_path_data.push_back (control1.y ());
  m_path_data.push_back (control2.x ());
  m_path_data.push_back (control2.y ());
  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::CURVE);


  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_quadratic (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double x1, y1, x, y;
  CHECK (str_to_double (data, x1));
  CHECK (str_to_double (data, y1));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  point_2d new_current_point (x, y);
  point_2d control1 (x1, y1);

  if (relative)
    {
      new_current_point += current_point;
      control1 += current_point;
    }
  m_path_data.push_back (control1.x ());
  m_path_data.push_back (control1.y ());
  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::QUAD);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_quadratic_short (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double x, y;
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  point_2d new_current_point (x, y);
  point_2d control1;

  if (relative)
    {
      new_current_point += current_point;
    }

  if (m_commands.size () < 4 || m_commands.back () != path_command::QUAD)
    control1 = new_current_point;
  else
    {
      point_2d prev_control_point = point_2d (m_path_data[m_path_data.size () - 4], m_path_data[m_path_data.size () - 3]);
      control1 = (current_point + (current_point - prev_control_point));
    }

  m_path_data.push_back (control1.x ());
  m_path_data.push_back (control1.y ());
  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::CURVE);

  current_point = new_current_point;
  return true;
}

bool svg_attribute_path_data::read_arc (const char *&data, point_2d &/*subpath_start_point*/, point_2d &current_point, bool relative)
{
  double rx, ry, xrot, large_arc, sweep_flag, x, y;
  CHECK (str_to_double (data, rx));
  CHECK (str_to_double (data, ry));
  CHECK (str_to_double (data, xrot));
  CHECK (str_to_double (data, large_arc));
  CHECK (str_to_double (data, sweep_flag));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  point_2d new_current_point (x, y);

  if (relative)
    {
      new_current_point += current_point;
    }
  m_path_data.push_back (rx);
  m_path_data.push_back (ry);
  m_path_data.push_back (xrot);
  m_path_data.push_back (large_arc);
  m_path_data.push_back (sweep_flag);
  m_path_data.push_back (new_current_point.x ());
  m_path_data.push_back (new_current_point.y ());
  m_commands.push_back (path_command::ARC);

  current_point = new_current_point;
  return true;
}

QPainterPath svg_attribute_path_data::create_painter_path () const
{
  QPainterPath path;
  size_t data_offset = 0;
  for (size_t i = 0; i < m_commands.size (); i++)
    {
      switch (m_commands[i])
        {
        case path_command::MOVE:
          path.moveTo (m_path_data[data_offset], m_path_data[data_offset + 1]);
          data_offset += 2;
          break;
        case path_command::CLOSE_PATH:
          path.closeSubpath ();
          break;
        case path_command::LINE:
          path.lineTo (m_path_data[data_offset], m_path_data[data_offset + 1]);
          data_offset += 2;
          break;
        case path_command::CURVE:
          path.cubicTo (m_path_data[data_offset + 0], m_path_data[data_offset + 1],
                        m_path_data[data_offset + 2], m_path_data[data_offset + 3],
                        m_path_data[data_offset + 4], m_path_data[data_offset + 5]);
          data_offset += 6;
          break;
        case path_command::QUAD:
          path.quadTo (m_path_data[data_offset + 0], m_path_data[data_offset + 1],
                       m_path_data[data_offset + 2], m_path_data[data_offset + 3]);
          data_offset += 4;
          break;
        case path_command::ARC:
          arc_converter::pathArc (path, m_path_data[data_offset + 0], m_path_data[data_offset + 1],
                                  m_path_data[data_offset + 2], m_path_data[data_offset + 3],
                                  m_path_data[data_offset + 4], m_path_data[data_offset + 5], m_path_data[data_offset + 6],
                                  path.currentPosition ().x (), path.currentPosition ().y ());
          data_offset += 7;
          break;
        }
    }

  return path;
}

int svg_attribute_path_data::command_data_count (path_command command) const
{
  switch (command)
    {
    case path_command::MOVE: return 2;
    case path_command::CLOSE_PATH:  return 0;
    case path_command::LINE: return 2;
    case path_command::CURVE: return 6;
    case path_command::QUAD: return 4;
    case path_command::ARC: return 7;
    }

  return 0;
}

const char *svg_attribute_path_data::command_svg_name (path_command command) const
{
  switch (command)
    {
    case path_command::MOVE: return "M";
    case path_command::CLOSE_PATH:  return "Z";
    case path_command::LINE: return "L";
    case path_command::CURVE: return "C";
    case path_command::QUAD: return "Q";
    case path_command::ARC: return "A";
    }

  return "";
}



