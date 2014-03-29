#ifndef DATA_TYPE_IRI_H
#define DATA_TYPE_IRI_H

#include <QImage>
#include <QString>

class svg_document;

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
  QString m_element_id;
  mutable unique_ptr<QImage> m_image_data;
  mutable data_type m_data_type;
  iri_type m_iri_type;
  QByteArray raw_data;
  QString link_to_resource;

public:
  svg_data_type_iri ();
  virtual ~svg_data_type_iri ();

  bool read (const QString &data_arg);
  bool write (QString &data) const;

  iri_type get_type () const; // Type should be checked before calling further getters
  data_type get_data_type () const;

  string get_fragment_name () const;
  QImage *get_image_data (const QString &svg_name) const;
  void create_from_image (const QImage &image);
  void create_from_element (QString name);

private:
  void load_linked_image (const QString &svg_name) const;
};

#endif // DATA_TYPE_IRI_H
