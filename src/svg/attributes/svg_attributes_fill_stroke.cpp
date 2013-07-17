#include "svg/attributes/svg_attributes_fill_stroke.h"

#include "common/common_utils.h"
#include "common/string_utils.h"

#include "renderer/renderer_paint_server.h"

#include "svg/items/svg_base_items_gradient.h"

#include <memory>



svg_paint_server::svg_paint_server (abstract_svg_item *item)
  : abstract_attribute (item), m_iri (item)
{
  m_server_type = paint_server_type::NONE;
}

svg_paint_server::~svg_paint_server ()
{

}

bool svg_paint_server::read (const QString &data, bool /*from_css*/)
{
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
  else
    {
      m_color = QColor (data);
      m_server_type = paint_server_type::COLOR;
    }

  return true;
}

bool svg_paint_server::write (QString &data, bool /*to_css*/) const 
{
  switch (m_server_type)
    {
    case paint_server_type::NONE: data = "none"; return true;
    case paint_server_type::COLOR:
      data = color_to_string (m_color);
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

renderer_paint_server *svg_paint_server::create_paint_server () const
{
  switch (m_server_type)
    {
    case paint_server_type::NONE: return new renderer_painter_server_none;
    case paint_server_type::COLOR:
      return new renderer_painter_server_color (m_color);
    case paint_server_type::IRI:
      {
        /// TODO: support other paint servers
        const svg_base_items_gradient *base_gradient = dynamic_cast<const svg_base_items_gradient *> (m_iri.get_fragment ());
        if (!base_gradient)
          return new renderer_painter_server_none;

        return base_gradient->create_paint_server ();
      }
    }

  return new renderer_painter_server_none;
}

bool svg_paint_server::need_to_render () const
{
  std::unique_ptr<renderer_paint_server> server (create_paint_server ());
  return server->need_to_render ();
}
