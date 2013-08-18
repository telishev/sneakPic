#ifndef SVG_ATTRIBUTES_MARKER_USAGE_H
#define SVG_ATTRIBUTES_MARKER_USAGE_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_iri.h"

class SkCanvas;
class QPainterPath;

class abstract_renderer_item;
class renderer_config;
class renderer_state;

class svg_base_attribute_marker_usage : public abstract_attribute
{
protected:
  svg_data_type_iri *m_element;
  bool m_start;
  bool m_mid;
  bool m_end;
public:
  svg_base_attribute_marker_usage (svg_document *document) : abstract_attribute (document) { m_element = nullptr; m_start = false; m_mid = false; m_end = false; }

  std::vector<abstract_renderer_item *> configure_markers_on_path_drawing (QPainterPath path, QTransform transform, double stroke_width) const;
  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool from_css = false) const override;
  bool is_specified () const { return (m_element != nullptr); }
};

class svg_attribute_marker_start : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_start (svg_document *document) : svg_base_attribute_marker_usage (document) { m_start = true;  }
};

class svg_attribute_marker_mid : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_mid (svg_document *document) : svg_base_attribute_marker_usage (document) { m_mid = true; }
};

class svg_attribute_marker_end : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_end (svg_document *document) : svg_base_attribute_marker_usage (document) { m_end = true; }
};

class svg_attribute_marker : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker (svg_document *document) : svg_base_attribute_marker_usage (document) { m_start = true; m_mid = true; m_end = true; }
};

#endif // SVG_ATTRIBUTES_MARKER_USAGE_H
