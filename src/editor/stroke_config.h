#ifndef STROKE_CONFIG_H
#define STROKE_CONFIG_H

#include <QtGlobal>
class abstract_svg_item;
class items_selection;

class stroke_config
{
  double m_width;
  Qt::PenJoinStyle m_linejoin;
  double m_miterlimit;
  Qt::PenCapStyle m_linecap;
public:
  stroke_config ();
  ~stroke_config ();

  double width () const { return m_width; }
  void set_width (double val) { m_width = val; }
  Qt::PenJoinStyle linejoin () const { return m_linejoin; }
  void set_linejoin (Qt::PenJoinStyle val) { m_linejoin = val; }
  double miterlimit () const { return m_miterlimit; }
  void set_miterlimit (double val) { m_miterlimit = val; }
  Qt::PenCapStyle linecap () const { return m_linecap; }
  void set_linecap (Qt::PenCapStyle val) { m_linecap = val; }

  void create_from_item (abstract_svg_item *item);
  void apply_to_item (abstract_svg_item *item) const;

  void create_from_selection (items_selection *selection);
  void apply_to_selection (items_selection *selection) const;
};

#endif // STROKE_CONFIG_H
