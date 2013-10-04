#ifndef TOOLS_CONTAINER_H
#define TOOLS_CONTAINER_H

class svg_painter;
class abstract_tool;
class selector_tool;

class tools_container
{
  selector_tool *m_selector;

  abstract_tool *m_current_tool;
public:
  tools_container ();
  ~tools_container ();

  void update_tools (svg_painter *painter);

  abstract_tool *current_tool () const;

private:
  void remove_tools ();
};

#endif // TOOLS_CONTAINER_H
