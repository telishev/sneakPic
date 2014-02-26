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
  svg_data_type_iri *m_element;
public:
  svg_base_attribute_marker_usage () { m_element = nullptr; }

  vector<abstract_renderer_item *> configure_markers_on_path_drawing (QPainterPath path, QTransform transform, double stroke_width) const;
  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool from_css = false) const override;
  bool is_specified () const { return (m_element != nullptr); }
  string fragment_name () const;

  virtual bool is_point_applicable (int number, const QPainterPath &path) const = 0;
};

class svg_attribute_marker_start : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_start () { }

  virtual bool is_point_applicable (int number, const QPainterPath &path) const  override;
};

class svg_attribute_marker_mid : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_mid () { }

  virtual bool is_point_applicable (int number, const QPainterPath &path) const  override;
};

class svg_attribute_marker_end : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker_end () { }

  virtual bool is_point_applicable (int number, const QPainterPath &path) const override;
};

class svg_attribute_marker : public svg_base_attribute_marker_usage
{
  SVG_ATTRIBUTE
public:
  svg_attribute_marker () {}

  virtual bool is_point_applicable (int /*number*/, const QPainterPath &/*path*/) const  override { return true; }
};

#endif // SVG_ATTRIBUTES_MARKER_USAGE_H
