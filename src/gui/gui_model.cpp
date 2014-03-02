#include "gui_model.h"

gui_model_updater::gui_model_updater (gui_model *model)
{
  m_model = model;
  m_model->m_changes_counter++;
}

gui_model_updater::gui_model_updater (gui_model_updater &&rhs)
{
  m_model = nullptr;
  std::swap (m_model, rhs.m_model);
}

gui_model_updater::~gui_model_updater ()
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


void gui_model::set_data (gui_model_role_t role, QVariant new_data)
{
  gui_model_updater updater (this);
  m_current_changes[role] = new_data;
}

std::set<gui_model_role_t> gui_model::get_change_set (const std::map<gui_model_role_t, QVariant> &data_map) const
{
  std::set<gui_model_role_t> result;

  for (auto && data_pair : data_map)
    result.insert (data_pair.first);

  return result;
}

gui_model::gui_model ()
{
  m_changes_counter = 0;
}
