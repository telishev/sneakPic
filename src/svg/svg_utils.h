#ifndef SVG_UTILS_H
#define SVG_UTILS_H

class svg_document;

class svg_utils
{
public:
  static bool get_doc_dimensions (svg_document *doc, double &width, double &height);
};


#endif // SVG_UTILS_H
