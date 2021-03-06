#include "svg_attribute_linetypes.h"
#include "path/svg_path_geom.h"


svg_attribute_linetypes::svg_attribute_linetypes ()
{

}

svg_attribute_linetypes::~svg_attribute_linetypes ()
{

}

bool svg_attribute_linetypes::read (const char *data, bool /*from_css*/)
{
  m_is_line_segment.clear ();
  for (const char *cur_data = data; *cur_data; cur_data++)
    {
      bool is_line = false;
      if (*cur_data == 'l')
        is_line = true;

      m_is_line_segment.push_back (is_line);
    }

  return true;
}

bool svg_attribute_linetypes::write (QString &data, bool /*to_css*/) const
{
  for (size_t i = 0; i < m_is_line_segment.size (); i++)
    data += m_is_line_segment[i] ? 'l' : 'c';

  return true;
}

void svg_attribute_linetypes::create_from_path (const svg_path_geom *path, bool dont_create_if_exists)
{
  if (dont_create_if_exists && m_is_line_segment.size () == path->total_segments ())
    return;

  m_is_line_segment.resize (path->total_segments ());

  for (auto it = path->begin (); it != path->end (); ++it)
    {
      int segment_index = it.segment_index (cp_type::RIGHT);
      if (segment_index < 0)
        continue;

      m_is_line_segment[segment_index] = it.segment (cp_type::RIGHT).is_line ();
    }
}
