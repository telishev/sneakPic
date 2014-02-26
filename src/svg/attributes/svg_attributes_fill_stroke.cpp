#include "svg/attributes/svg_attributes_fill_stroke.h"

#include "svg/attributes/svg_attribute_color.h"

#include "common/common_utils.h"
#include "common/string_utils.h"

#include "renderer/renderer_paint_server.h"

#include "svg/items/svg_base_items_gradient.h"
#include "svg/items/svg_items_container.h"

#include <memory>



svg_paint_server::svg_paint_server ()
{
  m_server_type = paint_server_type::NONE;
}

svg_paint_server::~svg_paint_server ()
{

}

void svg_paint_server::set_to_color (QColor color)
{
  m_server_type = paint_server_type::COLOR;
  m_color = color;
}

bool svg_paint_server::read (const char *char_data, bool /*from_css*/)
{
  QString data (char_data);
  if (data == "none")
    m_server_type = paint_server_type::NONE;
  else if (data.startsWith ("url("))
    {
      int begin = data.indexOf ('(');
      int end = data.indexOf (')');
      if (end == -1)
        return false;

      QString url = data.mid (begin + 1, end - begin - 1);
      if (!m_iri.read (url))
        return false;
      if (m_iri.get_type () != iri_type::document_fragment)
        return false;

      m_server_type = paint_server_type::IRI;
    }
  else if (data == "currentColor")
    {
      m_server_type = paint_server_type::CURRENT_COLOR;
    }
  else
    {
      m_server_type = paint_server_type::COLOR;
      m_color = string_to_color (char_data);
      if (!m_color.isValid ())
        return false;
    }
  return true;
}

bool svg_paint_server::write (QString &data, bool /*to_css*/) const
{
  switch (m_server_type)
    {
    case paint_server_type::NONE:
      data = "none";
      return true;
    case paint_server_type::COLOR:
      data = color_to_string (m_color);
      return true;
    case paint_server_type::CURRENT_COLOR:
      data =  "currentColor";
      return true;
    case paint_server_type::IRI:
      {
        QString url;
        CHECK (m_iri.write (url));
        data = QString ("url(%1)").arg (url);
        return true;
      }
    }

  return false;
}

renderer_paint_server *svg_paint_server::create_paint_server (const svg_items_container *container) const
{
  switch (m_server_type)
    {
    case paint_server_type::NONE:
      return new renderer_painter_server_none;
    case paint_server_type::COLOR:
      return new renderer_painter_server_color (m_color);
    case paint_server_type::CURRENT_COLOR:
      return new renderer_painter_server_color (get_item ()->get_computed_attribute <svg_attribute_color> ()->value ());
    case paint_server_type::IRI:
      {
        /// TODO: support other paint servers
        string fragment_name = m_iri.get_fragment_name ();

        const svg_base_items_gradient *base_gradient = dynamic_cast<const svg_base_items_gradient *> (container->get_item (fragment_name));
        if (!base_gradient)
          return new renderer_painter_server_none;

        return base_gradient->create_paint_server ();
      }
    }

  return new renderer_painter_server_none;
}

bool svg_paint_server::need_to_render (const svg_items_container *container) const
{
  unique_ptr<renderer_paint_server> server (create_paint_server (container));
  return server->need_to_render ();
}
