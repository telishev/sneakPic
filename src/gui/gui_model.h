#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <QVariant>
#include <QObject>

template<typename T>
class gui_model_observer
{
public:
  virtual ~gui_model_observer () {}

  virtual void data_changed_signal (const std::set<T> &changes) = 0;
};


template<typename ROLE>
class gui_model;

template<typename ROLE>
class gui_model_updater
{
  gui_model<ROLE> *m_model;
public:
  gui_model_updater (gui_model<ROLE> *model)
  {
    m_model = model;
    m_model->m_changes_counter++;
  }

  ~gui_model_updater ()
  {
    if (!m_model)
      return;

    m_model->m_changes_counter--;
    if (m_model->m_changes_counter == 0)
      {
        m_model->set_model_data (m_model->m_current_changes);
        m_model->m_current_changes.clear ();
      }
  }

  gui_model<ROLE> *operator-> () { return m_model; }

};

template<typename ROLE>
class gui_model : public QObject
{
  std::map<ROLE, QVariant> m_current_changes;
  std::set<gui_model_observer<ROLE> *> m_observers;
  int m_changes_counter = 0;
public:
  gui_model () {}
  virtual ~gui_model () {}

  virtual QVariant data (ROLE role) const = 0;
  void set_data (ROLE role, QVariant new_data)
  {
    gui_model_updater<ROLE> updater (this);
    m_current_changes[role] = new_data;
  }
  gui_model_updater<ROLE> do_multi_change () { return gui_model_updater<ROLE> (this); }

  void add_observer (gui_model_observer<ROLE> *view) { m_observers.insert (view); }
  void remove_observer (gui_model_observer<ROLE> *view) { m_observers.erase (view); }

protected:
  virtual void set_model_data (const std::map<ROLE, QVariant> &data_map) = 0;

  std::set<ROLE> get_change_set (const std::map<ROLE, QVariant> &data_map) const
  {
    std::set<ROLE> result;

    for (auto && data_pair : data_map)
      result.insert (data_pair.first);

    return result;
  }

  void data_changed (const std::set<ROLE> &changes)
  {
    for (auto &&view : m_observers)
      view->data_changed_signal (changes);
  }

private:
  friend class gui_model_updater<ROLE>;
};

#endif // GUI_MODEL_H
