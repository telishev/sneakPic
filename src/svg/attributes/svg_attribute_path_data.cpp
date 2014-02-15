#include "svg_attribute_path_data.h"

#include "common/string_utils.h"
#include "common/common_utils.h"
#include "common/memory_deallocation.h"

#include "svg/point_2d.h"

#include <functional>
#include <QString>
#include <QPainterPath>
#include "path/svg_path_geom.h"

#include "svg/svg_arc_data.h"
#include "path/path_builder.h"
#include "path/svg_path.h"
#include "svg/attributes/svg_attribute_nodetypes.h"



svg_attribute_path_data::svg_attribute_path_data ()
{
  m_path = new svg_path_geom;
}

svg_attribute_path_data::~svg_attribute_path_data ()
{
  FREE (m_path);
}

bool svg_attribute_path_data::write (QString &data, bool /*to_css*/) const 
{
  for (const auto &subpath : m_path->subpath ())
    {
      auto elements = subpath.elements ();
      data += QString ("M %1 ").arg (point_to_str (elements.front ().point));
      for (size_t i = 0; i < elements.size (); i++)
        {
          size_t next_element = i + 1;
          if (next_element == elements.size ())
            {
              if (!subpath.is_closed ())
                continue;
              else
                next_element = 0;
            }

          data += QString ("C %1 %2 %3 ").arg (point_to_str (elements[i].c2), point_to_str (elements[next_element].c1),
                                              point_to_str (elements[next_element].point));
        }

      if (subpath.is_closed ())
       data += "Z";
    }
  return true;
}

bool svg_attribute_path_data::read (const char *data, bool /*from_css*/)
{
  const char *cur = data;
  char cur_command = 0;
  char prev_command = 0;

  typedef bool (svg_attribute_path_data::*func_type) (const char *&, path_builder &, bool);
  func_type commands[255] = {};
  m_path->clear ();
  vector<bool> is_line_segment;
  vector<node_type_t> node_type;
  svg_path path (m_path, &is_line_segment, &node_type, QTransform ());
  path_builder builder (path);

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

      if (!(this->*cur_command_func) (cur, builder, relative))
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

bool svg_attribute_path_data::read_move (const char *&data, path_builder &builder, bool relative)
{
  double point_x = 0, point_y = 0;
  CHECK (str_to_double (data, point_x));
  CHECK (str_to_double (data, point_y));

  builder.move_to (QPointF (point_x, point_y), relative);
  return true;
}

bool svg_attribute_path_data::read_end_subpath (const char *&/*data*/, path_builder &builder, bool /*relative*/)
{
  builder.close_subpath ();
  return true;
}

bool svg_attribute_path_data::read_line (const char *&data, path_builder &builder, bool relative)
{
  double point_x = 0, point_y = 0;
  CHECK (str_to_double (data, point_x));
  CHECK (str_to_double (data, point_y));

  builder.line_to (QPointF (point_x, point_y), relative);
  return true;
}

bool svg_attribute_path_data::read_h_line (const char *&data, path_builder &builder, bool relative)
{
  double point_x = 0;
  CHECK (str_to_double (data, point_x));

  builder.hline_to (point_x, relative);
  return true;
}

bool svg_attribute_path_data::read_v_line (const char *&data, path_builder &builder, bool relative)
{
  double point_y = 0;
  CHECK (str_to_double (data, point_y));

  builder.vline_to (point_y, relative);
  return true;
}

bool svg_attribute_path_data::read_curve (const char *&data, path_builder &builder, bool relative)
{
  double x1, y1, x2, y2, x, y;
  CHECK (str_to_double (data, x1));
  CHECK (str_to_double (data, y1));
  CHECK (str_to_double (data, x2));
  CHECK (str_to_double (data, y2));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  QPointF new_current_point (x, y);
  QPointF control1 (x1, y1);
  QPointF control2 (x2, y2);

  builder.curve_to (new_current_point, control1, control2, relative);
  return true;
}

bool svg_attribute_path_data::read_curve_short (const char *&data, path_builder &builder, bool relative)
{
  double x2, y2, x, y;
  CHECK (str_to_double (data, x2));
  CHECK (str_to_double (data, y2));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  QPointF new_current_point (x, y);
  QPointF control2 (x2, y2);

  builder.curve_to_short (new_current_point, control2, relative);
  return true;
}

bool svg_attribute_path_data::read_quadratic (const char *&data, path_builder &builder, bool relative)
{
  double x1, y1, x, y;
  CHECK (str_to_double (data, x1));
  CHECK (str_to_double (data, y1));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  QPointF new_current_point (x, y);
  QPointF control1 (x1, y1);

  builder.quad_to (new_current_point, control1, relative);
  return true;
}

bool svg_attribute_path_data::read_quadratic_short (const char *&data, path_builder &builder, bool relative)
{
  double x, y;
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  QPointF new_current_point (x, y);

  builder.quad_to_short (new_current_point, relative);
  return true;
}

bool svg_attribute_path_data::read_arc (const char *&data, path_builder &builder, bool relative)
{
  double rx, ry, xrot, large_arc, sweep_flag, x, y;
  CHECK (str_to_double (data, rx));
  CHECK (str_to_double (data, ry));
  CHECK (str_to_double (data, xrot));
  CHECK (str_to_double (data, large_arc));
  CHECK (str_to_double (data, sweep_flag));
  CHECK (str_to_double (data, x));
  CHECK (str_to_double (data, y));

  QPointF new_current_point (x, y);

  builder.arc_to (new_current_point, rx, ry, xrot, (int)large_arc, (int)sweep_flag, relative);
  return true;
}

QPainterPath svg_attribute_path_data::create_painter_path () const
{
  QPainterPath path;
  for (const auto &subpath : m_path->subpath ())
    {
      auto elements = subpath.elements ();
      path.moveTo (elements.front ().point);
      for (size_t i = 0; i < elements.size (); i++)
        {
          size_t next_element = i + 1;
          if (next_element == elements.size ())
            {
              if (!subpath.is_closed ())
                continue;
              else
                next_element = 0;
            }

          path.cubicTo (elements[i].c2, elements[next_element].c1, elements[next_element].point);
        }

      if (subpath.is_closed ())
        path.closeSubpath ();
    }
  return path;
}



