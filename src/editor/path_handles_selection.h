#ifndef PATH_HANDLES_SELECTION_H
#define PATH_HANDLES_SELECTION_H

#include <map>
#include <set>
#include <string>
#include <QObject>

class items_selection;
class svg_document;
class abstract_attribute;

class QRectF;

class path_anchors_selection : public QObject
{
  Q_OBJECT

  items_selection *m_selection;
  svg_document *m_document;

  std::map<std::string, std::set<int>> m_selected_anchors;
  bool m_changes_started;

public:
  path_anchors_selection (items_selection *selection, svg_document *document);
  ~path_anchors_selection ();

  void add_anchor (const std::string &item, int id);
  void remove_anchor (const std::string &item, int id);
  void clear ();

  void update ();

  bool is_selected (const std::string &item, int id) const;

  void begin_changes () { m_changes_started = true; }
  void end_changes () { m_changes_started = false; }

  const std::map<std::string, std::set<int>> &selected_anchors () const { return m_selected_anchors; }

private slots:
  void attribute_changed (const std::string &sender, const abstract_attribute *computed_attribute);
};

#endif // PATH_HANDLES_SELECTION_H
