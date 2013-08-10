#ifndef RUBBERBAND_SELECTION_H
#define RUBBERBAND_SELECTION_H

#include "renderer/overlay_items_container.h"

class QRectF;

class rubberband_selection : public overlay_items_container
{
  double m_start_x, m_start_y;
  double m_end_x, m_end_y;
public:
  rubberband_selection (overlay_renderer *overlay);
  ~rubberband_selection ();
  
  void start_selection (double pos_x, double pos_y);
  void move_selection (double pos_x, double pos_y);
  void end_selection ();

  QRectF selection_rect () const;

protected:
  virtual std::vector<abstract_renderer_item *> create_overlay_item (const std::string &object) const override;
};

#endif // RUBBERBAND_SELECTION_H
