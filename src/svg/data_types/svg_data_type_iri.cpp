#include "svg/data_types/svg_data_type_iri.h"

#include "svg/items/svg_items_container.h"

#include "common/common_utils.h"
#include "common/enum_conversion.h"

#include <QBuffer>
#include <QImageReader>
#include <QFileInfo>
#include <QDir>

svg_data_type_iri::svg_data_type_iri ()
{
  m_iri_type = iri_type::document_fragment;
  m_data_type = data_type::unsupported;
}

svg_data_type_iri::~svg_data_type_iri ()
{
}

const char *enum_to_string (data_type id)
{
  switch (id)
    {
      case data_type::image_jpg: return "image/jpg";
      case data_type::image_jpeg: return "image/jpeg";
      case data_type::image_png: return "image/png";
      case data_type::unsupported: return "";
    };

  return "";
}

static inline int enum_values_count (data_type)
{
  return (int) data_type::unsupported;
}

bool svg_data_type_iri::read (const QString &data_arg)
{
  QString data = data_arg; // Maybe it's ineffective
  if (data.startsWith ("url(") && data.endsWith (")"))
    {
      data = data.mid (4);
      data.chop (1);
    }

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
      m_data_type = string_to_enum <data_type> (data_with_offset.left (next_index).toLatin1 ().data ());
      if (m_data_type == data_type::unsupported)
        return false;
      data_format format;
      if (data_with_offset[next_index] == ',')
        format = data_format::percentencoding;
      else
        {
          if (data_with_offset.startsWith ("base64,"))
            return false;
          format = data_format::base64;
        }

      next_index = data_with_offset.indexOf (',');
      data_with_offset = data_with_offset.mid (next_index + 1);
      m_iri_type = iri_type::media_data;

      switch (format)
      {
        case data_format::base64:
          raw_data = QByteArray::fromBase64 (data_with_offset.toLatin1 ());
          break;
        case data_format::percentencoding:
          raw_data = QByteArray::fromPercentEncoding (data_with_offset.toLatin1 ());
          return false;
      }
      QBuffer buffer(&raw_data);
      buffer.open (QIODevice::ReadOnly);
      DO_ON_EXIT (buffer.close ());
      switch (m_data_type)
      {
        case data_type::image_jpeg:
        case data_type::image_jpg:
          m_image_data.reset (new QImage ());
          m_image_data->load (&buffer, "jpg");
          break;
        case data_type::image_png:
          m_image_data.reset (new QImage ());
          m_image_data->load (&buffer, "png");
          break;
        case data_type::unsupported:
          return false;
      }
      if (m_image_data)
        *m_image_data = m_image_data->convertToFormat (QImage::Format_ARGB32_Premultiplied);
      return true;
    }
  else // try to treat as a link
    {
      m_iri_type = iri_type::media_resource;
      link_to_resource = data;
      return true;
    }
}

bool svg_data_type_iri::write (QString &data) const
{
  switch (m_iri_type)
    {
    case iri_type::document_fragment:
      data = QLatin1String ("#") + m_element_id;
      return true;
    case iri_type::media_data:
        data = QLatin1String ("data:") + enum_to_string (m_data_type) + ";base64," + raw_data.toBase64 (); // We're using the date we calculated previously
      return true;
    case iri_type::media_resource:
      data = link_to_resource;
      return true;
    case iri_type::unsupported:
      return false;
    }
  return false;
}

string svg_data_type_iri::get_fragment_name () const
{
  return m_element_id.toStdString ();
}

iri_type svg_data_type_iri::get_type () const
{
  return m_iri_type;
}

data_type svg_data_type_iri::get_data_type () const
{
  return m_data_type;
}

QImage *svg_data_type_iri::get_image_data (const QString &svg_name) const
{
  load_linked_image (svg_name);

  if (!m_image_data)
    return nullptr;

  return m_image_data.get ();
}

void svg_data_type_iri::create_from_image (const QImage &image)
{
  m_image_data.reset (new QImage (image));
  m_data_type = data_type::image_png;
  m_iri_type = iri_type::media_data;
  QBuffer buffer;
  buffer.open(QIODevice::WriteOnly);
  image.save (&buffer, "png");
  buffer.close ();
  raw_data = buffer.buffer ();
}

void svg_data_type_iri::load_linked_image (const QString &svg_name) const
{
  if (m_image_data || link_to_resource.isEmpty ())
    return;

  QFileInfo file (QFileInfo (svg_name).absoluteDir (), link_to_resource);
  if (!file.exists ())
    return;

  if (file.suffix () == "jpg" || file.suffix () == "jpeg")
    {
      m_image_data.reset (new QImage ());
      if (m_image_data->load (file.absoluteFilePath (), "jpeg"))
        m_data_type = data_type::image_jpeg;
      else
        {
          m_image_data.release ();
          return;
        }
    }
  else if (file.suffix () == "png")
    {
      m_image_data.reset (new QImage ());
      if (m_image_data->load (file.absoluteFilePath (), "png"))
        m_data_type = data_type::image_png;
      else
        {
          m_image_data.release ();
          return;
        }
    }

  if (m_image_data)
    *m_image_data = m_image_data->convertToFormat (QImage::Format_ARGB32_Premultiplied);
}
