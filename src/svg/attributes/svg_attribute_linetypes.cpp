#include "svg_attribute_linetypes.h"
#include "path/svg_path.h"


svg_attribute_linetypes::svg_attribute_linetypes ()
{

}

svg_attribute_linetypes::~svg_attribute_linetypes ()
{

}

bool svg_attribute_linetypes::read (const char *data, bool /*from_css*/)
{
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

void svg_attribute_linetypes::create_from_path (const svg_path *path, bool dont_create_if_exists)
{
  if (dont_create_if_exists && m_is_line_segment.size () == path->total_elements ())
    return;

  m_is_line_segment.resize (path->total_elements ());

  for (size_t i = 0; i < path->total_elements (); i++)
    m_is_line_segment[i] = path->element (i)->is_line ();
}
