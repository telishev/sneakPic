#ifndef ITEM_PAINT_STYLE_H
#define ITEM_PAINT_STYLE_H

#include "item_paint_server.h"
#include "stroke_config.h"

class item_paint_style
{
  item_paint_server m_fill;
  item_paint_server m_stroke;
  stroke_config m_stroke_cfg;
public:
  item_paint_style ();
  ~item_paint_style ();

  item_paint_server &fill () { return m_fill; }
  const item_paint_server &fill () const { return m_fill; }
  void set_fill (item_paint_server val) { m_fill = val; }
  
  item_paint_server &stroke () { return m_stroke; }
  const item_paint_server &stroke () const { return m_stroke; }
  void set_stroke (item_paint_server val) { m_stroke = val; }

  stroke_config &stroke_cfg () { return m_stroke_cfg; }
  const stroke_config &stroke_cfg () const { return m_stroke_cfg; }
  void set_stroke_cfg (stroke_config val) { m_stroke_cfg = val; }

  void load (QSettings *settings);
  void save (QSettings *settings);
};


#endif // ITEM_PAINT_STYLE_H
