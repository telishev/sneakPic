#ifndef OBJECT_OPERATIONS_HANDLER_H
#define OBJECT_OPERATIONS_HANDLER_H

class svg_painter;
class actions_applier;
class abstract_svg_item;
class QString;

class object_operations_handler
{
  svg_painter *m_painter;
  actions_applier *m_actions_applier;
public:
  object_operations_handler (svg_painter *painter, actions_applier *applier);
  ~object_operations_handler ();

private:
  bool clone_object ();
  bool duplicate ();
  bool unlink ();
  bool apply_for_selection (std::function<bool (abstract_svg_item *)> func, QString undo_name);
  bool clip ();
  bool group ();
  bool ungroup ();
  std::vector<abstract_svg_item *> items_in_selection () const;
};

#endif // OBJECT_OPERATIONS_HANDLER_H
