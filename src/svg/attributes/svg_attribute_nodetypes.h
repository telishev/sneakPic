#ifndef SVG_ATTRIBUTE_NODETYPES_H
#define SVG_ATTRIBUTE_NODETYPES_H

#include "svg/attributes/abstract_attribute.h"

class svg_path;

enum class node_type_t : char
{
  CUSP,
  SMOOTH,
  SYMMETRIC,

  COUNT,
};

class svg_attribute_nodetypes : public abstract_attribute
{
  SVG_ATTRIBUTE

  std::vector<node_type_t> m_node_type;
public:
  svg_attribute_nodetypes ();
  virtual ~svg_attribute_nodetypes ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  node_type_t node_type (int node) const { return m_node_type[node]; }

  void set_node_type (int node, node_type_t type) { m_node_type[node] = type; }

  void create_from_path (const svg_path *path, bool dont_create_if_exists = false);

private:
  char node_type_to_char (node_type_t type) const;
  node_type_t char_to_node_type (char val) const;
};

#endif // SVG_ATTRIBUTE_NODETYPES_H
