#ifndef OVERRIDE_UNDO_FILTER_H
#define OVERRIDE_UNDO_FILTER_H
// class to remove undo (ctrl+z) working on spinboxes and line edits, just set it as eventfilter

#include <QObject>
#include <QWidget>

class override_undo_filter : public QObject
{
  Q_OBJECT

  QWidget *m_parent;

public:
  override_undo_filter (QWidget *parent);

protected:
  virtual bool eventFilter(QObject *target, QEvent *anEvent) override;
};

#endif // OVERRIDE_UNDO_FILTER_H

