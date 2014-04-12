#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <QVariant>
#include <QObject>

class gui_model_updater;

class gui_model : public QObject
{
  Q_OBJECT

  std::map<int, QVariant> m_current_changes;
  int m_changes_counter = 0;
public:
  gui_model () {}
  virtual ~gui_model () {}

  virtual QVariant data (int role) const = 0;
  void set_data (int role, QVariant new_data);
  gui_model_updater do_multi_change ();

signals:
  void data_changed (const std::set<int> &changes);

protected:
  virtual void set_model_data (const std::map<int, QVariant> &data_map) = 0;
  std::set<int> get_change_set (const std::map<int, QVariant> &data_map) const;

private:
  friend class gui_model_updater;
};

class gui_model_updater
{
  gui_model *m_model;
public:
  gui_model_updater (gui_model *model);
  ~gui_model_updater ();
  gui_model *operator-> ();
};


#endif // GUI_MODEL_H
