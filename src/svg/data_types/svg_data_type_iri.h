#ifndef DATA_TYPE_IRI_H
#define DATA_TYPE_IRI_H

#include <QImage>
#include <QString>

class abstract_svg_item;

enum class iri_type
{
  document_fragment,
  // external_frament,  // Not supported
  // external_document, // Not supported
  media_resource,       // Only for raster image
  media_data,           // Only for raster image
  unsupported,
};

enum class data_type
{
  image_jpeg,
  image_jpg,
  image_png,
  unsupported
};

enum class data_format
{
  base64,
  percentencoding
};

class svg_data_type_iri
{
  abstract_svg_item *m_item;
  QString m_element_id;
  QImage *m_image_data;
  data_type m_data_type;
  iri_type m_iri_type;
  QByteArray raw_data;
  QString link_to_resource;
public:
  svg_data_type_iri (abstract_svg_item *item);
  virtual ~svg_data_type_iri ();

  bool read (const QString &data_arg);
  bool write (QString &data) const;

  iri_type get_type () const; // Type should be checked before calling further getters
  data_type get_data_type () const;

  bool has_image_data () const;
  abstract_svg_item *get_fragment () const;
  QImage *get_image_data () const;
};

#endif // DATA_TYPE_IRI_H
