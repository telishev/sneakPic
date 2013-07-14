#include "svg/data_types/data_type_iri.h"

#include "svg/svg_document.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"

#include "common/enum_conversion.h"

#include <QBuffer>
#include <QImageReader>

data_type_iri::data_type_iri (abstract_svg_item *item)
{
  m_item = item;
  m_image_data = 0; 
  m_iri_type = iri_type::document_fragment;
  m_data_type = data_type_t::unsupported;
}

data_type_iri::~data_type_iri ()
{
  FREE (m_image_data);
}

const char *enum_to_string (data_type_t id)
{
  switch (id)
    {
      case data_type_t::image_jpeg: return "image/jpeg";
      case data_type_t::image_png: return "image/png";
      case data_type_t::unsupported: return "";
    };

  return "";
}

static inline int enum_values_count (data_type_t)
{
  return (int) data_type_t::unsupported;
}

bool data_type_iri::read (const QString &data)
{
  /// For now we only support local references in a form  #<elementID>
  if (data.startsWith ("#"))
    {
      m_iri_type = iri_type::document_fragment;
      m_element_id = data.mid (1);
      return true;
    }
  else if (data.startsWith ("data:"))
    {
      QString data_with_offset = data.mid (5);

      int next_index = data_with_offset.indexOf (QRegExp ("[;,]"));
      m_data_type = string_to_enum <data_type_t> (data_with_offset.left (next_index).toLatin1 ().data ());
      if (m_data_type == data_type_t::unsupported)
        return false;
      data_format_t data_format;
      if (data_with_offset[next_index] == ',')
        data_format = data_format_t::percentencoding;
      else
        {
          if (data_with_offset.startsWith ("base64,"))
            return false;
          data_format = data_format_t::base64;
        }

      next_index = data_with_offset.indexOf (',');
      data_with_offset = data_with_offset.mid (next_index + 1);
      m_iri_type = iri_type::media_data;


      QByteArray raw_data;
      switch (data_format)
      {
        case data_format_t::base64:
          raw_data = QByteArray::fromBase64 (data_with_offset.toLatin1 ());
          break;
        case data_format_t::percentencoding:
          raw_data = QByteArray::fromPercentEncoding (data_with_offset.toLatin1 ());
          return false;
      }
      switch (m_data_type)
      {
        case data_type_t::image_jpeg:
          m_image_data = new QImage ();
          m_image_data->load (&QBuffer (&raw_data), "jpg");
          break;
        case data_type_t::image_png:
          m_image_data = new QImage ();
          m_image_data->load (&QBuffer (&raw_data), "png");
          break;
        case data_type_t::unsupported:
          return false;
      }
      return true;
    }
  // Everything else is unsupported
  return false;
}

bool data_type_iri::write (QString &data) const 
{
  data = QLatin1String ("#") + m_element_id;
  return true;
}

abstract_svg_item *data_type_iri::get_fragment () const
{
  if (!m_item)
    return nullptr;

  return m_item->document ()->item_container ()->get_item (m_element_id);
}

iri_type data_type_iri::get_type () const
{
  return m_iri_type;
}

data_type_t data_type_iri::get_data_type () const
{
  return m_data_type;
}

QImage *data_type_iri::get_image_data () const
{
  if (!m_image_data)
    return nullptr;

  return m_image_data;
}
