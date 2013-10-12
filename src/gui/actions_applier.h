#ifndef ACTIONS_APPLIER_H
#define ACTIONS_APPLIER_H

#include <vector>
#include <functional>

enum class gui_action_id;

class actions_applier
{
  typedef std::function<bool ()> action_type; 
  std::vector<action_type> m_actions;

public:
  actions_applier ();
  ~actions_applier ();

  void register_action (gui_action_id id, action_type func);
  bool apply_action (gui_action_id id);
};

#endif // ACTIONS_APPLIER_H
