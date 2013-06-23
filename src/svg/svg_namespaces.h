#ifndef SVG_NAMESPACES_H
#define SVG_NAMESPACES_H

#include <QString>

enum class svg_namespaces_t
{
  UNKNOWN,
  DC,
  CC,
  RDF,
  SVG,
  EMPTY,
};

class svg_namespaces
{
public:

  static QString uri (svg_namespaces_t type)
  {
    switch (type)
      {
      case svg_namespaces_t::DC : return dc_uri ();
      case svg_namespaces_t::CC : return cc_uri ();
      case svg_namespaces_t::RDF : return rdf_uri ();
      case svg_namespaces_t::SVG : return svg_uri ();
      case svg_namespaces_t::EMPTY: return QString ();
      default: return QString ();
      }
  }

  static QString name (svg_namespaces_t type)
  {
    switch (type)
    {
    case svg_namespaces_t::DC : return dc_name ();
    case svg_namespaces_t::CC : return cc_name ();
    case svg_namespaces_t::RDF : return rdf_name ();
    case svg_namespaces_t::SVG : return svg_name ();
    case svg_namespaces_t::EMPTY: return QString ();
    default: return QString ();
    }
  }

  static QString dc_uri () { return "http://purl.org/dc/items/1.1/"; }
  static QString cc_uri () { return "http://creativecommons.org/ns#"; }
  static QString rdf_uri () { return "http://www.w3.org/1999/02/22-rdf-syntax-ns#"; }
  static QString svg_uri () { return "http://www.w3.org/2000/svg"; }

  static QString dc_name () { return "dc"; }
  static QString cc_name () { return "cc"; }
  static QString rdf_name () { return "rdf"; }
  static QString svg_name () { return "svg"; }
};

#endif // SVG_NAMESPACES_H


