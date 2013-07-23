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
  XLINK,
  EMPTY,
  XML,
};

class svg_namespaces
{
public:

  static const char *uri (svg_namespaces_t type)
  {
    switch (type)
      {
      case svg_namespaces_t::DC : return dc_uri ();
      case svg_namespaces_t::CC : return cc_uri ();
      case svg_namespaces_t::RDF : return rdf_uri ();
      case svg_namespaces_t::SVG : return svg_uri ();
      case svg_namespaces_t::XLINK: return xlink_uri ();
      case svg_namespaces_t::XML: return xml_uri ();
      case svg_namespaces_t::EMPTY: return "";
      default: return "";
      }
  }

  static const char *name (svg_namespaces_t type)
  {
    switch (type)
    {
    case svg_namespaces_t::DC : return dc_name ();
    case svg_namespaces_t::CC : return cc_name ();
    case svg_namespaces_t::RDF : return rdf_name ();
    case svg_namespaces_t::SVG : return svg_name ();
    case svg_namespaces_t::XLINK: return xlink_name ();
    case svg_namespaces_t::XML: return xml_name ();
    case svg_namespaces_t::EMPTY: return "";
    default: return "";
    }
  }

  static const char *dc_uri () { return "http://purl.org/dc/items/1.1/"; }
  static const char *cc_uri () { return "http://creativecommons.org/ns#"; }
  static const char *rdf_uri () { return "http://www.w3.org/1999/02/22-rdf-syntax-ns#"; }
  static const char *svg_uri () { return "http://www.w3.org/2000/svg"; }
  static const char *xlink_uri () { return "http://www.w3.org/1999/xlink"; }
  static const char *xml_uri () { return "http://www.w3.org/XML/1998/namespace"; }

  static const char *dc_name () { return "dc"; }
  static const char *cc_name () { return "cc"; }
  static const char *rdf_name () { return "rdf"; }
  static const char *svg_name () { return "svg"; }
  static const char *xlink_name () { return "xlink"; }
  static const char *xml_name () { return "xml"; }
};

#endif // SVG_NAMESPACES_H


