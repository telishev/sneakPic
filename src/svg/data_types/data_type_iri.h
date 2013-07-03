#ifndef DATA_TYPE_IRI_H
#define DATA_TYPE_IRI_H

#include <QString>

class abstract_svg_item;

class data_type_iri
{
  abstract_svg_item *m_item;
  QString m_element_id;
public:
  data_type_iri (abstract_svg_item *item);
  virtual ~data_type_iri ();

  bool read (const QString &data);
  bool write (QString &data) const;

  abstract_svg_item *href () const;
};

#endif // DATA_TYPE_IRI_H
