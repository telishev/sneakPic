#ifndef COLOR_SELECTOR_WIDGET_HANDLER_H
#define COLOR_SELECTOR_WIDGET_HANDLER_H

class QColor;
class QGridLayout;
class QLayout;
class QTabWidget;
class QWidget;

class color_selector;

enum class color_single_selector_type;

#include <QObject>

class color_selector_widget_handler : public QObject
{
  Q_OBJECT

  QLayout *m_color_selector_layout;
  QTabWidget *m_tab_widget;
  QColor *m_color;
  QList <color_selector *> m_color_widgets;

public:
  color_selector_widget_handler (QColor *color);
  QWidget *widget ();
  void set_color (QColor *color);

private:
  void add_color_selectors ();
  void create_cmyk_widget();
  void create_hsv_tab();
  void create_hsl_tab();
  void create_rgb_tab();

public slots:
  void update_colors_momentarily ();

private slots:
  void update_colors_finally ();
  void add_typical_scroller_widget (QGridLayout *grid_layout, color_single_selector_type type);

signals:
  void color_changed_momentarily ();
  void color_changing_finished ();
};
#endif // COLOR_SELECTOR_WIDGET_HANDLER_H
