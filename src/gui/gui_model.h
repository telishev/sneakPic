#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <QVariant>
#include <QObject>

enum class gui_model_role_t
{
  /// stroke_config
  STROKE_WIDTH, /// double
  STROKE_MITER, /// double
  LINECAP, /// Qt::PenCapStyle
  LINEJOIN, // Qt::PenJoinStyle

  /// color

  FILL_COLOR, /// item_paint_server
  STROKE_COLOR, /// item_paint_server
  CURRENT_COLOR, /// QColor
  CURRENT_COLOR_TEMP, /// QColor

  IS_SELECTED_FILL, /// bool
};

class gui_model;

class gui_model_updater
{
  gui_model *m_model;
public:
  gui_model_updater (gui_model *model);
  ~gui_model_updater ();

  gui_model_updater (gui_model_updater &&rhs);

  gui_model *operator-> () { return m_model; }

private:
  gui_model_updater (const gui_model_updater &);
  gui_model_updater &operator= (const gui_model_updater &);

};

class gui_model : public QObject
{
  Q_OBJECT

  std::map<gui_model_role_t, QVariant> m_current_changes;
  int m_changes_counter;
public:
  gui_model ();
  virtual ~gui_model () {}

  virtual QVariant data (gui_model_role_t role) const = 0;
  void set_data (gui_model_role_t role, QVariant new_data);
  gui_model_updater do_multi_change () { return gui_model_updater (this); }

signals:
  void data_changed (const std::set<gui_model_role_t> &changes);

protected:
  virtual void set_model_data (const std::map<gui_model_role_t, QVariant> &data_map) = 0;

  std::set<gui_model_role_t> get_change_set (const std::map<gui_model_role_t, QVariant> &data_map) const;

private:
  friend class gui_model_updater;
};

#endif // GUI_MODEL_H
