#ifndef PATH_OPERATIONS_HANDLER_H
#define PATH_OPERATIONS_HANDLER_H

class svg_painter;
class actions_applier;
class abstract_svg_item;
class QString;

class path_operations_handler
{
  svg_painter *m_painter;
  actions_applier *m_actions_applier;
public:
  path_operations_handler (svg_painter *painter, actions_applier *applier);
  ~path_operations_handler ();

private:
  bool objects_to_path ();
  bool apply_for_selection (std::function<bool (abstract_svg_item *)> func, QString undo_name);
};

#endif // PATH_OPERATIONS_HANDLER_H
