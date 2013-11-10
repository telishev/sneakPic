#ifndef COLOR_INDICATOR_H
#define COLOR_INDICATOR_H
#include "gui/color_selectors/color_selector.h"

class color_indicator : public color_selector
{
  Q_OBJECT

public:
  color_indicator (QWidget *parent, QColor *color);

protected:
  virtual QSize sizeHint() const;
  virtual void paintEvent (QPaintEvent *event) override;
};

#endif // COLOR_INDICATOR_H

