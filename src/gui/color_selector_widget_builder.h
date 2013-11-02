#ifndef COLOR_SELECTOR_WIDGET_BUILDER_H
#define COLOR_SELECTOR_WIDGET_BUILDER_H

class QColor;
class QGridLayout;
class QLayout;
class QTabWidget;
class QWidget;

class dock_widget_builder;
class color_selector;

enum class color_single_selector_type;

#include <QObject>

class color_selector_widget_builder : public QObject
{
  Q_OBJECT

  dock_widget_builder *m_dock_widget_builder;
  QWidget *m_color_selector_widget;
  QLayout *m_layout;
  QTabWidget *m_tab_widget;
  QColor *m_color;
  QList <color_selector *> m_color_widgets;

public:
  color_selector_widget_builder (dock_widget_builder *dock_widget_builder_arg, QColor *color);

private:
  void add_color_selectors ();

private slots:
  void update_colors ();
  void add_typical_scroller_widget (QGridLayout *grid_layout, color_single_selector_type type);

signals:
  void color_changed ();
};
#endif // COLOR_SELECTOR_WIDGET_BUILDER_H
