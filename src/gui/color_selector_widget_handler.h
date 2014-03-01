#ifndef COLOR_SELECTOR_WIDGET_HANDLER_H
#define COLOR_SELECTOR_WIDGET_HANDLER_H

class QColor;
class QGridLayout;
class QLayout;
class QTabWidget;

class color_selector;
class gui_model;
class gui_widget_view;
class color_selector;

enum class color_single_selector_type;

#include <QWidget>
#include <QColor>

class color_selector_widget_handler : public QWidget
{
  Q_OBJECT

  QLayout *m_color_selector_layout;
  QTabWidget *m_tab_widget;
  unique_ptr<gui_widget_view> m_view;
  gui_model *m_model;

public:
  color_selector_widget_handler (gui_model *model);
  ~color_selector_widget_handler ();
private:
  void add_color_selectors ();
  void create_cmyk_widget();
  void create_hsv_tab();
  void create_hsl_tab();
  void create_rgb_tab();
  void add_typical_scroller_widget (QGridLayout *grid_layout, color_single_selector_type type);
  void register_color_selector (color_selector *selector);
};
#endif // COLOR_SELECTOR_WIDGET_HANDLER_H
