#ifndef ITEM_PAINT_SERVER_H
#define ITEM_PAINT_SERVER_H

#include <QColor>
#include <QGradient>

class abstract_svg_item;
class items_selection;

enum class item_paint_server_type
{
  NONE,
  COLOR,
  GRADIENT,
};

class item_paint_server
{
  QColor m_color;
  QGradient m_gradient;
  item_paint_server_type m_type;
public:
  item_paint_server ();
  ~item_paint_server ();

  void set_color (QColor color);
  QColor color () const;
  item_paint_server_type type () const { return m_type; }

  void create_from_item (abstract_svg_item *item, bool is_fill);
  void apply_to_item (abstract_svg_item *item, bool is_fill) const;

  void create_from_selection (items_selection *selection, bool is_fill);
  void apply_to_selection (items_selection *selection, bool is_fill) const;

};

#endif // ITEM_PAINT_SERVER_H
